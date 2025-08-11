/* swisstable.h - swiss hash table definitions */

#ifndef LIBF_DS_SWISSTABLE_H
#define LIBF_DS_SWISSTABLE_H

#include <libf/config.h>
#include <libf/sl/xstdint.h>


/* @def: swisstable */
struct swisstable_group {
	/* TODO: simd optimization */
	uint8 ctrl;
};

struct swisstable_head {
	struct swisstable_group *group;
	void *array;
	uint64 wsize; /* members size */
	uint64 size;
	uint64 total_size;
	uint64L (*call_hash)(const void *, uint64);
	int32 (*call_cmp)(void *, const void *, uint64);
};

#undef SWISSTABLE_EMPTY
#define SWISSTABLE_EMPTY 0x80
#undef SWISSTABLE_DELETE
#define SWISSTABLE_DELETE 0xfe

#undef SWISSTABLE_FACTOR
#define SWISSTABLE_FACTOR(x) (((x)->size * 100) / (x)->total_size)

#undef SWISSTABLE_GROUP_ALIGN
#define SWISSTABLE_GROUP_ALIGN(n) (((n) + 0) / 1)
#undef SWISSTABLE_SIZE_ALIGN
#define SWISSTABLE_SIZE_ALIGN(x, n) ((x) * (((n) + 0) / 1))

#undef SWISSTABLE_NEW
#define SWISSTABLE_NEW(name, _ctrl, _array, _wsize, \
		_total_size, _hash, _cmp) \
	struct swisstable_head name = { \
		.group = _ctrl, \
		.array = _array, \
		.wsize = _wsize, \
		.size = 0, \
		.total_size = _total_size, \
		.call_hash = _hash, \
		.call_cmp = _cmp \
		}

#undef SWISSTABLE_INIT
#define SWISSTABLE_INIT(x, _ctrl, _array, _wsize, \
		_total_size, _hash, _cmp) \
	(x)->group = _ctrl; \
	(x)->array = _array; \
	(x)->wsize = _wsize; \
	(x)->size = 0; \
	(x)->total_size = _total_size; \
	(x)->call_hash = _hash; \
	(x)->call_cmp = _cmp
/* end */

#ifdef __cplusplus
extern "C" {
#endif

/* swisstable.c */
extern void *FSYMBOL(swisstable_insert)(struct swisstable_head *head,
		const void *key, uint64 len);
extern void *FSYMBOL(swisstable_find)(struct swisstable_head *head,
		const void *key, uint64 len);
extern void *FSYMBOL(swisstable_delete)(struct swisstable_head *head,
		const void *key, uint64 len);

#ifdef __cplusplus
}
#endif


#endif
