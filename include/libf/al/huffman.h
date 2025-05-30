/* huffman.h - huffman encode definitions */

#ifndef LIBF_AL_HUFFMAN_H
#define LIBF_AL_HUFFMAN_H

#include <libf/config.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/ds/minheap.h>


/* @def: huffman */
#undef huf_node
struct huf_node {
	uint8 data[4];
	uint32 freq;
	struct huf_node *left, *right;
};

#undef huf_head
struct huf_head {
	struct minheap_head heap;
	struct huf_node *parent;
};

#undef HUF_NEW
#define HUF_NEW(name, _array, _size, _parent) \
	struct huf_head name = { \
		.heap.array = _array, \
		.heap.total_size = _size, \
		.heap.size = 0, \
		.parent = _parent \
	}

#undef HUF_INIT
#define HUF_INIT(name, _array, _size, _parent) \
	(name)->heap.array = _array; \
	(name)->heap.total_size = _size; \
	(name)->heap.size = 0; \
	(name)->parent = _parent
/* end */

#ifdef __cplusplus
extern "C" {
#endif

/* huffman.c */
extern int32 FSYMBOL(huf_insert)(struct huf_head *head, struct huf_node *node);
extern struct huf_node *FSYMBOL(huf_build)(struct huf_head *head);
extern void FSYMBOL(huf_u8freq)(struct huf_node *node, uint8 *s, uint32 len);
extern struct huf_node *FSYMBOL(huf_u8build)(struct huf_head *head,
		struct huf_node *node);

#ifdef __cplusplus
}
#endif


#endif
