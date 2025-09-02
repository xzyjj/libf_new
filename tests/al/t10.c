#include <stdio.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>


#undef DEFLATE_LEN_CODES
#define DEFLATE_LEN_CODES 29

#undef DEFLATE_BITS_MAX
#define DEFLATE_BITS_MAX 15
#undef DEFLATE_LITERALS
#define DEFLATE_LITERALS 256

#undef DEFLATE_L_CODES
#define DEFLATE_L_CODES (DEFLATE_LITERALS + 1 + DEFLATE_LEN_CODES + 2)

#undef DEFLATE_HEAP_SIZE
#define DEFLATE_HEAP_SIZE (DEFLATE_L_CODES * 2)

struct deflate_ctdata {
	union {
		uint16 freq; /* character freq */
		uint16 code; /* huffman code */
	} fc;
	union {
		uint16 dad; /* huffman parent node */
		uint16 len; /* code length */
	} dl;
};

struct deflate_ctdata __tree[DEFLATE_HEAP_SIZE];

struct dyntree_ctx {
	/* codes bitlen count */
	uint16 bitlen_count[DEFLATE_BITS_MAX + 1];
	/* minheap array (tree index) */
	uint16 heap[DEFLATE_HEAP_SIZE];
	/* node depth (use when the freq are same) */
	uint8 depth[DEFLATE_HEAP_SIZE];
	int32 size;
	int32 size_max;
};

struct dyntree_desc {
	struct deflate_ctdata *tree;
	int32 elems;
	int32 bitlen_max;
	int32 code_max;
};

static uint16 _bit_reverse(uint16 c, uint16 len) {
	uint16 r = 0;
	while (len--) {
		r |= c & 1;
		r <<= 1;
		c >>= 1;
	}

	return r >> 1;
}

static void _gen_codes(struct deflate_ctdata *tree, const uint16 *bitlen_count,
		int32 n) {
	uint16 next_code[DEFLATE_BITS_MAX + 1];
	uint16 code = 0;

	for (int32 i = 1; i <= DEFLATE_BITS_MAX; i++) {
		code = (code + bitlen_count[i - 1]) << 1;
		next_code[i] = code;
	}

	for (int32 i = 0; i < n; i++) {
		uint32 len = tree[i].dl.len;
		if (!len)
			continue;
		tree[i].fc.code = _bit_reverse(next_code[len]++, len);
	}
}

static void _gen_bitlen(struct dyntree_ctx *ctx, struct dyntree_desc *desc) {
	struct deflate_ctdata *tree = desc->tree;
	int32 bitlen_max = desc->bitlen_max;
	int32 code_max = desc->code_max;
	int32 n, m, len, overflow = 0;

	tree[ctx->heap[ctx->size_max]].dl.len = 0; /* root */

	for (int32 i = ctx->size_max + 1; i < DEFLATE_HEAP_SIZE; i++) {
		n = ctx->heap[i];
		len = tree[tree[n].dl.dad].dl.len + 1;
		if (len > bitlen_max) {
			len = bitlen_max;
			overflow++;
		}

		tree[n].dl.len = len;
		if (n > code_max)
			continue;

		ctx->bitlen_count[len]++;
	}
	if (!overflow)
		return;

	do {
		n = bitlen_max - 1;
		while (!ctx->bitlen_count[n])
			n--;
		ctx->bitlen_count[n]--;
		ctx->bitlen_count[n + 1] += 2;
		ctx->bitlen_count[bitlen_max]--;
		overflow -= 2;
	} while (overflow > 0);

	len = DEFLATE_HEAP_SIZE;
	for (int32 i = bitlen_max; i > 0; i--) {
		n = ctx->bitlen_count[i];
		while (n--) {
			m = ctx->heap[--len];
			if (m > code_max)
				continue;
			if (tree[m].dl.len != i)
				tree[m].dl.len = i;
		}
	}
}

static void _pqdown(struct dyntree_ctx *ctx, struct deflate_ctdata *tree,
		int32 k) {
	int32 v = ctx->heap[k];
	int32 j = k * 2; /* left child of the k (start at index 1) */

	while (j <= ctx->size) {
		int32 l = ctx->heap[j];
		int32 r = ctx->heap[j + 1]; /* right child */

		if (j < ctx->size && (tree[r].fc.freq < tree[l].fc.freq
				|| (tree[r].fc.freq == tree[l].fc.freq
				&& ctx->depth[r] <= ctx->depth[l])
				)) {
			j++;
			l = ctx->heap[j];
		}

		if (tree[v].fc.freq < tree[l].fc.freq
				|| (tree[v].fc.freq == tree[l].fc.freq
				&& ctx->depth[v] <= ctx->depth[l])
				)
			break;

		ctx->heap[k] = ctx->heap[j];
		k = j;
		j *= 2;
	}

	ctx->heap[k] = v;
}

