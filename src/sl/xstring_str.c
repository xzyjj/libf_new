/* xstring_str.c - string operations implementations */

#include <libf/config.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>


/* @func: memchr - scan memory for a character
* @param1: const void * # source pointer
* @param2: uint8        # character
* @param3: uint64       # source length
* @return: void *       # target location / NULL pointer
*/
void *XSYMBOL(memchr)(const void *s, uint8 c, uint64 len) {
	for (uint8 *tmp = (uint8 *)s; len--; tmp++) {
		if (*tmp == c)
			return (void *)tmp;
	}

	return NULL;
} /* end */

/* @func: memrchr - scan memory for a character from the back
* @param1: const void * # source pointer
* @param2: uint8        # character
* @param3: uint64       # source length
* @return: void *       # target location / NULL pointer
*/
void *XSYMBOL(memrchr)(const void *s, uint8 c, uint64 len) {
	for (uint8 *tmp = (uint8 *)s + len; len--; ) {
		if (*--tmp == c)
			return (void *)tmp;
	}

	return NULL;
} /* end */

/* @func: memcmp - compare memory areas
* @param1: const void * # source1 pointer
* @param2: const void * # source2 pointer
* @param3: uint64       # length
* @return: int32        # 0: no error, 0<N>0: *s2 - *s1
*/
int32 XSYMBOL(memcmp)(const void *s1, const void *s2, uint64 len) {
	if (!len)
		return 0;

	uint8 *t1 = (uint8 *)s1, *t2 = (uint8 *)s2;
	for (; --len && *t1 == *t2; t1++, t2++);

	return *t1 - *t2;
} /* end */

/* @func: strlen - calculate the length of a string
* @param1: const char * # string pointer
* @return: uint64       # string length
*/
uint64 XSYMBOL(strlen)(const char *s) {
	const char *a = s;
	for (; *s != '\0'; s++);

	return (uint64)(s - a);
} /* end */

/* @func: strnlen - calculate the length of a string
* @param1: const char * # string pointer
* @param2: uint64       # string length-max
* @return: uint64       # string length
*/
uint64 XSYMBOL(strnlen)(const char *s, uint64 len) {
	const char *a = s;
	for (; len && *s != '\0'; s++, len--);

	return (uint64)(s - a);
} /* end */

/* @func: strcpy - copy string
* @param1: char *       # target pointer
* @param2: const char * # source pointer
* @return: char *       # target pointer
*/
char *XSYMBOL(strcpy)(char *t, const char *s) {
	char *a = t;
	while (*s != '\0')
		*t++ = *s++;
	*t = '\0';

	return a;
} /* end */

/* @func: strncpy - copy the string into the buffer and zero the rest
* @param1: char *       # target pointer
* @param2: const char * # source pointer
* @param3: uint64       # target length-max
* @return: char *       # target pointer
*/
char *XSYMBOL(strncpy)(char *t, const char *s, uint64 len) {
	char *a = t;
	for (; len && *s != '\0'; len--)
		*t++ = *s++;
	XSYMBOL(memset)(t, 0, len);

	return a;
} /* end */

/* @func: strcat - string append or catenate
* @param1: char *       # target pointer
* @param2: const char * # source pointer
* @return: char *       # target pointer
*/
char *XSYMBOL(strcat)(char *t, const char *s) {
	XSYMBOL(strcpy)(t + XSYMBOL(strlen)(t), s);

	return t;
} /* end */

/* @func: strncat - string append or catenate
* @param1: char *       # target pointer
* @param2: const char * # source pointer
* @param3: uint64       # source length-max
* @return: char *       # target pointer
*/
char *XSYMBOL(strncat)(char *t, const char *s, uint64 len) {
	char *a = t;

	t += XSYMBOL(strlen)(t);
	for (; len && *s != '\0'; len--)
		*t++ = *s++;
	*t = '\0';

	return a;
} /* end */

