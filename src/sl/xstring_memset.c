/* xstring_memset.c - fill memory implementations */

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>


/* @func: memset - fill memory with a constant byte
* @param1: void * # source pointer
* @param2: int8   # character
* @param3: uint64 # source length
* @return: void * # source pointer
*/
void *XSYMBOL(memset)(void *t, int8 c, uint64 len) {
	int8 cc[4] = { c, c, c, c };
	volatile uint8 *_t = t;

	for (; ((uint64)_t & 3) && len; len--)
		*_t++ = c;

	for (; len > 15; len -= 16) {
		((uint32 *)_t)[0] = *((uint32 *)cc);
		((uint32 *)_t)[1] = *((uint32 *)cc);
		((uint32 *)_t)[2] = *((uint32 *)cc);
		((uint32 *)_t)[3] = *((uint32 *)cc);
		_t += 16;
	}
	for (; len > 3; len -= 4) {
		*((uint32 *)_t) = *((uint32 *)cc);
		_t += 4;
	}

	while (len--)
		*_t++ = c;

	return t;
} /* end */
