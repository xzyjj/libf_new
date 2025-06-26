//* ed25519_sign_ref.c - edwards-curve digital signature algorithm (eddsa) implementations */

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/al/bn_512.h>
#include <libf/al/sha512.h>
#include <libf/al/ed25519_ref.h>


/* @func: ed25519_sign_init - ed25519 init function
* @param1: struct ed25519_sign_ctx # ed25519-sign struct context
* @return: void
*/
void FSYMBOL(ed25519_sign_init)(struct ed25519_sign_ctx *ctx) {
	FSYMBOL(bn_int512_strtonum)(ctx->p,
		ED25519_P, NULL, 10);
	FSYMBOL(bn_int512_strtonum)(ctx->d,
		ED25519_D, NULL, 10);
	FSYMBOL(bn_int512_strtonum)(ctx->q,
		ED25519_Q, NULL, 10);

	FSYMBOL(bn_int512_strtonum)(ctx->xyz1.x,
		ED25519_Bx1, NULL, 10);
	FSYMBOL(bn_int512_strtonum)(ctx->xyz1.y,
		ED25519_By1, NULL, 10);
	FSYMBOL(bn_int512_strtonum)(ctx->xyz1.z,
		ED25519_Bz1, NULL, 10);
	FSYMBOL(bn_int512_strtonum)(ctx->xyz1.t,
		ED25519_Bt1, NULL, 10);
} /* end */

/* @func: ed25519_sign_private - ed25519 private key operation function
* @param1: struct ed25519_sign_ctx # ed25519-sign struct context
* @param2: const uint8 *           # private key (length: ED25519_LEN)
* @return: void
*/
void FSYMBOL(ed25519_sign_private)(struct ed25519_sign_ctx *ctx,
		const uint8 *key) {
	SHA512_NEW(sha_ctx);
	FSYMBOL(sha512_init)(&sha_ctx);
	FSYMBOL(sha512)(&sha_ctx, key, ED25519_LEN);

	/* copy(pri, sha, ED25519_LEN) */
	FSYMBOL(bn_int512_zero)(ctx->pri);
	XSYMBOL(memcpy)(ctx->pri, &(SHA512_STATE(&sha_ctx, 0)),
		ED25519_LEN);
	FSYMBOL(ed25519_clamp_key)(ctx->pri);

	/* copy(ran, sha + ED25519_LEN, ED25519_LEN) */
	FSYMBOL(bn_int512_zero)(ctx->ran);
	XSYMBOL(memcpy)(ctx->ran, &(SHA512_STATE(&sha_ctx, ED25519_LEN)),
		ED25519_LEN);
} /* end */

/* @func: ed25519_sign_public - ed25519 public key create
* @param1: struct ed25519_sign_ctx # ed25519-sign struct context
* @return: void
*/
void FSYMBOL(ed25519_sign_public)(struct ed25519_sign_ctx *ctx) {
	struct ed25519_point xyz2;

	/* pub = compress(p, scalar(p, d, pri, xyz1)) */
	FSYMBOL(ed25519_scalar_mul)(ctx->p, ctx->d, ctx->pri,
		&ctx->xyz1, &xyz2);
	FSYMBOL(ed25519_point_compress)(ctx->p, &xyz2, ctx->pub);
} /* end */

/* @func: ed25519_sign - ed25519 signature function
* @param1: struct ed25519_sign_ctx # ed25519-sign struct context
* @param2: const uint8 * # input message
* @param3: uint64        # message length
* @return: void
*/
void FSYMBOL(ed25519_sign)(struct ed25519_sign_ctx *ctx,
		const uint8 *mesg, uint64 len) {
	bn_int512_t t, r, Rs, h, s;
	struct ed25519_point R;
	SHA512_NEW(sha_ctx);

	/* sh = sha(ran + mesg) */
	FSYMBOL(sha512_init)(&sha_ctx);
	FSYMBOL(sha512_process)(&sha_ctx, (uint8 *)ctx->ran, ED25519_LEN);
	FSYMBOL(sha512_process)(&sha_ctx, mesg, len);
	FSYMBOL(sha512_finish)(&sha_ctx, len + ED25519_LEN);

	/* r = copy(r, sh, SHA512_LEN) % q */
	FSYMBOL(bn_int512_zero)(r);
	XSYMBOL(memcpy)(r, &(SHA512_STATE(&sha_ctx, 0)), SHA512_LEN);
	FSYMBOL(bn_int512_divmod)(t, r, r, ctx->q);

	/* Rs = compress(p, scalar(p, d, r, xyz1)) */
	FSYMBOL(ed25519_scalar_mul)(ctx->p, ctx->d, r, &ctx->xyz1, &R);
	FSYMBOL(ed25519_point_compress)(ctx->p, &R, Rs);

	/* sh = sha(Rs + pub + mesg) */
	FSYMBOL(sha512_init)(&sha_ctx);
	FSYMBOL(sha512_process)(&sha_ctx, (uint8 *)Rs, ED25519_LEN);
	FSYMBOL(sha512_process)(&sha_ctx, (uint8 *)ctx->pub, ED25519_LEN);
	FSYMBOL(sha512_process)(&sha_ctx, mesg, len);
	FSYMBOL(sha512_finish)(&sha_ctx, len + (ED25519_LEN * 2));

	/* h = copy(h, sh, SHA512_LEN) % q */
	FSYMBOL(bn_int512_zero)(h);
	XSYMBOL(memcpy)(h, &(SHA512_STATE(&sha_ctx, 0)), SHA512_LEN);
	FSYMBOL(bn_int512_divmod)(t, h, h, ctx->q);

	/* s = ((h * pri) + r) % q */
	FSYMBOL(bn_int512_mul)(s, h, ctx->pri);
	FSYMBOL(bn_int512_add)(s, s, r);
	FSYMBOL(bn_int512_divmod)(t, s, s, ctx->q);

	FSYMBOL(bn_int512_move)(ctx->rs, Rs);
	FSYMBOL(bn_int512_move)(ctx->ss, s);
} /* end */

