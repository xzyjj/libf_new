/* x25519_ecdh.c - x25519 elliptic curve diffie-hellman (ecdh) \
*                  key exchange implementations */

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/al/x25519.h>


/* @func: x25519_ecdh_init - x25519 init function
* @param1: struct x25519_ecdh_ctx # x25519-ecdh struct context
* @return: void
*/
void FSYMBOL(x25519_ecdh_init)(struct x25519_ecdh_ctx *ctx) {
	FSYMBOL(bn_int512_strtonum)(ctx->p,
		X25519_P, NULL, 10);
	FSYMBOL(bn_int512_strtonum)(ctx->a,
		X25519_A, NULL, 10);
	FSYMBOL(bn_int512_strtonum)(ctx->b,
		X25519_B, NULL, 10);
} /* end */

/* @func: x25519_ecdh_private - x25519 private key operation function
* @param1: struct x25519_ecdh_ctx # x25519-ecdh struct context
* @param2: const uint8 *          # private key (length: X25519_LEN)
* @return: void
*/
void FSYMBOL(x25519_ecdh_private)(struct x25519_ecdh_ctx *ctx,
		const uint8 *key) {
	/* copy(ctx->pri, key, X25519_LEN) */
	FSYMBOL(bn_int512_zero)(ctx->pri);
	XSYMBOL(memcpy)(ctx->pri, key, X25519_LEN);

	FSYMBOL(x25519_clamp_key)(ctx->pri);
} /* end */

/* @func: x25519_ecdh_public - x25519 public key create
* @param1: struct x25519_ecdh_ctx # x25519-ecdh struct context
* @return: void
*/
void FSYMBOL(x25519_ecdh_public)(struct x25519_ecdh_ctx *ctx) {
	FSYMBOL(x25519_scalar_mul)(ctx->p, ctx->a, ctx->pri,
		ctx->b, ctx->pub);
} /* end */

/* @func: x25519_ecdh_exchange - x25519 key exchange function
* @param1: struct x25519_ecdh_ctx # x25519-ecdh struct context
* @param2: const uint8 * # public key (other side, length: X25519_LEN)
* @return: void
*/
void FSYMBOL(x25519_ecdh_exchange)(struct x25519_ecdh_ctx *ctx,
		const uint8 *key) {
	bn_uint512_t pub;
	FSYMBOL(bn_int512_zero)(pub);
	/* copy(pub, key, X25519_LEN) */
	XSYMBOL(memcpy)(pub, key, X25519_LEN);

	FSYMBOL(x25519_base_mask)(pub);

	FSYMBOL(x25519_scalar_mul)(ctx->p, ctx->a, ctx->pri,
		pub, ctx->key);
} /* end */