static void _build_tree(struct dyntree_ctx *ctx, struct dyntree_desc *desc) {
	struct deflate_ctdata *tree = desc->tree;
	int32 elems = desc->elems, code_max = -1;
	int32 n, m, node;
	ctx->size = 0;
	ctx->size_max = DEFLATE_HEAP_SIZE;

	for (int32 i = 0; i < elems; i++) {
		if (tree[i].fc.freq) {
			ctx->heap[++(ctx->size)] = i;
			ctx->depth[i] = 0;
			code_max = i;
		} else {
			tree[i].dl.len = 0;
		}
	}

	while (ctx->size < 2) {
		node = ctx->heap[++(ctx->size)] = (code_max < 2) ?
			++(code_max) : 0;
		ctx->depth[node] = 0;
		tree[node].fc.freq = 1;
	}
	desc->code_max = code_max;

	for (int32 k = ctx->size / 2; k > 0; k--)
		_pqdown(ctx, tree, k);

#undef MAX
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

	/*
	* Standardized Huffman coding, generating a coding
	* tree based on the codes length.
	*
	* freq:
	*   'A'(65): 2
	*   'B'(66): 2
	*   'C'(67): 7
	*   'D'(68): 4
	*
	* heap-back: 290 289 67 288 68 66 65
	*       290
	*       / \
	*     67  289
	*         / \
	*       68   288
	*            / \
	*          66   65
	*
	* skip 290 - 290.len     = 0
	* each 289 - 290.len + 1 = 1
	* each 67  - 290.len + 1 = 1
	* each 288 - 289.len + 1 = 2
	* each 68  - 289.len + 1 = 2
	* each 66  - 288.len + 1 = 3
	* each 65  - 288.len + 1 = 3
	*
	* len:
	*   'A'(65): 3
	*   'B'(66): 3
	*   'C'(67): 1
	*   'D'(68): 2
	*/

	node = elems;
	do {
		n = ctx->heap[1];
		ctx->heap[1] = ctx->heap[ctx->size--];
		_pqdown(ctx, tree, 1);
		m = ctx->heap[1];

		ctx->heap[--(ctx->size_max)] = n;
		ctx->heap[--(ctx->size_max)] = m;

		tree[node].fc.freq = tree[n].fc.freq + tree[m].fc.freq;
		tree[n].dl.dad = tree[m].dl.dad = node;
		ctx->depth[node] = MAX(ctx->depth[n], ctx->depth[m]) + 1;

		ctx->heap[1] = node++;
		_pqdown(ctx, tree, 1);
	} while (ctx->size > 1);

	ctx->heap[--(ctx->size_max)] = ctx->heap[1];

	for (int32 i = 0; i <= DEFLATE_BITS_MAX; i++)
		ctx->bitlen_count[i] = 0;

	_gen_bitlen(ctx, desc);
	_gen_codes(tree, ctx->bitlen_count, desc->code_max + 1);
}



int main(void) {
	struct dyntree_ctx ctx;
	struct dyntree_desc desc;

	desc.tree = __tree;
	desc.elems = DEFLATE_L_CODES;
	desc.bitlen_max = DEFLATE_BITS_MAX;

	for (int32 i = 0; i < DEFLATE_L_CODES; i++)
		__tree[i].fc.freq = 0;

#if 0

	for (int32 i = 0; i < DEFLATE_L_CODES; i++)
		__tree[i].fc.freq = ((1300 * (i + 1)) + (i & 7)) & 1800;

#else

	__tree['A'].fc.freq = 2;
	__tree['B'].fc.freq = 2;
	__tree['C'].fc.freq = 7;
	__tree['D'].fc.freq = 4;

#endif


	_build_tree(&ctx, &desc);
#if 0

	for (int32 i = 0; i < DEFLATE_L_CODES; i++)
		printf("%d: %u %u\n", i,
			__tree[i].fc.code, __tree[i].dl.len);

#else

	printf("%d %d\n", __tree['A'].fc.code, __tree['A'].dl.len);
	printf("%d %d\n", __tree['B'].fc.code, __tree['B'].dl.len);
	printf("%d %d\n", __tree['C'].fc.code, __tree['C'].dl.len);
	printf("%d %d\n", __tree['D'].fc.code, __tree['D'].dl.len);

#endif

	return 0;
}
