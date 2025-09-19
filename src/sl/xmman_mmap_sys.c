/* xmman_mmap_sys.c - memory management declarations implementations */

#include <libf/config.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xmman_sys.h>
#include <libf/sl/xsyscall_sys.h>


/* @func: mmap - create a new memory pages or file/device mapping
* @param1: void * # starting address (NULL kernel allocation)
* @param2: uint64 # mapping length (page-aligne)
* @param3: int32  # memory protection flags
* @param4: int32  # mapping argument flags
* @param5: int32  # file descriptor
* @param5: uint64 # file descriptor offset (page-aligne)
* @return: void * # return memory address / NULL
*/
void *XSYMBOL(mmap)(void *addr, uint64 len, int32 prot, int32 flags,
		int32 fd, uint64 off) {
	return NULL;
} /* end */

/* @func: munmap - unmap memory pages
* @param1: void * # starting address of the head or tail (page-aligne)
* @param2: uint64 # mapping length (page-aligne)
* @return: int32  # 0: no error, -1: error
*/
int32 XSYMBOL(munmap)(void *addr, uint64 len) {
	return 0;
} /* end */

/* @func: mprotect - set protection for a memory region
* @param1: void * # starting address of the head or tail (page-aligne)
* @param2: uint64 # mapping length (page-aligne)
* @param3: int32  # memory protection flags
* @return: int32  # 0: no error, -1: error
*/
int32 XSYMBOL(mprotect)(void *addr, uint64 len, int32 prot) {
	return 0;
} /* end */

/* @func: msync - synchronize a file with a memory mapping
* @param1: void * # starting address of the head or tail (page-aligne)
* @param2: uint64 # mapping length (page-aligne)
* @param3: int32  # flags
* @return: int32  # 0: no error, -1: error
*/
int32 XSYMBOL(msync)(void *addr, uint64 len, int32 flags) {
	return 0;
} /* end */

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
