/* base64.h - base64 encode/decode definitions */

#ifndef LIBF_AL_BASE64_H
#define LIBF_AL_BASE64_H

#include <libf/config.h>
#include <libf/sl/xstdint.h>


#define BASE64_ENC_IALIGN(x) (3 * (((x) + 2) / 3))
#define BASE64_ENC_OALIGN(x) (4 * (((x) + 2) / 3))

#define BASE64_DEC_IALIGN(x) BASE64_ENC_OALIGN(x)
#define BASE64_DEC_OALIGN(x) ((3 * BASE64_DEC_IALIGN(x)) / 4)

#ifdef __cplusplus
extern "C" {
#endif

/* base64.c */
void FSYMBOL(base64_enc)(const char *s, uint32 slen, char *t, uint32 *tlen);
int32 FSYMBOL(base64_dec)(const char *s, uint32 slen, char *t, uint32 *tlen);

#ifdef __cplusplus
}
#endif


#endif
