/* rc4.c - rivest cipher 4 encrypt algorithm implementations */

#include <libf/config.h>
#include <libf/sl/xstddef.h>
#include <libf/al/rc4.h>


/* @func: rc4_init - rc4 key init
* @param1: struct rc4_ctx * # rc4 context struct
* @param2: const uint8 *    # key
* @param3: uint32           # key length
* @return: void
*/
void FSYMBOL(rc4_init)(struct rc4_ctx *ctx, const uint8 *key, uint32 len) {
	for (int32 i = 0; i < 256; i++)
		ctx->s[i] = i;

	uint8 t = 0;
	for (int32 i = 0, j = 0; i < 256; i++) {
		j = (j + ctx->s[i] + key[i % len]) % 256;

		t = ctx->s[i];
		ctx->s[i] = ctx->s[j];
		ctx->s[j] = t;
	}
	ctx->i = 0;
	ctx->j = 0;
} /* end */

/* @func: rc4_crypto - rc4 encrypt and decrypt (stream)
* @param1: struct rc4_ctx * # rc4 context struct
* @param2: uint8 *          # buffer
* @param3: uint32           # length
* @return: void
*/
void FSYMBOL(rc4_crypto)(struct rc4_ctx *ctx, uint8 *s, uint32 len) {
	int32 i = 0, j = 0;
	uint8 t = 0;
	for (uint32 k = 0; k < len; k++) {
		i = (i + 1) % 256;
		j = (j + ctx->s[i]) % 256;

		t = ctx->s[i];
		ctx->s[i] = ctx->s[j];
		ctx->s[j] = t;

		s[k] ^= ctx->s[(ctx->s[i] + ctx->s[j]) % 256];
	}
	ctx->i = i;
	ctx->j = j;
} /* end */
