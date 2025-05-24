/* xalloc_umalloc.h - memory allocator definitions */

#ifndef LIBF_SL_XALLOC_UMALLOC_H
#define LIBF_SL_XALLOC_UMALLOC_H

#include <libf/config.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/ds/list.h>


/* @def: umalloc
* 32/64bit are 8/16byte aligend */
struct umalloc_chunk {
	uint32 prev_size;
	uint32 size_flags;
};

struct umalloc_chunk_node {
	struct list list;
	uint64 size;
	struct umalloc_chunk chunk;
};

struct umalloc_ctx {
	struct list_head chunk;
	struct umalloc_chunk *idle;
	void *arg;
	/* size, arg */
	void *(*call_alloc)(uint64, void *);
	/* alloc pointer, size, arg */
	void (*call_free)(void *, uint64, void *);
};

#undef UMALLOC_NEW
#define UMALLOC_NEW(name, alloc, free, _arg) \
	struct umalloc_ctx name = { \
		.chunk.node = NULL, \
		.call_alloc = alloc, \
		.call_free = free, \
		.arg = _arg \
		}

#undef UMALLOC_INIT
#define UMALLOC_INIT(name, alloc, free, _arg) \
	(name)->chunk.node = NULL; \
	(name)->call_alloc = alloc; \
	(name)->call_free = free; \
	(name)->arg = _arg
/* end */

#ifdef __cplusplus
extern "C" {
#endif

/* xalloc_umalloc.c */
extern void *XSYMBOL(umalloc)(struct umalloc_ctx *ctx, uint64 size);
extern void XSYMBOL(ufree)(struct umalloc_ctx *ctx, void *p);
extern void XSYMBOL(ufree_all)(struct umalloc_ctx *ctx);
extern uint64 XSYMBOL(umalloc_idle)(struct umalloc_ctx *ctx, int32 type);

#ifdef __cplusplus
}
#endif


#endif
