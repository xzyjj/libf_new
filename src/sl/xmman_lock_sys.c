/* xmman_lock_sys.c - memory management declarations implementations */

#include <libf/config.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xmman_sys.h>
#include <libf/sl/xsyscall_sys.h>


/* @func: mlock - lock memory pages
* @param1: void * # starting address of the head or tail (page-aligne)
* @param2: uint64 # mapping length (page-aligne)
* @return: int32  # 0: no error, -1: error
*/
int32 XSYMBOL(mlock)(const void *addr, uint64 len) {
	return 0;
} /* end */

/* @func: munlock - unlock memory pages
* @param1: void * # starting address of the head or tail (page-aligne)
* @param2: uint64 # mapping length (page-aligne)
* @return: int32 # 0: no error, -1: error
*/
int32 XSYMBOL(munlock)(const void *addr, uint64 len) {
	return 0;
} /* end */

/* @func: mlockall - lock all memory pages
* @param1: int32 # flags
* @return: int32 # 0: no error, -1: error
*/
int32 XSYMBOL(mlockall)(int32 flags) {
	return 0;
} /* end */

/* @func: munlockall - unlock all memory pages
* @return: int32 # 0: no error, -1: error
*/
int32 XSYMBOL(munlockall)(void) {
	return 0;
} /* end */
