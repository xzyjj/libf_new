/* rc4.h - rivest cipher 4 encrypt algorithm definitions */

#ifndef LIBF_AL_RC4_H
#define LIBF_AL_RC4_H

#include <libf/config.h>
#include <libf/sl/xstdint.h>


/* @def: rc4 */
struct rc4_ctx {
	uint8 s[256];
	int32 i, j;
};

#undef RC4_NEW
#define RC4_NEW(x) struct rc4_ctx x
/* end */

#ifdef __cplusplus
extern "C" {
#endif

/* rc4.c */
void FSYMBOL(rc4_init)(struct rc4_ctx *ctx, const uint8 *key, uint32 len);
void FSYMBOL(rc4_crypto)(struct rc4_ctx *ctx, uint8 *s, uint32 len);

#ifdef __cplusplus
}
#endif


#endif
