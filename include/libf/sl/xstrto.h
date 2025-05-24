/* xstrto.h - string and number convert definitions */

#ifndef LIBF_SL_XSTRTO_H
#define LIBF_SL_XSTRTO_H

#include <libf/config.h>
#include <libf/sl/xstdint.h>


#ifdef __cplusplus
extern "C" {
#endif

/* xstrto_strtoul.c */
extern int32 XSYMBOL(atoi)(const char *s);
extern int64 XSYMBOL(atol)(const char *s);
extern int64L XSYMBOL(atoll)(const char *s);
extern int64 XSYMBOL(strtol)(const char *s, char **e, int32 b);
extern uint64 XSYMBOL(strtoul)(const char *s, char **e, int32 b);
extern int64L XSYMBOL(strtoll)(const char *s, char **e, int32 b);
extern uint64L XSYMBOL(strtoull)(const char *s, char **e, int32 b);

/* xstrto_ultostr.c */
extern int32 XSYMBOL(ulltostr_o)(int32 n, char *p, uint64L v);
extern int32 XSYMBOL(ulltostr_d)(int32 n, char *p, uint64L v);
extern int32 XSYMBOL(ulltostr_x)(int32 n, char *p, uint64L v);
extern int32 XSYMBOL(ulltostr_X)(int32 n, char *p, uint64L v);
extern char *XSYMBOL(ultostr)(char *buf, uint64 v, int32 type);
extern char *XSYMBOL(ulltostr)(char *buf, uint64L v, int32 type);

#ifdef __cplusplus
}
#endif


#endif
