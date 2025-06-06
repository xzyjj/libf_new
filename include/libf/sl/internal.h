/* internal.h - internal definitions */

#ifndef LIBF_SL_INTERNAL_H
#define LIBF_SL_INTERNAL_H

#include <libf/config.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstdarg.h>
#include <libf/sl/xfloat.h>
#include <libf/ds/list.h>


/* @def: ftoa */
#undef FLTO_PREMAX
#define FLTO_PREMAX 324
#undef FLTO_LENMAX
#define FLTO_LENMAX 680
/* end */

/* @def: fmt  */
#undef fmt_ctx
struct fmt_ctx {
	int64 align;
	int64 precise;
	int32 flags;
	char specifiers;
	union {
		int64L _i64L;
		uint64L _u64L;
		float64 _f64;
		char _char;
		const char *_str;
	} va;
	void *arg;
	/* character, size, arg */
	int32 (*call_pad)(char, int64, void *);
	/* string, length, arg */
	int32 (*call_out)(const char *, uint64, void *);
};

#undef FMT_NEW
#define FMT_NEW(name, _call_pad, _call_out, _arg) \
	struct fmt_ctx name = { \
		.call_pad = _call_pad, \
		.call_out = _call_out, \
		.arg = _arg \
	}

#undef FMT_INIT
#define FMT_INIT(name, _call_pad, _call_out, _arg) \
	(name)->call_pad = _call_pad; \
	(name)->call_out = _call_out; \
	(name)->arg = _arg
/* end */

/* @def: umalloc
* 32/64bit are 8/16byte aligend */
#undef umalloc_chunk
struct umalloc_chunk {
	uint32 prev_size;
	uint32 size_flags;
};

#undef umalloc_chunk_node
struct umalloc_chunk_node {
	struct list list;
	uint64 size;
	struct umalloc_chunk chunk;
};

#undef umalloc_ctx
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

/* internal_fmt.c */
extern int32 XSYMBOL(internal_fmt)(struct fmt_ctx *ctx, const char *fmt,
		va_list ap);

/* internal_ftoa.c */
extern int32 XSYMBOL(internal_fltostr_num)(int32 n, char *p, float64 v, int32 pre);

/* internal_itoa.c */
extern int32 XSYMBOL(internal_ulltostr_o)(int32 n, char *p, uint64L v);
extern int32 XSYMBOL(internal_ulltostr_d)(int32 n, char *p, uint64L v);
extern int32 XSYMBOL(internal_ulltostr_x)(int32 n, char *p, uint64L v);
extern int32 XSYMBOL(internal_ulltostr_X)(int32 n, char *p, uint64L v);

/* internal_umalloc.c */
extern void *XSYMBOL(internal_umalloc)(struct umalloc_ctx *ctx, uint64 size);
extern void XSYMBOL(internal_ufree)(struct umalloc_ctx *ctx, void *p);
extern void XSYMBOL(internal_ufree_all)(struct umalloc_ctx *ctx);
extern uint64 XSYMBOL(internal_umalloc_idle)(struct umalloc_ctx *ctx, int32 type);

#ifdef __cplusplus
}
#endif


#endif
