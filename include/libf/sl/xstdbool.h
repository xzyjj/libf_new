/* xstdbool.h - boolean type definitions */

#ifndef LIBF_SL_XSTDBOOL_H
#define LIBF_SL_XSTDBOOL_H

#include <libf/config.h>


/* @def: stdbool */
#ifndef __cplusplus
#	undef bool
#	define bool _Bool

#	undef true
#	define true 1

#	undef false
#	define false 0
#endif
/* end */


#endif