/* @func: ed25519_sign_verify - ed25519 signature verification function
* @param1: struct ed25519_sign_ctx # ed25519-sign struct context
* @param2: const uint8 * # public key (length: ED25519_LEN)
* @param3: const uint8 * # signature (rs + ss, length: ED25519_LEN * 2)
* @param4: const uint8 * # input message
* @param5: uint64        # message length
* @return: int32         # 0: success, 1: fail
*/
int32 FSYMBOL(ed25519_sign_verify)(struct ed25519_sign_ctx *ctx,
		const uint8 *key, const uint8 *sign,
		const uint8 *mesg, uint64 len) {
	bn_int512_t Rs, s, h, t, pub;
	struct ed25519_point A, R, sB, hA;
	SHA512_NEW(sha_ctx);

	FSYMBOL(bn_int512_zero)(pub);
	/* copy(pub, key, ED25519_LEN) */
	XSYMBOL(memcpy)(pub, key, ED25519_LEN);

	/* A = decompress(p, d, pub) */
	FSYMBOL(ed25519_point_decompress)(ctx->p, ctx->d, pub, &A);

	/* Rs = copy(s, sign, ED25519_LEN) */
	FSYMBOL(bn_int512_zero)(Rs);
	XSYMBOL(memcpy)(Rs, &sign[0], ED25519_LEN);
	/* R = decompress(p, d, Rs) */
	FSYMBOL(ed25519_point_decompress)(ctx->p, ctx->d, Rs, &R);

	/* s = copy(s, sign + ED25519_LEN, ED25519_LEN) */
	FSYMBOL(bn_int512_zero)(s);
	XSYMBOL(memcpy)(s, &sign[ED25519_LEN], ED25519_LEN);
	/* s >= q */
	if (FSYMBOL(bn_int512_cmp)(s, ctx->q) >= 0)
		return 1;

	/* sh = sha(Rs + pub + mesg) */
	FSYMBOL(sha512_init)(&sha_ctx);
	FSYMBOL(sha512_process)(&sha_ctx, (uint8 *)Rs, ED25519_LEN);
	FSYMBOL(sha512_process)(&sha_ctx, (uint8 *)pub, ED25519_LEN);
	FSYMBOL(sha512_process)(&sha_ctx, mesg, len);
	FSYMBOL(sha512_finish)(&sha_ctx, len + (ED25519_LEN * 2));

	/* h = copy(h, sh, SHA512_LEN) % q */
	FSYMBOL(bn_int512_zero)(h);
	XSYMBOL(memcpy)(h, &(SHA512_STATE(&sha_ctx, 0)), SHA512_LEN);
	FSYMBOL(bn_int512_divmod)(t, h, h, ctx->q);

	/* sB = scalar(p, d, s, xyz1) */
	FSYMBOL(ed25519_scalar_mul)(ctx->p, ctx->d, s, &ctx->xyz1, &sB);

	/* hA = scalar(p, d, h, A) */
	FSYMBOL(ed25519_scalar_mul)(ctx->p, ctx->d, h, &A, &hA);
	/* hA = add(p, d, R, hA) */
	FSYMBOL(ed25519_point_add)(ctx->p, ctx->d, &R, &hA, &hA);

	return FSYMBOL(ed25519_point_equal)(ctx->p, &sB, &hA);
} /* end */
