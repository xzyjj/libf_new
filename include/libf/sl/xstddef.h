/* xstddef.h - standard type definitions */

#ifndef LIBF_SL_XSTDDEF_H
#define LIBF_SL_XSTDDEF_H

#include <libf/config.h>


/* @def: stddef
* nil pointer */
#undef NULL
#ifdef __cplusplus
#	define NULL (nullptr)
#else
#	define NULL ((void *)0)
#endif

/* struct member offset size */
#undef offsetof
#define offsetof(type, member) \
	((uint64)&(((type *)0)->member))

/* struct member and container offset size */
#undef container_of
#define container_of(ptr, type, member) \
	((type *)((char *)(ptr) - offsetof(type, member)))

/* size_t and ssize_t are uint64 and int64 */

/* ptrdiff_t is int64 */

/* end */


#endif
