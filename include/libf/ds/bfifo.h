/* bfifo.h - first-in-first-out (fifo) buffer definitions */

#ifndef LIBF_DS_BFIFO_H
#define LIBF_DS_BFIFO_H

#include <libf/config.h>
#include <libf/sl/xstdint.h>


/* @def: bfifo */
#undef bfifo_ctx
struct bfifo_ctx {
	char *buf;
	uint32 size;
	uint32 in;
	uint32 out;
};

#undef BFIFO_NEW
#define BFIFO_NEW(x, buf, size) \
	struct bfifo_ctx x = { \
		buf, size, 0, 0 \
		}

#undef BFIFO_INIT
#define BFIFO_INIT(x, buf, size) \
	(x)->buf = buf; \
	(x)->size = len; \
	(x)->in = 0; \
	(x)->out = 0

#undef BFIFO_USED
#define BFIFO_USED(x) ((x)->in - (x)->out)
#undef BFIFO_AVAIL
#define BFIFO_AVAIL(x) ((x)->size - ((x)->in - (x)->out))

#undef BFIFO_IS_EMPTY
#define BFIFO_IS_EMPTY(x) ((x)->in == (x)->out)
#undef BFIFO_IS_FULL
#define BFIFO_IS_FULL(x) ((x)->size == ((x)->in - (x)->out))

#undef BFIFO_RESET
#define BFIFO_RESET(x) (x)->in = (x)->out = 0
/* end */

#ifdef __cplusplus
extern "C" {
#endif

/* bfifi.c */
extern uint32 FSYMBOL(bfifo_in)(struct bfifo_ctx *ctx, const void *buf, uint32 len);
extern uint32 FSYMBOL(bfifo_out)(struct bfifo_ctx *ctx, void *buf, uint32 len);
extern uint32 FSYMBOL(bfifo_out_peek)(struct bfifo_ctx *ctx, void *buf, uint32 len,
		uint32 offset);

#ifdef __cplusplus
}
#endif


#endif
