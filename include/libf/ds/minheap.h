/* minheap.h - minheap data structure definitions */

#ifndef LIBF_DS_MINHEAP_H
#define LIBF_DS_MINHEAP_H

#include <libf/config.h>
#include <libf/sl/xstdint.h>


/* @def: minheap */
#undef MINHEAP_NEW
#define MINHEAP_NEW(name, _array, _total_size) \
	struct minheap_head name = { \
		.array = _array, \
		.size = 0, \
		.total_size = _total_size \
		}

#undef MINHEAP_INIT
#define MINHEAP_INIT(name, _array, _total_size) \
	(name)->array = _array; \
	(name)->size = 0; \
	(name)->total_size = _total_size

#undef MINHEAP_PARENT
#define MINHEAP_PARENT(x) (((x) - 1) / 2)
#undef MINHEAP_LEFT
#define MINHEAP_LEFT(x) ((x) * 2 + 1)
#undef MINHEAP_RIGHT
#define MINHEAP_RIGHT(x) ((x) * 2 + 2)
#undef MINHEAP_SIZEOF
#define MINHEAP_SIZEOF (sizeof(void *))

struct minheap_head {
	void **array;
	uint64 size;
	uint64 total_size;
};

#undef MINHEAP_SIZE
#define MINHEAP_SIZE(x) ((x)->size)
#undef MINHEAP_ARRAY
#define MINHEAP_ARRAY(x, n) ((x)->array[n])
/* end */

#ifdef __cplusplus
extern "C" {
#endif

/* minheap.c */
int32 FSYMBOL(minheap_insert)(struct minheap_head *head, void *_new,
		int32 (*cmp)(void *, void *));
void FSYMBOL(minheap_build)(struct minheap_head *head,
		int32 (*cmp)(void *, void *));
int64 FSYMBOL(minheap_search)(struct minheap_head *head, void *p);
int64 FSYMBOL(minheap_search2)(struct minheap_head *head, void *arg,
		int32 (*cmp)(void *, void *));
int32 FSYMBOL(minheap_erase)(struct minheap_head *head, uint64 ind,
		int32 (*cmp)(void *, void *));
void* FSYMBOL(minheap_extract)(struct minheap_head *head,
		int32 (*cmp)(void *, void *));

#ifdef __cplusplus
}
#endif


#endif
