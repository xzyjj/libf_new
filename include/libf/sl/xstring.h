/* xstring.h - string operations definitions */

#ifndef LIBF_SL_XSTRING_H
#define LIBF_SL_XSTRING_H

#include <libf/config.h>
#include <libf/sl/xstdint.h>


#ifdef __cplusplus
extern "C" {
#endif

/* xstring_memcpy.c */
extern void *XSYMBOL(memcpy)(void *t, const void *s, uint64 len);
extern void *XSYMBOL(memmove)(void *t, const void *s, uint64 len);

/* xstring_memset.c */
extern void *XSYMBOL(memset)(void *s, int8 c, uint64 len);

/* xstring_str.c */
extern void *XSYMBOL(memchr)(const void *s, uint8 c, uint64 len);
extern void *XSYMBOL(memrchr)(const void *s, uint8 c, uint64 len);
extern int32 XSYMBOL(memcmp)(const void *s1, const void *s2, uint64 len);
extern uint64 XSYMBOL(strlen)(const char *s);
extern uint64 XSYMBOL(strnlen)(const char *s, uint64 len);
extern char *XSYMBOL(strcpy)(char *t, const char *s);
extern char *XSYMBOL(strncpy)(char *t, const char *s, uint64 len);
extern char *XSYMBOL(strcat)(char *t, const char *s);
extern char *XSYMBOL(strncat)(char *t, const char *s, uint64 len);
extern char *XSYMBOL(strchr)(const char *s, uint8 c);
extern char *XSYMBOL(strrchr)(const char *s, uint8 c);
extern int32 XSYMBOL(strcmp)(const char *s1, const char *s2);
extern int32 XSYMBOL(strncmp)(const char *s1, const char *s2, uint64 len);
extern char *XSYMBOL(strstr)(const char *s1, const char *s2);
extern char *XSYMBOL(strpbrk)(const char *s, const char *cs);
extern uint64 XSYMBOL(strspn)(const char *s, const char *cs);
extern uint64 XSYMBOL(strcspn)(const char *s, const char *cs);
extern char *XSYMBOL(strtok_r)(char *s, const char *sp, char **sl);

#ifdef __cplusplus
}
#endif


#endif
