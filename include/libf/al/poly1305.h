/* poly1305.h - poly1305 message authentication code definitions */

#ifndef LIBF_AL_POLY1305_H
#define LIBF_AL_POLY1305_H

#include <libf/config.h>
#include <libf/sl/xstdint.h>


/* @def: poly1305 */
#undef POLY1305_KEYLEN
#define POLY1305_KEYLEN 32
#undef POLY1305_TAGLEN
#define POLY1305_TAGLEN 16
#undef POLY1305_BLOCKSIZE
#define POLY1305_BLOCKSIZE 16

#undef poly1305_ctx
struct poly1305_ctx {
	uint32 h[5];
	uint32 r[5];
	uint32 s[5];
	uint32 tag[4];
	uint32 count;
	uint8 buf[POLY1305_BLOCKSIZE];
};

#undef POLY1305_NEW
#define POLY1305_NEW(x) struct poly1305_ctx x

#undef POLY1305_TAG
#define POLY1305_TAG(x, n) (((uint8 *)(x)->tag)[n])
/* end */

#ifdef __cplusplus
extern "C" {
#endif

/* poly1305.c */
extern void FSYMBOL(poly1305_init)(struct poly1305_ctx *ctx, const uint8 *key);
extern void FSYMBOL(poly1305_block)(struct poly1305_ctx *ctx, const uint8 *s,
		uint32 padbit);
extern void FSYMBOL(poly1305_process)(struct poly1305_ctx *ctx, const uint8 *s,
		uint64 len);
extern void FSYMBOL(poly1305_finish)(struct poly1305_ctx *ctx);
extern void FSYMBOL(poly1305)(struct poly1305_ctx *ctx, const uint8 *s, uint64 len);
extern int32 FSYMBOL(poly1305_tag_auth)(const uint8 *a, const uint8 *b);

#ifdef __cplusplus
}
#endif


#endif
