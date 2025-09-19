/* deflate.h - deflate compress algorithm definitions */

#ifndef LIBF_AL_DEFLATE_H
#define LIBF_AL_DEFLATE_H

#include <libf/config.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>


/* @def: deflate
* Length:
*   |Code|Extra bits|Length
*   |257 |0         |3
*   |258 |0         |4
*   |259 |0         |5
*   |260 |0         |6
*   |261 |0         |7
*   |262 |0         |8
*   |263 |0         |9
*   |264 |0         |10
*   |265 |1         |11-12
*   |266 |1         |13-14
*   |267 |1         |15-16
*   |268 |1         |17-18
*   |269 |2         |19-22
*   |270 |2         |23-26
*   |271 |2         |27-30
*   |272 |2         |31-34
*   |273 |3         |35-42
*   |274 |3         |43-50
*   |275 |3         |51-58
*   |276 |3         |59-66
*   |277 |4         |67-82
*   |278 |4         |83-98
*   |279 |4         |99-114
*   |280 |4         |115-130
*   |281 |5         |131-162
*   |282 |5         |163-194
*   |283 |5         |195-226
*   |284 |5         |227-257
*   |285 |0         |258
*
* Distance:
*   |Code|Extra bits|Distance
*   |0   |0         |1
*   |1   |0         |2
*   |2   |0         |3
*   |3   |0         |4
*   |4   |1         |5-6
*   |5   |1         |7-8
*   |6   |2         |9-12
*   |7   |2         |13-16
*   |8   |3         |17-24
*   |9   |3         |25-32
*   |10  |4         |33-48
*   |11  |4         |49-64
*   |12  |5         |65-96
*   |13  |5         |97-128
*   |14  |6         |129-192
*   |15  |6         |193-256
*   |16  |7         |257-384
*   |17  |7         |385-512
*   |18  |8         |513-768
*   |19  |8         |769-1024
*   |20  |9         |1025-1536
*   |21  |9         |1537-2048
*   |22  |10        |2049-3072
*   |23  |10        |3073-4096
*   |24  |11        |4097-6144
*   |25  |11        |6145-8192
*   |26  |12        |8193-12288
*   |27  |12        |12289-16384
*   |28  |13        |16385-24576
*   |29  |13        |24577-32768
*/

/* min matching length */
#undef DEFLATE_MATCH_MIN
#define DEFLATE_MATCH_MIN 3
/* max matching length */
#undef DEFLATE_MATCH_MAX
#define DEFLATE_MATCH_MAX 258
/* dictionary buffer size */
#undef DEFLATE_DICT_SIZE
#define DEFLATE_DICT_SIZE 32768

/* length codes number */
#undef DEFLATE_LEN_CODES
#define DEFLATE_LEN_CODES 29
/* distance codes number */
#undef DEFLATE_DIST_CODES
#define DEFLATE_DIST_CODES 30
/* bit-length codes number (15+3+1) */
#undef DEFLATE_BL_CODES
#define DEFLATE_BL_CODES 19

/* length mapping codes table size */
#undef DEFLATE_LEN_SIZE
#define DEFLATE_LEN_SIZE (DEFLATE_MATCH_MAX - DEFLATE_MATCH_MIN + 1)
/* distance mapping codes table size */
#undef DEFLATE_DIST_SIZE
#define DEFLATE_DIST_SIZE ((DEFLATE_MATCH_MAX - DEFLATE_MATCH_MIN + 1) * 2)

/* max bit length */
#undef DEFLATE_BITS_MAX
#define DEFLATE_BITS_MAX 15
/* literals number */
#undef DEFLATE_LITERALS
#define DEFLATE_LITERALS 256
/* end-block codes */
#undef DEFLATE_END_BLOCK
#define DEFLATE_END_BLOCK 256

/* literals/length codes number */
#undef DEFLATE_L_CODES
#define DEFLATE_L_CODES (DEFLATE_LITERALS + 1 + DEFLATE_LEN_CODES + 2)
/* distance codes number */
#undef DEFLATE_D_CODES
#define DEFLATE_D_CODES DEFLATE_DIST_CODES

/* huffman heap size */
#undef DEFLATE_HEAP_SIZE
#define DEFLATE_HEAP_SIZE (DEFLATE_L_CODES * 2 + 1)
/* dynamic literals/length tree size */
#undef DEFLATE_L_SIZE
#define DEFLATE_L_SIZE DEFLATE_HEAP_SIZE
/* dynamic distance tree size */
#undef DEFLATE_D_SIZE
#define DEFLATE_D_SIZE (DEFLATE_D_CODES * 2 + 1)

/* repeat previous bit length 3-6 times (2-bit) */
#undef DEFLATE_REP_3_6
#define DEFLATE_REP_3_6 16

/* repeat a zero length 3-10 times (3-bit) */
#undef DEFLATE_REPZ_3_10
#define DEFLATE_REPZ_3_10 17

/* repeat a zero length 11-138 times (7-bit) */
#undef DEFLATE_REPZ_11_138
#define DEFLATE_REPZ_11_138 18

#undef DEFLATE_BITS_BUFSIZE
#define DEFLATE_BITS_BUFSIZE 40960

#undef deflate_ctdata
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

#undef deflate_dyntree_ctx
struct deflate_dyntree_ctx {
	/* codes bitlen count */
	uint16 bitlen_count[DEFLATE_BITS_MAX + 1];
	/* minheap array (tree index) */
	uint16 heap[DEFLATE_HEAP_SIZE];
	/* node depth (use when the freq are same) */
	uint8 depth[DEFLATE_HEAP_SIZE];
	int32 size;     /* heap elems number */
	int32 size_max; /* heap-back pos */
};

#undef deflate_dyntree_desc
struct deflate_dyntree_desc {
	const struct deflate_ctdata *stree; /* static tree */
	struct deflate_ctdata *tree; /* dynamic tree */
	int32 elems;      /* codes number */
	int32 bitlen_max; /* max codes bits */
	int32 code_max;   /* max codes */
	int32 opt_slen;   /* static codes len */
	int32 opt_dlen;   /* dynamic codes len */
};
/* end */





#endif
