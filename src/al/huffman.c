/* huffman.c - huffman encode implementations */

#include <libf/config.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/ds/minheap.h>
#include <libf/al/huffman.h>


/* @func: _huf_cmp (static) - compare huffman frequencies
* @param1: void * # huffman node
* @param2: void * # huffman node
* @return: int32  # compare value
*/
static int32 _huf_cmp(void *a, void *b) {
	struct huf_node *_a = a, *_b = b;
	uint32 af = _a->freq, bf = _b->freq;

	return (af > bf) ? 1 : ((af < bf) ? -1 : 0);
} /* end */

/* @func: huf_insert - huffman node insert
* @param1: struct huf_head * # huffman head
* @param2: struct huf_node * # huffman node
* @return: int32             # 0: no error, -1: error array full
*/
int32 FSYMBOL(huf_insert)(struct huf_head *head, struct huf_node *node) {
	return FSYMBOL(minheap_insert)(&head->heap, node, _huf_cmp);
} /* end */

/* @func: huf_build - huffman tree building
* @param1: struct huf_head * # huffman head
* @return: struct huf_node * # huffman root / NULL pointer
*         $ 5        'Hello'
*       0/ \1        =======
*   2 'l'  $ 3       'l': 0
*        0/ \1       'e': 10
*     1 'e'  $ 2     'H': 110
*          0/ \1     'o': 111
*       1 'H' 'o' 1
*/
struct huf_node *FSYMBOL(huf_build)(struct huf_head *head) {
	if (MINHEAP_SIZE(&head->heap) < 2)
		return FSYMBOL(minheap_extract)(&head->heap, _huf_cmp);

	uint32 n = 0;
	struct huf_node *left, *right, *parent;
	while (MINHEAP_SIZE(&head->heap) != 1) {
		left = FSYMBOL(minheap_extract)(&head->heap, _huf_cmp);
		right = FSYMBOL(minheap_extract)(&head->heap, _huf_cmp);
		parent = &head->parent[n++];
		parent->left = left;
		parent->right = right;
		parent->data[0] = '$';
		parent->freq = left->freq + right->freq;

		FSYMBOL(minheap_insert)(&head->heap, parent, _huf_cmp);
	}

	return FSYMBOL(minheap_extract)(&head->heap, _huf_cmp);
} /* end */

/* @func: huf_u8freq - count character frequencies
* @param1: struct huf_node * # huffman node array (256 of node)
* @param2: uint8 *           # input buffer
* @param3: uint32            # input length
* @return: void
*/
void FSYMBOL(huf_u8freq)(struct huf_node *node, uint8 *s, uint32 len) {
	for (uint32 i = 0; i < len; i++)
		node[s[i]].freq++;
} /* end */

/* @func: huf_u8build - huffman tree building
* @param1: struct huf_head * # huffman head
* @param1: struct huf_node * # huffman node array (256 of node)
* @return: struct huf_node * # huffman root / NULL pointer
*/
struct huf_node *FSYMBOL(huf_u8build)(struct huf_head *head,
		struct huf_node *node) {
	for (int32 i = 0; i < 256; i++) {
		if (node[i].freq) {
			node[i].data[0] = i;
			if (FSYMBOL(huf_insert)(head, &node[i]))
				return NULL;
		}
	}

	return FSYMBOL(huf_build)(head);
} /* end */
