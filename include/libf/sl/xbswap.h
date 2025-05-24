/* xbswap.h - byte order swap definitions */

#ifndef LIBF_SL_XBSWAP_H
#define LIBF_SL_XBSWAP_H

#include <libf/config.h>


/* @def: bswap */
#undef BSWAP16
#define BSWAP16(x) \
	((((x) & 0xff) << 8) | (((x) & ff00) >> 8))

#undef BSWAP32
#define BSWAP32(x) \
	((((x) & 0xff) << 24) \
	| (((x) & 0xff00) << 8) \
	| (((x) & 0xff0000) >> 8) \
	| (((x) & 0xff000000) >> 24))

#undef BSWAP64
#define BSWAP64(x) \
	((((x) & 0xffULL) << 56) \
	| (((x) & 0xff00ULL) << 40) \
	| (((x) & 0xff0000ULL) << 24) \
	| (((x) & 0xff000000ULL) << 8) \
	| (((x) & 0xff00000000ULL) >> 8) \
	| (((x) & 0xff0000000000ULL) >> 24) \
	| (((x) & 0xff000000000000ULL) >> 40) \
	| (((x) & 0xff00000000000000ULL) >> 56))
/* end */


#endif
