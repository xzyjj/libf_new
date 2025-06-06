/* xstring_memcpy.c - copy memory area implementations */

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>


/* @func: memcpy - copy memory area
* @param1: void *       # target pointer
* @param2: const void * # source pointer
* @param3: uint64       # length
* @return: void *       # target pointer
*/
void *XSYMBOL(memcpy)(void *t, const void *s, uint64 len) {
#if 1
#	if 0

	for (char *tmp1 = t, *tmp2 = (char *)s; len--; )
		*tmp1++ = *tmp2++;

	return t;

#	else

	volatile uint64 *_t = t, *_s = (uint64 *)s;
	for (; len > (sizeof(uint64) * 4 - 1);
			len -= (sizeof(uint64) * 4)) {
		*_t++ = *_s++;
		*_t++ = *_s++;
		*_t++ = *_s++;
		*_t++ = *_s++;
	}
	for (; len > (sizeof(uint64) - 1);
			len -= sizeof(uint64))
		*_t++ = *_s++;

	for (volatile uint8 *_t1 = (uint8 *)_t,
			*_s1 = (uint8 *)_s;
			len--; )
		*_t1++ = *_s1++;

	return t;

#	endif
#else

	/* Duff's Device */
	char *tmp1 = t, *tmp2 = (char *)s;
	uint64 n = (len + 7) / 8;
	switch (len % 8) {
		case 0:
		do {
			*tmp1++ = *tmp2++;
		case 7:
			*tmp1++ = *tmp2++;
		case 6:
			*tmp1++ = *tmp2++;
		case 5:
			*tmp1++ = *tmp2++;
		case 4:
			*tmp1++ = *tmp2++;
		case 3:
			*tmp1++ = *tmp2++;
		case 2:
			*tmp1++ = *tmp2++;
		case 1:
			*tmp1++ = *tmp2++;
		} while (--n > 0);
	}

	return t;

#endif
} /* end */

/* @func: memmove - copy memory area (memory overlap and then copy)
* @param1: void *       # target pointer
* @param2: const void * # source pointer
* @param3: uint64       # length
* @return: void *       # target pointer
*/
void *XSYMBOL(memmove)(void *t, const void *s, uint64 len) {
	if (t <= s)
		return XSYMBOL(memcpy)(t, s, len);

#if 0

	for (char *tmp1 = (char *)t + len,
			*tmp2 = (char *)s + len;
			len--; )
		*--tmp1 = *--tmp2;

	return t;

#else

	volatile uint64 *_t = (uint64 *)((char *)t + len),
		*_s = (uint64 *)((char *)s + len);
	for (; len > (sizeof(uint64) * 4 - 1);
			len -= (sizeof(uint64) * 4)) {
		*--_t = *--_s;
		*--_t = *--_s;
		*--_t = *--_s;
		*--_t = *--_s;
	}
	for (; len > (sizeof(uint64) - 1);
			len -= sizeof(uint64))
		*--_t = *--_s;

	for (volatile uint8 *_t1 = (uint8 *)_t,
			*_s1 = (uint8 *)_s;
			len--; )
		*--_t1 = *--_s1;

	return t;

#endif
} /* end */
