/* blowfish.h - blowfish symmetric-key block cipher definitions */

#ifndef LIBF_AL_BLOWFISH_H
#define LIBF_AL_BLOWFISH_H

#include <libf/config.h>
#include <libf/sl/xstdint.h>


/* @def: blowfish */
#undef blowfish_ctx
struct blowfish_ctx {
	uint32 P[18];
	uint32 S1[256];
	uint32 S2[256];
	uint32 S3[256];
	uint32 S4[256];
};

#undef BLOWFISH_NEW
#define BLOWFISH_NEW(x) struct blowfish_ctx x
/* end */

#ifdef __cplusplus
extern "C" {
#endif

/* blowfish.c */
extern void FSYMBOL(blowfish_encrypt)(struct blowfish_ctx *ctx,
		uint32 *xl, uint32 *xr);
extern void FSYMBOL(blowfish_decrypt)(struct blowfish_ctx *ctx,
		uint32 *xl, uint32 *xr);
extern void FSYMBOL(blowfish_setkey)(struct blowfish_ctx *ctx, const uint8 *key,
		uint32 len);
extern void FSYMBOL(blowfish_ekskey)(struct blowfish_ctx *ctx, const uint8 *data,
		uint32 data_len, const uint8 *key, uint32 key_len);
extern void FSYMBOL(blowfish_init)(struct blowfish_ctx *ctx, const uint8 *key,
		uint32 len);

#ifdef __cplusplus
}
#endif


#endif
