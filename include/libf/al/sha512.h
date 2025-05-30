/* sha512.h - sha512 hash algorithm definitions */

#ifndef LIBF_AL_SHA512_H
#define LIBF_AL_SHA512_H

#include <libf/config.h>
#include <libf/sl/xstdint.h>


/* @def: sha512 */
#undef sha512_ctx
struct sha512_ctx {
	uint32 count;
	uint64L state[8];
	uint8 buf[128];
};

#undef SHA512_LEN
#define SHA512_LEN 64
#undef SHA512_NEW
#define	SHA512_NEW(x) \
	struct sha512_ctx x = { \
		.count = 0, \
		.state = { \
		0x6a09e667f3bcc908, 0xbb67ae8584caa73b, \
		0x3c6ef372fe94f82b, 0xa54ff53a5f1d36f1, \
		0x510e527fade682d1, 0x9b05688c2b3e6c1f, \
		0x1f83d9abfb41bd6b, 0x5be0cd19137e2179 \
		} }

#undef SHA384_LEN
#define SHA384_LEN 48
#undef SHA384_NEW
#define	SHA384_NEW(x) \
	struct sha512_ctx x = { \
		.count = 0, \
		.state = { \
		0xcbbb9d5dc1059ed8, 0x629a292a367cd507, \
		0x9159015a3070dd17, 0x152fecd8f70e5939, \
		0x67332667ffc00b31, 0x8eb44a8768581511, \
		0xdb0c2e0d64f98fa7, 0x47b5481dbefa4fa4 \
		} }

#undef SHA512_STATE
#define SHA512_STATE(x, n) (((uint8 *)((x)->state))[n])
#undef SHA384_STATE
#define SHA384_STATE(x, n) (((uint8 *)((x)->state))[n])
/* end */

#ifdef __cplusplus
extern "C" {
#endif

/* sha512.c */
extern void FSYMBOL(sha512_init)(struct sha512_ctx *ctx);
extern void FSYMBOL(sha384_init)(struct sha512_ctx *ctx);
extern void FSYMBOL(sha512_process)(struct sha512_ctx *ctx, const uint8 *s,
		uint64 len);
extern void FSYMBOL(sha512_finish)(struct sha512_ctx *ctx, uint64L len);
extern void FSYMBOL(sha512)(struct sha512_ctx *ctx, const uint8 *s, uint64 len);

#ifdef __cplusplus
}
#endif


#endif
