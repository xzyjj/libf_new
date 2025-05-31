/* x448_ecdh.c - x448 elliptic curve diffie-hellman (ecdh) key exchange implementations */

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/al/x448.h>


/* @func: x448_ecdh_init - x448 init function
* @param1: struct x448_ecdh_ctx # x448-ecdh struct context
* @return: void
*/
void FSYMBOL(x448_ecdh_init)(struct x448_ecdh_ctx *ctx) {
	FSYMBOL(bn_int1024_strtonum)(ctx->p,
		X448_P, NULL, 10);
	FSYMBOL(bn_int1024_strtonum)(ctx->a,
		X448_A, NULL, 10);
	FSYMBOL(bn_int1024_strtonum)(ctx->b,
		X448_B, NULL, 10);
} /* end */

/* @func: x448_ecdh_init - x448 private key operation function
* @param1: struct x448_ecdh_ctx # x448-ecdh struct context
* @param2: const uint8 *          # private key (length: X448_LEN)
* @return: void
*/
void FSYMBOL(x448_ecdh_private)(struct x448_ecdh_ctx *ctx,
		const uint8 *key) {
	/* copy(ctx->pri, key, X448_LEN) */
	FSYMBOL(bn_int1024_zero)(ctx->pri);
	XSYMBOL(memcpy)(ctx->pri, key, X448_LEN);

	FSYMBOL(x448_clamp_key)(ctx->pri);
} /* end */

/* @func: x448_ecdh_public - x448 public key create
* @param1: struct x448_ecdh_ctx # x448-ecdh struct context
* @return: void
*/
void FSYMBOL(x448_ecdh_public)(struct x448_ecdh_ctx *ctx) {
	FSYMBOL(x448_scalar_mul)(ctx->p, ctx->a, ctx->pri,
		ctx->b, ctx->pub);
} /* end */

/* @func: x448_ecdh_exchange - x448 key exchange function
* @param1: struct x448_ecdh_ctx # x448-ecdh struct context
* @param2: const uint8 * # public key (other side, length: X448_LEN)
* @return: void
*/
void FSYMBOL(x448_ecdh_exchange)(struct x448_ecdh_ctx *ctx,
		const uint8 *key) {
	bn_uint1024_t pub;
	FSYMBOL(bn_int1024_zero)(pub);
	/* copy(pub, key, X448_LEN) */
	XSYMBOL(memcpy)(pub, key, X448_LEN);

	FSYMBOL(x448_scalar_mul)(ctx->p, ctx->a, ctx->pri,
		pub, ctx->key);
} /* end */
