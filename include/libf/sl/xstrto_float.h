/* xstrto_float.h - string and number convert definitions */

#ifndef LIBF_SL_XSTRTO_FLOAT_H
#define LIBF_SL_XSTRTO_FLOAT_H

#include <libf/config.h>
#include <libf/sl/xstdint.h>


#ifdef __cplusplus
extern "C" {
#endif

/* xstrto_fltostr_float.c */
extern int32 XSYMBOL(fltostr_num)(int32 n, char *p, float64 v, int32 pre);
extern char *XSYMBOL(fltostr)(char *buf, float64 v, int32 type, int32 pre);

/* xstrto_strtofl_float.c */
extern float64 XSYMBOL(atof)(const char *s);
extern float32 XSYMBOL(strtof)(const char *s, char **e);
extern float64 XSYMBOL(strtod)(const char *s, char **e);

#ifdef __cplusplus
}
#endif


#endif
