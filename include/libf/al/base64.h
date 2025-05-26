/* base64.h - base64 encode/decode definitions */

#ifndef LIBF_AL_BASE64_H
#define LIBF_AL_BASE64_H

#include <libf/config.h>
#include <libf/sl/xstdint.h>


/* @def: base64 */
#undef BASE64_ENC_IALIGN
#define BASE64_ENC_IALIGN(x) (3 * (((x) + 2) / 3))
#undef BASE64_ENC_OALIGN
#define BASE64_ENC_OALIGN(x) (4 * (((x) + 2) / 3))

#undef BASE64_DEC_IALIGN
#define BASE64_DEC_IALIGN(x) BASE64_ENC_OALIGN(x)
#undef BASE64_DEC_OALIGN
#define BASE64_DEC_OALIGN(x) ((3 * BASE64_DEC_IALIGN(x)) / 4)
/* end */

#ifdef __cplusplus
extern "C" {
#endif

/* base64.c */
extern void FSYMBOL(base64_enc)(const char *s, uint32 slen, char *t, uint32 *tlen);
extern int32 FSYMBOL(base64_dec)(const char *s, uint32 slen, char *t, uint32 *tlen);

#ifdef __cplusplus
}
#endif


#endif
