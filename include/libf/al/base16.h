/* base16.h - base16 encode/decode definitions */

#ifndef LIBF_AL_BASE16_H
#define LIBF_AL_BASE16_H

#include <libf/config.h>
#include <libf/sl/xstdint.h>


/* @def: base16 */
#undef BASE16_ENC_IALIGN
#define BASE16_ENC_IALIGN(x) (x)
#undef BASE16_ENC_OALIGN
#define BASE16_ENC_OALIGN(x) (2 * (((x) + 1) / 2))

#undef BASE16_DEC_IALIGN
#define BASE16_DEC_IALIGN(x) BASE16_ENC_OALIGN(x)
#undef BASE16_DEC_OALIGN
#define BASE16_DEC_OALIGN(x) (x)
/* end */

#ifdef __cplusplus
extern "C" {
#endif

/* base16.c */
extern void FSYMBOL(base16_enc)(const char *s, uint32 slen, char *t, uint32 *tlen);
extern int32 FSYMBOL(base16_dec)(const char *s, uint32 slen, char *t, uint32 *tlen);

#ifdef __cplusplus
}
#endif


#endif
