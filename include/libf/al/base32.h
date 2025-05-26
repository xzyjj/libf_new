/* base32.h - base32 encode/decode definitions */

#ifndef LIBF_AL_BASE32_H
#define LIBF_AL_BASE32_H

#include <libf/config.h>
#include <libf/sl/xstdint.h>


/* @def: base32 */
#undef BASE32_ENC_IALIGN
#define BASE32_ENC_IALIGN(x) (5 * (((x) + 4) / 5))
#undef BASE32_ENC_OALIGN
#define BASE32_ENC_OALIGN(x) (8 * (((x) + 4) / 5))

#undef BASE32_DEC_IALIGN
#define BASE32_DEC_IALIGN(x) BASE32_ENC_OALIGN(x)
#undef BASE32_DEC_OALIGN
#define BASE32_DEC_OALIGN(x) ((5 * BASE32_DEC_IALIGN(x)) / 8)
/* end */

#ifdef __cplusplus
extern "C" {
#endif

/* base32.c */
extern void FSYMBOL(base32_enc)(const char *s, uint32 slen, char *t, uint32 *tlen);
extern int32 FSYMBOL(base32_dec)(const char *s, uint32 slen, char *t, uint32 *tlen);

#ifdef __cplusplus
}
#endif


#endif
