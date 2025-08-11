/* bits_add.h - add bits to buffer definitions */

#ifndef LIBF_DS_BITS_ADD_H
#define LIBF_DS_BITS_ADD_H

#include <libf/config.h>
#include <libf/sl/xstdint.h>


/* @def: bits_add */
#undef BITS_ADD_BUFSIZE
#define BITS_ADD_BUFSIZE 16

#undef bits_add_ctx
struct bits_add_ctx {
	uint8 buf[BITS_ADD_BUFSIZE];
	uint32 count;
	uint16 blen; /* bit length */
	uint16 brem; /* remaining bit length */
};

#undef BITS_ADD_NEW
#define BITS_ADD_NEW(name) \
	struct bits_add_ctx name = { \
		.count = 0, \
		.blen = 0, .brem = 0 \
		}

#undef BITS_ADD_INIT
#define BITS_ADD_INIT(x) \
	(x)->count = 0; \
	(x)->blen = 0; \
	(x)->brem = 0

#undef BITS_ADD_FLUSH
#define BITS_ADD_FLUSH(x) \
	(x)->count = 0

#undef BITS_ADD_BUF
#define BITS_ADD_BUF(x) ((x)->buf)
#undef BITS_ADD_GETSIZE
#define BITS_ADD_GETSIZE(x) ((x)->count)
#undef BITS_ADD_BLEN
#define BITS_ADD_BLEN(x) ((x)->blen)
#undef BITS_ADD_BREM
#define BITS_ADD_BREM(x) ((x)->brem)
/* end */

#ifdef __cplusplus
extern "C" {
#endif

/* bits_add.c */
extern int32 FSYMBOL(bits_add)(struct bits_add_ctx *ctx, uint32 v, uint32 len);

#ifdef __cplusplus
}
#endif


#endif
