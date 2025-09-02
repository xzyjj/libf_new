/* bits_get.h - get bits from buffer definitions */

#ifndef LIBF_DS_BITS_GET_H
#define LIBF_DS_BITS_GET_H

#include <libf/config.h>
#include <libf/sl/xstdint.h>


/* @def: bits_get */
#undef bits_get_ctx
struct bits_get_ctx {
	const uint8 *buf;
	uint32 len;
	uint32 count;
	uint32 blen;
};

#undef BITS_GET_NEW
#define BITS_GET_NEW(name, _buf, _len) \
	struct bits_get_ctx name = { \
		.buf = _buf, \
		.len = _len, \
		.count = 0, \
		.blen = 0 \
		}

#undef BITS_GET_INIT
#define BITS_GET_INIT(x, _buf, _len) \
	(x)->buf = _buf; \
	(x)->len = _len; \
	(x)->count = 0; \
	(x)->blen = 0

#undef BITS_GET_RESET
#define BITS_GET_RESET(x) \
	do { \
		(x)->count = 0; \
		(x)->blen = 0; \
	} while (0)

#undef BITS_GET_GETSIZE
#define BITS_GET_GETSIZE(x) ((x)->count)
#undef BITS_GET_BLEN
#define BITS_GET_BLEN(x) ((x)->blen)

#undef BITS_GET_GETBITS
#define BITS_GET_GETBITS(x) \
	((BITS_GET_GETSIZE(x) << 3) + BITS_GET_BLEN(x))
#undef BITS_GET_BTASIZE
#define BITS_GET_BTASIZE(x) ((((x) + 7) & ~7) / 8)

#undef BITS_GET_SETBITS
#define BITS_GET_SETBITS(x, n) \
	do { \
		(x)->count = n / 8; \
		(x)->blen = n % 8; \
	} while (0)
/* end */

#ifdef __cplusplus
extern "C" {
#endif

/* bits_get.c */
int32 FSYMBOL(bits_get)(struct bits_get_ctx *ctx, uint32 *v, uint32 len,
		int32 peek);

#ifdef __cplusplus
}
#endif


#endif
