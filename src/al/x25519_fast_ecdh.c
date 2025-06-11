/* x25519_fast_ecdh.c - x25519 elliptic curve diffie-hellman (ecdh) \
*                       key exchange implementations */

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/al/x25519_fast.h>


/* @func: x25519_fast_ecdh_init - x25519 init function
* @param1: struct x25519_ecdh_ctx # x25519-ecdh struct context
* @return: void
*/
void FSYMBOL(x25519_fast_ecdh_init)(struct x25519_fast_ecdh_ctx *ctx) {
	/* copy(ctx->b, X25519_FAST_B, X25519_FAST_LEN) */
	XSYMBOL(memcpy)(ctx->b, X25519_FAST_B, X25519_FAST_LEN);
} /* end */

/* @func: x25519_fast_ecdh_private - x25519 private key operation function
* @param1: struct x25519_ecdh_ctx # x25519-ecdh struct context
* @param2: const uint8 *          # private key (length: X25519_FAST_LEN)
* @return: void
*/
void FSYMBOL(x25519_fast_ecdh_private)(struct x25519_fast_ecdh_ctx *ctx,
		const uint8 *key) {
	/* copy(ctx->pri, key, X25519_FAST_LEN) */
	XSYMBOL(memcpy)(ctx->pri, key, X25519_FAST_LEN);

	FSYMBOL(x25519_fast_clamp_key)(ctx->pri);
} /* end */

/* @func: x25519_fast_ecdh_public - x25519 public key create
* @param1: struct x25519_fast_ecdh_ctx # x25519-ecdh struct context
* @return: void
*/
void FSYMBOL(x25519_fast_ecdh_public)(struct x25519_fast_ecdh_ctx *ctx) {
	FSYMBOL(x25519_fast_scalar_mul)(ctx->pri, ctx->b, ctx->pub);
} /* end */

/* @func: x25519_fast_ecdh_exchange - x25519 key exchange function
* @param1: struct x25519_fast_ecdh_ctx # x25519-ecdh struct context
* @param2: const uint8 * # public key (other side, length: X25519_FAST_LEN)
* @return: void
*/
void FSYMBOL(x25519_fast_ecdh_exchange)(struct x25519_fast_ecdh_ctx *ctx,
		const uint8 *key) {
	uint32 pub[8];
	/* copy(pub, key, X25519_FAST_LEN) */
	XSYMBOL(memcpy)(pub, key, X25519_FAST_LEN);

	FSYMBOL(x25519_fast_base_mask)(pub);

	FSYMBOL(x25519_fast_scalar_mul)(ctx->pri, pub, ctx->key);
} /* end */
