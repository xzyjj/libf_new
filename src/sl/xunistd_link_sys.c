/* xunistd_link_sys.c - implementations */

#include <libf/config.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>


/* @func: unlink - remove a directory entry (file)
* @param1: const char * # file path
* @return: int32        # 0: no error
*/
int32 XSYMBOL(unlink)(const char *path) {
	return 0;
} /* end */

/* @func: unlinkat - remove a directory entry (file/directory)
* @param1: int32        # dir file descriptor
* @param2: const char * # file path
* @param3: int32        # flags
* @return: int32        # 0: no error
*/
int32 XSYMBOL(unlinkat)(int32 dirfd, const char *path, int32 flags) {
	return 0;
} /* end */

/* @func: symlink - create a new symbolic link
* @param1: const char * # link path
* @param2: const char * # new link
* @return: int32        # 0: no error
*/
int32 XSYMBOL(symlink)(const char *spath, const char *tpath) {
	return 0;
} /* end */

/* @func: symlinkat - create a new symbolic link
* @param1: const char * # link path
* @param2: int32        # dir file descriptor
* @param3: const char * # new link
* @return: int32        # 0: no error
*/
int32 XSYMBOL(symlinkat)(const char *spath, int32 tdirfd,
		const char *tpath) {
	return 0;
} /* end */

/* @func: readlink - read a symbolic link target
* @param1: const char * # file path
* @param2: char *       # path buffer
* @param3: uint64       # length
* @return: int32        # 0: no error
*/
int32 XSYMBOL(readlink)(const char *path, char *buf, uint64 len) {
	return 0;
} /* end */

/* @func: readlinkat - read a symbolic link target
* @param1: int32        # dir file descriptor
* @param2: const char * # file path
* @param3: char *       # path buffer
* @param4: uint64       # length
* @return: int32        # 0: no error
*/
int32 XSYMBOL(readlinkat)(int32 dirfd, const char *path, char *buf,
		uint64 len) {
	return 0;
} /* end */

/* @func: link - create a link directory entry (file)
* @param1: const char * # link path
* @param2: const char * # new link
* @return: int32        # 0: no error
*/
int32 XSYMBOL(link)(const char *spath, const char *tpath) {
	return 0;
} /* end */

/* @func: linkat - create a link directory entry (file)
* @param1: int32        # dir file descriptor
* @param2: const char * # link path
* @param3: int32        # dir file descriptor
* @param4: const char * # new link
* @param5: int32        # flags
* @return: int32        # 0: no error
*/
int32 XSYMBOL(linkat)(int32 sdirfd, const char *spath, int32 tdirfd,
		const char *tpath, int32 flags) {
	return 0;
} /* end */
