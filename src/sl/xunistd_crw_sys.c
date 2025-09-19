/* xunistd_file_sys.c - implementations */

#include <libf/config.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>


/* @func: close - close a file descriptor
* @param1: int32 # file descriptor
* @return: int32 # 0: no error
*/
int32 XSYMBOL(close)(int32 fd) {
	return 0;
} /* end */

/* @func: read - read from file descriptor data
* @param1: int32  # file descriptor
* @param2: void * # buffer
* @param3: uint64 # length
* @return: int64  # >0: read length
*/
int64 XSYMBOL(read)(int32 fd, void *buf, uint64 len) {
	return 0;
} /* end */

/* @func: write - write data to file descriptor
* @param1: int32        # file descriptor
* @param2: const void * # input buffer
* @param3: uint64       # length
* @return: int64        # >0: write length
*/
int64 XSYMBOL(write)(int32 fd, const void *buf, uint64 len) {
	return 0;
} /* end */
