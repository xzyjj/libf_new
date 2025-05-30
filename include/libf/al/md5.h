/* md5.h - md5 hash algorithm definitions */

#ifndef LIBF_AL_MD5_H
#define LIBF_AL_MD5_H

#include <libf/config.h>
#include <libf/sl/xstdint.h>


/* @def: md5 */
#undef md5_ctx
struct md5_ctx {
	uint32 count;
	uint32 state[4];
	uint8 buf[64];
};

#undef MD5_LEN
#define MD5_LEN 16
#undef MD5_NEW
#define	MD5_NEW(x) \
	struct md5_ctx x = { \
		.count = 0, \
		.state = { \
		0x67452301, 0xefcdab89, \
		0x98badcfe, 0x10325476 \
		} }

#undef MD5_STATE
#define MD5_STATE(x, n) (((uint8 *)((x)->state))[n])
/* end */

#ifdef __cplusplus
extern "C" {
#endif

/* md5.c */
extern void FSYMBOL(md5_init)(struct md5_ctx *ctx);
extern void FSYMBOL(md5_process)(struct md5_ctx *ctx, const uint8 *s,
		uint64 len);
extern void FSYMBOL(md5_finish)(struct md5_ctx *ctx, uint64L len);
extern void FSYMBOL(md5)(struct md5_ctx *ctx, const uint8 *s, uint64 len);

#ifdef __cplusplus
}
#endif


#endif