/* @func: strchr - locate character in string
* @param1: const char * # string pointer
* @param2: uint8        # character
* @return: char *       # string pointer
*/
char *XSYMBOL(strchr)(const char *s, uint8 c) {
	for (; *s != '\0'; s++) {
		if (*((uint8 *)s) == c)
			return (char *)s;
	}

	return (*((uint8 *)s) == c) ? (char *)s : NULL;
} /* end */

/* @func: strrchr - locate the character in the string from the end
* @param1: const char * # string pointer
* @param2: uint8        # character
* @return: char *       # string pointer
*/
char *XSYMBOL(strrchr)(const char *s, uint8 c) {
	return XSYMBOL(memrchr)(s, c, XSYMBOL(strlen)(s) + 1);
} /* end */

/* @func: strcmp - compare two string
* @param1: const char * # string1 pointer
* @param2: const char * # string2 pointer
* @return: int32        # 0<N>0: *s1 - *s2
*/
int32 XSYMBOL(strcmp)(const char *s1, const char *s2) {
	for (; *s1 == *s2 && *s1 != '\0'; s1++, s2++);

	return *((uint8 *)s1) - *((uint8 *)s2);
} /* end */

/* @func: strncmp - compare two string
* @param1: const char * # string1 pointer
* @param2: const char * # string2 pointer
* @param3: uint64       # length
* @return: int32        # 0<N>0: *s1 - *s2
*/
int32 XSYMBOL(strncmp)(const char *s1, const char *s2, uint64 len) {
	if (!len)
		return 0;

	for (; --len && *s1 == *s2 && *s1 != '\0'; s1++, s2++);

	return *((uint8 *)s1) - *((uint8 *)s2);
} /* end */

/* @func: strstr - locate a substring
* @param1: const char * # string1 pointer
* @param2: const char * # string2 pointer
* @return: char *       # string1 location / NULL pointer
*/
char *XSYMBOL(strstr)(const char *s1, const char *s2) {
	uint64 n1 = XSYMBOL(strlen)(s1), n2 = XSYMBOL(strlen)(s2);
	for (uint64 n = 0; (n1 - n) >= n2; s1++, n++) {
		if (!XSYMBOL(memcmp)(s2, s1, n2))
			return (char *)s1;
	}

	return NULL;
} /* end */

/* @func: strpbrk - locate character set in string
* @param1: const char * # string pointer
* @param2: const char * # character set
* @return: char *       # string location / NULL pointer
*/
char *XSYMBOL(strpbrk)(const char *s, const char *cs) {
	for (; *s != '\0'; s++) {
		if (XSYMBOL(strchr)(cs, *s))
			return (char *)s;
	}

	return NULL;
} /* end */

/* @func: strspn - get the prefix length of a substring consisting \
*                  of a character set
* @param1: const char * # string pointer
* @param2: const char * # character set
* @return: uint64       # string location length
*/
uint64 XSYMBOL(strspn)(const char *s, const char *cs) {
	uint64 n = 0;
	for (; *s != '\0'; s++, n++) {
		if (!XSYMBOL(strchr)(cs, *s))
			break;
	}

	return n;
} /* end */

/* @func: strcspn - get the prefix length of the substring that \
*                   does not belong to the character set
* @param1: const char * # string pointer
* @param2: const char * # character set
* @return: uint64       # string location length
*/
uint64 XSYMBOL(strcspn)(const char *s, const char *cs) {
	uint64 n = 0;
	for (; *s != '\0'; s++, n++) {
		if (XSYMBOL(strchr)(cs, *s))
			return n;
	}

	return n;
} /* end */

/* @func: strtok_r - extract tokens from strings
* @param1: char *       # string pointer
* @param2: const char * # separation character set
* @param3: char **      # separation location save pointer
* @return: char *       # separation location / NULL pointer
*/
char *XSYMBOL(strtok_r)(char *s, const char *sp, char **sl) {
	if (!s)
		s = *sl;

	char *tmp = s;
	if (*tmp == '\0')
		return NULL;

	for (; *tmp != '\0'; tmp++) {
		if (XSYMBOL(strchr)(sp, *tmp)) {
			*sl = tmp + 1;
			*tmp = '\0';
			return s;
		}
	}
	*sl = tmp;

	return s;
} /* end */
