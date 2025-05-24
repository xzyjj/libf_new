/* sha256.h - sha256 hash algorithm definitions */

#ifndef LIBF_AL_SHA256_H
#define LIBF_AL_SHA256_H

#include <libf/config.h>
#include <libf/sl/xstdint.h>


/* @def: sha256 */
struct sha256_ctx {
	uint32 count;
	uint32 state[8];
	uint8 buf[64];
};

#undef SHA256_LEN
#define SHA256_LEN 32
#undef SHA256_NEW
#define	SHA256_NEW(x) \
	struct sha256_ctx x = { \
		.count = 0, \
		.state = { \
		0x6a09e667, 0xbb67ae85, 0x3c6ef372, \
		0xa54ff53a, 0x510e527f, 0x9b05688c, \
		0x1f83d9ab, 0x5be0cd19 \
		} }

#undef SHA224_LEN
#define SHA224_LEN 28
#undef SHA224_NEW
#define	SHA224_NEW(x) \
	struct sha256_ctx x = { \
		.count = 0, \
		.state = { \
		0xc1059ed8, 0x367cd507, 0x3070dd17, \
		0xf70e5939, 0xffc00b31, 0x68581511, \
		0x64f98fa7, 0xbefa4fa4 \
		} }

#undef SHA256_STATE
#define SHA256_STATE(x, n) (((uint8 *)((x)->state))[n])
#undef SHA224_STATE
#define SHA224_STATE(x, n) (((uint8 *)((x)->state))[n])
/* end */

#ifdef __cplusplus
extern "C" {
#endif

/* sha256.c */
void FSYMBOL(sha256_init)(struct sha256_ctx *ctx);
void FSYMBOL(sha224_init)(struct sha256_ctx *ctx);
void FSYMBOL(sha256_process)(struct sha256_ctx *ctx, const uint8 *s,
		uint64 len);
void FSYMBOL(sha256_finish)(struct sha256_ctx *ctx, uint64L len);
void FSYMBOL(sha256)(struct sha256_ctx *ctx, const uint8 *s,
		uint64 len);

#ifdef __cplusplus
}
#endif


#endif
