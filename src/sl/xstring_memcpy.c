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

#if 1 /* little endian */
#	define LS >>
#	define RS <<
#else
#	define LS <<
#	define RS >>
#endif

	volatile uint8 *_t = t, *_s = (uint8 *)s;
	uint32 w, x;

	for (; ((uint64)_s & 3) && len; len--)
		*_t++ = *_s++;

	switch ((uint64)_t & 3) {
		case 0:
			for (; len > 15; len -= 16) {
				((volatile uint32 *)_t)[0] = ((uint32 *)_s)[0];
				((volatile uint32 *)_t)[1] = ((uint32 *)_s)[1];
				((volatile uint32 *)_t)[2] = ((uint32 *)_s)[2];
				((volatile uint32 *)_t)[3] = ((uint32 *)_s)[3];
				_t += 16;
				_s += 16;
			}
			break;
		case 1:
			if (len < 32)
				break;

			w = *((uint32 *)_s);
			*_t++ = *_s++;
			*_t++ = *_s++;
			*_t++ = *_s++;
			len -= 3;

			for (; len > 16; len -= 16) {
				x = *((uint32 *)(_s + 1));
				((uint32 *)_t)[0] = (w LS 24) | (x RS 8);
				w = *((uint32 *)(_s + 5));
				((uint32 *)_t)[1] = (x LS 24) | (w RS 8);
				x = *((uint32 *)(_s + 9));
				((uint32 *)_t)[2] = (w LS 24) | (x RS 8);
				w = *((uint32 *)(_s + 13));
				((uint32 *)_t)[3] = (x LS 24) | (w RS 8);
				_t += 16;
				_s += 16;
			}
			break;
		case 2:
			if (len < 32)
				break;

			w = *((uint32 *)_s);
			*_t++ = *_s++;
			*_t++ = *_s++;
			len -= 2;

			for (; len > 17; len -= 16) {
				x = *((uint32 *)(_s + 2));
				((uint32 *)_t)[0] = (w LS 16) | (x RS 16);
				w = *((uint32 *)(_s + 6));
				((uint32 *)_t)[1] = (x LS 16) | (w RS 16);
				x = *((uint32 *)(_s + 10));
				((uint32 *)_t)[2] = (w LS 16) | (x RS 16);
				w = *((uint32 *)(_s + 14));
				((uint32 *)_t)[3] = (x LS 16) | (w RS 16);
				_t += 16;
				_s += 16;
			}
			break;
		case 3:
			if (len < 32)
				break;

			w = *((uint32 *)_s);
			*_t++ = *_s++;
			len -= 1;

			for (; len > 18; len -= 16) {
				x = *((uint32 *)(_s + 3));
				((uint32 *)_t)[0] = (w LS 8) | (x RS 24);
				w = *((uint32 *)(_s + 7));
				((uint32 *)_t)[1] = (x LS 8) | (w RS 24);
				x = *((uint32 *)(_s + 11));
				((uint32 *)_t)[2] = (w LS 8) | (x RS 24);
				w = *((uint32 *)(_s + 15));
				((uint32 *)_t)[3] = (x LS 8) | (w RS 24);
				_t += 16;
				_s += 16;
			}
			break;
		default:
			break;
	}

	if (len & 16) {
		*_t++ = *_s++; *_t++ = *_s++; *_t++ = *_s++; *_t++ = *_s++;
		*_t++ = *_s++; *_t++ = *_s++; *_t++ = *_s++; *_t++ = *_s++;
		*_t++ = *_s++; *_t++ = *_s++; *_t++ = *_s++; *_t++ = *_s++;
		*_t++ = *_s++; *_t++ = *_s++; *_t++ = *_s++; *_t++ = *_s++;
	}
	if (len & 8) {
		*_t++ = *_s++; *_t++ = *_s++; *_t++ = *_s++; *_t++ = *_s++;
		*_t++ = *_s++; *_t++ = *_s++; *_t++ = *_s++; *_t++ = *_s++;
	}
	if (len & 4) {
		*_t++ = *_s++; *_t++ = *_s++; *_t++ = *_s++; *_t++ = *_s++;
	}
	if (len & 2) {
		*_t++ = *_s++; *_t++ = *_s++;
	}
	if (len & 1)
		*_t++ = *_s++;

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

	volatile uint8 *_t = (uint8 *)t + len,
		*_s = (uint8 *)s + len;
	while (len--)
		*--_t = *--_s;

	return t;
} /* end */
