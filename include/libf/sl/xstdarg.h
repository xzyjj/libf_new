/* xstdarg.h - variable argument list definitions (deps compiler) */

#ifndef LIBF_SL_XSTDARG_H
#define LIBF_SL_XSTDARG_H

#include <libf/config.h>


/* @def: stdarg
* variable argument list */
#undef va_list
typedef __builtin_va_list va_list;

#undef va_start
#define va_start(ap, fmt) __builtin_va_start(ap, fmt)
#undef va_arg
#define va_arg(ap, type) __builtin_va_arg(ap, type)
#undef va_end
#define va_end(ap) __builtin_va_end(ap)
#undef va_copy
#define va_copy(dest, sour) __builtin_va_copy(dest, sour)
/* end */


#endif
