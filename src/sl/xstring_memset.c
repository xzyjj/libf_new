/* xstring_memset.c - string operations implementations */

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>


/* @func: memset - fill memory with a constant byte
* @param1: void * # source pointer
* @param2: int8   # character
* @param3: uint64 # source length
* @return: void * # source pointer
*/
void *XSYMBOL(memset)(void *s, int8 c, uint64 len) {
	int8 cc[4] = { c, c, c, c };
	volatile uint32 *_s = s;
	for (; len > (sizeof(uint32) * 4 - 1);
			len -= (sizeof(uint32) * 4)) {
		*_s++ = *((uint32 *)cc);
		*_s++ = *((uint32 *)cc);
		*_s++ = *((uint32 *)cc);
		*_s++ = *((uint32 *)cc);
	}
	for (; len > (sizeof(uint32) - 1);
			len -= sizeof(uint32))
		*_s++ = *((uint32 *)cc);

	for (volatile uint8 *_s1 = (uint8 *)_s; len--; )
		*_s1++ = c;

	return s;
} /* end */
