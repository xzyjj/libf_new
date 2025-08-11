/* flate_tree.c - implementations */

#include <libf/config.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>


/* @def: flate
* length: match data length.
* distance: forward offset equal the matching data.
* extra bits: more compact encoding of 'length' and 'distance' data.
* actual length: 'length' + 'extra bits' (195 + ((1 << 5) - 1))
*
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
*   285: length 258 = ((227 + ((1 << 5) - 1)))
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

/* max bit length */
#undef FLATE_BITS_MAX
#define FLATE_BITS_MAX 15

/* literals character 0~255 */
#undef FLATE_LITERALS
#define FLATE_LITERALS 256

/* end of block */
#undef FLATE_END_BLOCK
#define FLATE_END_BLOCK 256

/* length codes 257~285 */
#undef FLATE_LENGTH_CODES
#define FLATE_LENGTH_CODES 29

/* distance codes 0~29 */
#undef FLATE_DISTANCE_CODES
#define FLATE_DISTANCE_CODES 30

/* length-base min */
#undef FLATE_MATCH_MIN
#define FLATE_MATCH_MIN 3

/* length-base max */
#undef FLATE_MATCH_MAX
#define FLATE_MATCH_MAX 258

/* literals + 1 (END 256) + length */
#undef FLATE_L_CODES
#define FLATE_L_CODES (FLATE_LITERALS + 1 + FLATE_LENGTH_CODES)

/* distance */
#undef FLATE_D_CODES
#define FLATE_D_CODES FLATE_DISTANCE_CODES

/* max bit length for bit length codes */
#undef FLATE_BL_BITS_MAX
#define FLATE_BL_BITS_MAX 15

/* bit length codes */
#undef FLATE_BL_CODES
#define FLATE_BL_CODES 19

/* heap size for dynamic tree */
#undef FLATE_HEAP_SIZE
#define FLATE_HEAP_SIZE (FLATE_L_CODES * 2 + 1)

/* heap size for dynamic distance tree */
#undef FLATE_D_HEAP_SIZE
#define FLATE_D_HEAP_SIZE (FLATE_D_CODES * 2 + 1)

/* heap size for dynamic bit length tree */
#undef FLATE_BL_HEAP_SIZE
#define FLATE_BL_HEAP_SIZE (FLATE_BL_CODES * 2 + 1)

struct flate_ctdata {
	union {
		uint16 freq; /* character freq */
		uint16 code; /* huffman code */
	} fc;
	union {
		uint16 dad; /* huffman parent node */
		uint16 len; /* code length */
	} dl;
};

struct flate_stree_desc {
	const struct flate_ctdata *static_tree; /* static tree or NULL */
	const uint8 *extra_bits; /* extra bits or NULL */
	int32 extra_base;        /* base index for extra_bits */
	int32 elems;             /* number of elements for the tree */
	int32 max_length;        /* max bit length for the codes */
};

struct flate_tree_desc {
	struct flate_ctdata *dyn_tree; /* dynamic tree */
	const flate_stree_desc *stree; /* static tree */
	int32 max_code; /* largest code with non zero frequency */
};

static const uint8 extra_lbits[FLATE_LENGTH_CODES] = {
	0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 2, 2, 2, 2,
	3, 3, 3, 3, 4, 4, 4, 4,
	5, 5, 5, 5, 0
	};

static const uint8 extra_dbits[FLATE_DISTANCE_CODES] = {
	 0,  0,  0,  0,  1,  1,  2,  2,
	 3,  3,  4,  4,  5,  5,  6,  6,
	 7,  7,  8,  8,  9,  9, 10, 10,
	11, 11, 12, 12, 13, 13
	};

static const uint8 extra_blbits[FLATE_BL_CODES] = {
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	2, 3, 7
	};

static const uint8 bl_order[FLATE_BL_CODES] = {
	16, 17, 18, 0,  8, 7,  9, 6,
	10,  5, 11, 4, 12, 3, 13, 2,
	14,  1, 15
	};

static const uint16 lbase[FLATE_DISTANCE_CODES] = {
	  3,   4,   5,   6,   7,  8,  9,  10,
	 11,  13,  15,  17,  19, 23, 27,  31,
	 35,  43,  51,  59,  67, 83, 99, 115,
	131, 163, 195, 227, 258
	};

static const uint16 dbase[FLATE_DISTANCE_CODES] = {
	   1,    2,    3,     4,     5,     7,    9,   13,
	  17,   25,   33,    49,    65,    97,  129,  193,
	 257,  385,  513,   769,  1025,  1537, 2049, 3073,
	4097, 6145, 8193, 12289, 16385, 24577
	};

static const uint16 base_length[FLATE_LENGTH_CODES] = {
	  0,   1,   2,   3,  4,  5,  6,   7,
	  8,  10,  12,  14, 16, 20, 24,  28,
	 32,  40,  48,  56, 64, 80, 96, 112,
	128, 160, 192, 224,  0
	};

static const uint16 base_distance[FLATE_DISTANCE_CODES] = {
	   0,    1,    2,     3,     4,     6,    8,   12,
	  16,   24,   32,    48,    64,    96,  128,  192,
	 256,  384,  512,   768,  1024,  1536, 2048, 3072,
	4096, 6144, 8192, 12288, 16384, 24576
	};

static const uint8 length_code[FLATE_MATCH_MAX - FLATE_MATCH_MIN + 1] = {
	 0,  1,  2,  3,  4,  5,  6,  7,  8,  8,  9,  9,
	10, 10, 11, 11, 12, 12, 12, 12, 13, 13, 13, 13,
	14, 14, 14, 14, 15, 15, 15, 15, 16, 16, 16, 16,
	16, 16, 16, 16, 17, 17, 17, 17, 17, 17, 17, 17,
	18, 18, 18, 18, 18, 18, 18, 18, 19, 19, 19, 19,
	19, 19, 19, 19, 20, 20, 20, 20, 20, 20, 20, 20,
	20, 20, 20, 20, 20, 20, 20, 20, 21, 21, 21, 21,
	21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21,
	22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
	22, 22, 22, 22, 23, 23, 23, 23, 23, 23, 23, 23,
	23, 23, 23, 23, 23, 23, 23, 23, 24, 24, 24, 24,
	24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
	24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
	24, 24, 24, 24, 25, 25, 25, 25, 25, 25, 25, 25,
	25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25,
	25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25,
	26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26,
	26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26,
	26, 26, 26, 26, 26, 26, 26, 26, 27, 27, 27, 27,
	27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27,
	27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27,
	27, 27, 27, 28
	};

static const uint8 distance_code[(FLATE_MATCH_MAX - FLATE_MATCH_MIN + 1) * 2] = {
	 0,  1,  2,  3,  4,  4,  5,  5,  6,  6,  6,  6,
	 7,  7,  7,  7,  8,  8,  8,  8,  8,  8,  8,  8,
	 9,  9,  9,  9,  9,  9,  9,  9, 10, 10, 10, 10,
	10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
	11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
	11, 11, 11, 11, 12, 12, 12, 12, 12, 12, 12, 12,
	12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
	12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
	13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
	13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
	13, 13, 13, 13, 13, 13, 13, 13, 14, 14, 14, 14,
	14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
	14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
	14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
	14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
	14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
	15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15,  0,  0, 16, 17, 18, 18, 19, 19,
	20, 20, 20, 20, 21, 21, 21, 21, 22, 22, 22, 22,
	22, 22, 22, 22, 23, 23, 23, 23, 23, 23, 23, 23,
	24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
	24, 24, 24, 24, 25, 25, 25, 25, 25, 25, 25, 25,
	25, 25, 25, 25, 25, 25, 25, 25, 26, 26, 26, 26,
	26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26,
	26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26,
	26, 26, 26, 26, 27, 27, 27, 27, 27, 27, 27, 27,
	27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27,
	27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27,
	28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
	28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
	28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
	28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
	28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
	28, 28, 28, 28, 29, 29, 29, 29, 29, 29, 29, 29,
	29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29,
	29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29,
	29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29,
	29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29,
	29, 29, 29, 29, 29, 29, 29, 29
	};

static const struct flate_ctdata static_ltree[FLATE_L_CODES + 2] = {
	{ {  12 }, { 8 } }, { { 140 }, { 8 } },
	{ {  76 }, { 8 } }, { { 204 }, { 8 } },
	{ {  44 }, { 8 } }, { { 172 }, { 8 } },
	{ { 108 }, { 8 } }, { { 236 }, { 8 } },
	{ {  28 }, { 8 } }, { { 156 }, { 8 } },
	{ {  92 }, { 8 } }, { { 220 }, { 8 } },
	{ {  60 }, { 8 } }, { { 188 }, { 8 } },
	{ { 124 }, { 8 } }, { { 252 }, { 8 } },
	{ {   2 }, { 8 } }, { { 130 }, { 8 } },
	{ {  66 }, { 8 } }, { { 194 }, { 8 } },
	{ {  34 }, { 8 } }, { { 162 }, { 8 } },
	{ {  98 }, { 8 } }, { { 226 }, { 8 } },
	{ {  18 }, { 8 } }, { { 146 }, { 8 } },
	{ {  82 }, { 8 } }, { { 210 }, { 8 } },
	{ {  50 }, { 8 } }, { { 178 }, { 8 } },
	{ { 114 }, { 8 } }, { { 242 }, { 8 } },
	{ {  10 }, { 8 } }, { { 138 }, { 8 } },
	{ {  74 }, { 8 } }, { { 202 }, { 8 } },
	{ {  42 }, { 8 } }, { { 170 }, { 8 } },
	{ { 106 }, { 8 } }, { { 234 }, { 8 } },
	{ {  26 }, { 8 } }, { { 154 }, { 8 } },
	{ {  90 }, { 8 } }, { { 218 }, { 8 } },
	{ {  58 }, { 8 } }, { { 186 }, { 8 } },
	{ { 122 }, { 8 } }, { { 250 }, { 8 } },
	{ {   6 }, { 8 } }, { { 134 }, { 8 } },
	{ {  70 }, { 8 } }, { { 198 }, { 8 } },
	{ {  38 }, { 8 } }, { { 166 }, { 8 } },
	{ { 102 }, { 8 } }, { { 230 }, { 8 } },
	{ {  22 }, { 8 } }, { { 150 }, { 8 } },
	{ {  86 }, { 8 } }, { { 214 }, { 8 } },
	{ {  54 }, { 8 } }, { { 182 }, { 8 } },
	{ { 118 }, { 8 } }, { { 246 }, { 8 } },
	{ {  14 }, { 8 } }, { { 142 }, { 8 } },
	{ {  78 }, { 8 } }, { { 206 }, { 8 } },
	{ {  46 }, { 8 } }, { { 174 }, { 8 } },
	{ { 110 }, { 8 } }, { { 238 }, { 8 } },
	{ {  30 }, { 8 } }, { { 158 }, { 8 } },
	{ {  94 }, { 8 } }, { { 222 }, { 8 } },
	{ {  62 }, { 8 } }, { { 190 }, { 8 } },
	{ { 126 }, { 8 } }, { { 254 }, { 8 } },
	{ {   1 }, { 8 } }, { { 129 }, { 8 } },
	{ {  65 }, { 8 } }, { { 193 }, { 8 } },
	{ {  33 }, { 8 } }, { { 161 }, { 8 } },
	{ {  97 }, { 8 } }, { { 225 }, { 8 } },
	{ {  17 }, { 8 } }, { { 145 }, { 8 } },
	{ {  81 }, { 8 } }, { { 209 }, { 8 } },
	{ {  49 }, { 8 } }, { { 177 }, { 8 } },
	{ { 113 }, { 8 } }, { { 241 }, { 8 } },
	{ {   9 }, { 8 } }, { { 137 }, { 8 } },
	{ {  73 }, { 8 } }, { { 201 }, { 8 } },
	{ {  41 }, { 8 } }, { { 169 }, { 8 } },
	{ { 105 }, { 8 } }, { { 233 }, { 8 } },
	{ {  25 }, { 8 } }, { { 153 }, { 8 } },
	{ {  89 }, { 8 } }, { { 217 }, { 8 } },
	{ {  57 }, { 8 } }, { { 185 }, { 8 } },
	{ { 121 }, { 8 } }, { { 249 }, { 8 } },
	{ {   5 }, { 8 } }, { { 133 }, { 8 } },
	{ {  69 }, { 8 } }, { { 197 }, { 8 } },
	{ {  37 }, { 8 } }, { { 165 }, { 8 } },
	{ { 101 }, { 8 } }, { { 229 }, { 8 } },
	{ {  21 }, { 8 } }, { { 149 }, { 8 } },
	{ {  85 }, { 8 } }, { { 213 }, { 8 } },
	{ {  53 }, { 8 } }, { { 181 }, { 8 } },
	{ { 117 }, { 8 } }, { { 245 }, { 8 } },
	{ {  13 }, { 8 } }, { { 141 }, { 8 } },
	{ {  77 }, { 8 } }, { { 205 }, { 8 } },
	{ {  45 }, { 8 } }, { { 173 }, { 8 } },
	{ { 109 }, { 8 } }, { { 237 }, { 8 } },
	{ {  29 }, { 8 } }, { { 157 }, { 8 } },
	{ {  93 }, { 8 } }, { { 221 }, { 8 } },
	{ {  61 }, { 8 } }, { { 189 }, { 8 } },
	{ { 125 }, { 8 } }, { { 253 }, { 8 } },
	{ {  19 }, { 9 } }, { { 275 }, { 9 } },
	{ { 147 }, { 9 } }, { { 403 }, { 9 } },
	{ {  83 }, { 9 } }, { { 339 }, { 9 } },
	{ { 211 }, { 9 } }, { { 467 }, { 9 } },
	{ {  51 }, { 9 } }, { { 307 }, { 9 } },
	{ { 179 }, { 9 } }, { { 435 }, { 9 } },
	{ { 115 }, { 9 } }, { { 371 }, { 9 } },
	{ { 243 }, { 9 } }, { { 499 }, { 9 } },
	{ {  11 }, { 9 } }, { { 267 }, { 9 } },
	{ { 139 }, { 9 } }, { { 395 }, { 9 } },
	{ {  75 }, { 9 } }, { { 331 }, { 9 } },
	{ { 203 }, { 9 } }, { { 459 }, { 9 } },
	{ {  43 }, { 9 } }, { { 299 }, { 9 } },
	{ { 171 }, { 9 } }, { { 427 }, { 9 } },
	{ { 107 }, { 9 } }, { { 363 }, { 9 } },
	{ { 235 }, { 9 } }, { { 491 }, { 9 } },
	{ {  27 }, { 9 } }, { { 283 }, { 9 } },
	{ { 155 }, { 9 } }, { { 411 }, { 9 } },
	{ {  91 }, { 9 } }, { { 347 }, { 9 } },
	{ { 219 }, { 9 } }, { { 475 }, { 9 } },
	{ {  59 }, { 9 } }, { { 315 }, { 9 } },
	{ { 187 }, { 9 } }, { { 443 }, { 9 } },
	{ { 123 }, { 9 } }, { { 379 }, { 9 } },
	{ { 251 }, { 9 } }, { { 507 }, { 9 } },
	{ {   7 }, { 9 } }, { { 263 }, { 9 } },
	{ { 135 }, { 9 } }, { { 391 }, { 9 } },
	{ {  71 }, { 9 } }, { { 327 }, { 9 } },
	{ { 199 }, { 9 } }, { { 455 }, { 9 } },
	{ {  39 }, { 9 } }, { { 295 }, { 9 } },
	{ { 167 }, { 9 } }, { { 423 }, { 9 } },
	{ { 103 }, { 9 } }, { { 359 }, { 9 } },
	{ { 231 }, { 9 } }, { { 487 }, { 9 } },
	{ {  23 }, { 9 } }, { { 279 }, { 9 } },
	{ { 151 }, { 9 } }, { { 407 }, { 9 } },
	{ {  87 }, { 9 } }, { { 343 }, { 9 } },
	{ { 215 }, { 9 } }, { { 471 }, { 9 } },
	{ {  55 }, { 9 } }, { { 311 }, { 9 } },
	{ { 183 }, { 9 } }, { { 439 }, { 9 } },
	{ { 119 }, { 9 } }, { { 375 }, { 9 } },
	{ { 247 }, { 9 } }, { { 503 }, { 9 } },
	{ {  15 }, { 9 } }, { { 271 }, { 9 } },
	{ { 143 }, { 9 } }, { { 399 }, { 9 } },
	{ {  79 }, { 9 } }, { { 335 }, { 9 } },
	{ { 207 }, { 9 } }, { { 463 }, { 9 } },
	{ {  47 }, { 9 } }, { { 303 }, { 9 } },
	{ { 175 }, { 9 } }, { { 431 }, { 9 } },
	{ { 111 }, { 9 } }, { { 367 }, { 9 } },
	{ { 239 }, { 9 } }, { { 495 }, { 9 } },
	{ {  31 }, { 9 } }, { { 287 }, { 9 } },
	{ { 159 }, { 9 } }, { { 415 }, { 9 } },
	{ {  95 }, { 9 } }, { { 351 }, { 9 } },
	{ { 223 }, { 9 } }, { { 479 }, { 9 } },
	{ {  63 }, { 9 } }, { { 319 }, { 9 } },
	{ { 191 }, { 9 } }, { { 447 }, { 9 } },
	{ { 127 }, { 9 } }, { { 383 }, { 9 } },
	{ { 255 }, { 9 } }, { { 511 }, { 9 } },
	{ {   0 }, { 7 } }, { {  64 }, { 7 } },
	{ {  32 }, { 7 } }, { {  96 }, { 7 } },
	{ {  16 }, { 7 } }, { {  80 }, { 7 } },
	{ {  48 }, { 7 } }, { { 112 }, { 7 } },
	{ {   8 }, { 7 } }, { {  72 }, { 7 } },
	{ {  40 }, { 7 } }, { { 104 }, { 7 } },
	{ {  24 }, { 7 } }, { {  88 }, { 7 } },
	{ {  56 }, { 7 } }, { { 120 }, { 7 } },
	{ {   4 }, { 7 } }, { {  68 }, { 7 } },
	{ {  36 }, { 7 } }, { { 100 }, { 7 } },
	{ {  20 }, { 7 } }, { {  84 }, { 7 } },
	{ {  52 }, { 7 } }, { { 116 }, { 7 } },
	{ {   3 }, { 8 } }, { { 131 }, { 8 } },
	{ {  67 }, { 8 } }, { { 195 }, { 8 } },
	{ {  35 }, { 8 } }, { { 163 }, { 8 } },
	{ {  99 }, { 8 } }, { { 227 }, { 8 } }
	};

static const struct flate_ctdata static_dtree[FLATE_D_CODES] = {
	{ {  0 }, { 5 } }, { { 16 }, { 5 } },
	{ {  8 }, { 5 } }, { { 24 }, { 5 } },
	{ {  4 }, { 5 } }, { { 20 }, { 5 } },
	{ { 12 }, { 5 } }, { { 28 }, { 5 } },
	{ {  2 }, { 5 } }, { { 18 }, { 5 } },
	{ { 10 }, { 5 } }, { { 26 }, { 5 } },
	{ {  6 }, { 5 } }, { { 22 }, { 5 } },
	{ { 14 }, { 5 } }, { { 30 }, { 5 } },
	{ {  1 }, { 5 } }, { { 17 }, { 5 } },
	{ {  9 }, { 5 } }, { { 25 }, { 5 } },
	{ {  5 }, { 5 } }, { { 21 }, { 5 } },
	{ { 13 }, { 5 } }, { { 29 }, { 5 } },
	{ {  3 }, { 5 } }, { { 19 }, { 5 } },
	{ { 11 }, { 5 } }, { { 27 }, { 5 } },
	{ {  7 }, { 5 } }, { { 23 }, { 5 } }
	};

static const struct flate_stree_desc static_l_desc = {
	static_ltree, extra_lbits, FLATE_LITERALS + 1,
	FLATE_L_CODES, FLATE_BITS_MAX
	};

static const struct flate_stree_desc static_d_desc = {
	static_dtree, extra_dbits, 0,
	FLATE_D_CODES, FLATE_BITS_MAX
	};

static const struct flate_stree_desc static_bl_desc = {
	NULL, extra_blbits, 0,
	FLATE_BL_CODES, FLATE_BL_BITS_MAX
	};

static const struct flate_config config_table[10] = {
	{ 0,    0,   0,    0, flate_stored }, /* level:0 store only */
	{ 4,    4,   8,    4, flate_fast   }, /* level:1 max speed, no lazy matches */
	{ 4,    5,  16,    8, flate_fast   }, /* level:2 */
	{ 4,    6,  32,   32, flate_fast   }, /* level:3 */
	{ 4,    4,  16,   16, flate_slow   }, /* level:4 lazy matches */
	{ 8,   16,  32,   32, flate_slow   }, /* level:5 */
	{ 8,   16, 128,  128, flate_slow   }, /* level:6 */
	{ 8,   32, 128,  256, flate_slow   }, /* level:7 */
	{ 32, 128, 258, 1024, flate_slow   }, /* level:8 */
	{ 32, 258, 258, 4096, flate_slow   }  /* level:9 */
	};
/* end */

struct flate_ctx {
	struct flate_tree_desc l_desc;
	struct flate_tree_desc d_desc;
	struct flate_tree_desc bl_desc;
	struct flate_ctdata dyn_ltree[FLATE_HEAP_SIZE];
	struct flate_ctdata dyn_dtree[FLATE_D_HEAP_SIZE];
	struct flate_ctdata dyn_bltree[FLATE_BL_HEAP_SIZE];
	uint16 bl_count[FLATE_BITS_MAX];
	int32 heap[FLATE_HEAP_SIZE];
};

void FSYMBOL(flate_init)(struct flate_ctx *ctx) {
	ctx->l_desc.dyn_tree = ctx->dyn_ltree;
	ctx->l_desc.static_tree = &static_l_desc;
	ctx->l_desc.max_code = 0;

	ctx->d_desc.dyn_tree = ctx->dyn_dtree;
	ctx->d_desc.static_tree = &static_d_desc;
	ctx->d_desc.max_code = 0;

	ctx->bl_desc.dyn_tree = ctx->dyn_bltree;
	ctx->bl_desc.static_tree = &static_bl_desc;
	ctx->bl_desc.max_code = 0;
};

void FSYMBOL(flate_block_init)(struct flate_ctx *ctx) {
	for (int32 i = 0; i < FLATE_L_CODES; i++)
		ctx->dyn_ltree[i].freq = 0;
	for (int32 i = 0; i < FLATE_D_CODES; i++)
		ctx->dyn_dtree[i].freq = 0;
	for (int32 i = 0; i < FLATE_BL_CODES; i++)
		ctx->dyn_bltree[i].freq = 0;
	ctx->dyn_ltree[FLATE_END_BLOCK].freq = 1;

	ctx->opt_len = ctx->static_len = 0;
}

uint16 FSYMBOL(flate_bit_reverse)(uint16 c, uint16 len) {
	uint16 r = 0;
	while (len--) {
		r |= c & 1;
		r <<= 1;
		c >>= 1;
	}

	return r >> 1;
}

void FSYMBOL(flate_gen_codes)(struct flate_ctdata *tree,
		const uint16 *bl_count, int32 n) {
	uint16 next_code[16];
	uint16 code = 0;

	for (int32 i = 1; i <= FLATE_BITS_MAX; i++) {
		code = (code + bl_count[i - 1]) << 1;
		next_code[i] = code;
	}

	for (int32 i = 0; i <= n; i++) {
		uint16 len = tree[i].dl.len;
		if (!len)
			continue;
		tree[i].fc.code = FSYMBOL(flate_bit_reverse)(
			next_code[len]++, len);
	}
}

static void _pqdownheap(uint8 *depth, int32 *heap, int32 n,
		struct flate_ctdata *tree, int32 k) {
	int32 v = heap[k];

#undef SMALLER
#define SMALLER(depth, tree, n, m) \
	(tree[n].freq < tree[m].freq \
	|| (tree[n].freq == tree[m].freq && depth[n] <= depth[m]))

	for (int32 j = 0; (k << 1) <= n; ) {
		j = k << 1;
		if (j < n && SMALLER(depth, tree, heap[j + 1], heap[j]))
			j++;
		if (SMALLER(depth, tree, v, heap[j]))
			break;
		heap[k] = heap[j];
		k = j;
	}

	heap[k] = v;
}

void FSYMBOL(flate_build_tree)(uint8 *depth, int32 *heap, uint32 n,
		struct flate_tree_desc *desc) {
	struct flate_ctdata *tree = desc->dyn_tree;
	const struct flate_ctdata *stree = desc->stree->static_tree;
	int32 elems = ctx->elems;
	int32 max_code = -1;
	int32 heap_len = 0;
	int32 node = 0;

	/* NOTE: heap index starts at 1 */

	for (int32 i = 0; i < elems; i++) {
		if (tree[i].freq) {
			heap[++heap_len] = max_code = i;
			depth[i] = 0;
		} else {
			tree[i].len = 0;
		}
	}

	while (heap_len < 2) {
		node = heap[++heap_len] = (max_code < 2 ? ++max_code : 0);
		tree[node].freq = 1;
		depth[node] = 0;
		opt_len--;
		if (stree)
			static_len -= stree[node].len;
	}
	desc->max_code = max_code;

	for (int32 k = heap_len / 2; k >= 1; k--)
		_pqdownheap(depth, heap, heap_len, tree, k);

#undef MAX
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

	uint32 j, k;
	do {
		j = heap[1];
		heap[1] = heap[heap_len--];
		_pqdownheap(depth, heap, heap_len, tree, 1);
		k = heap[1];

		heap[--heap_max] = j;
		heap[--heap_max] = k;

		tree[node].freq = tree[j].freq + tree[k].freq;
		depth[node] = MAX(depth[j], depth[k]) + 1;
		tree[j].dad = tree[k].dad = node;

		heap[1] = node++;
		_pqdownheap(depth, heap, heap_len, tree, 1);
	} while (heap_len >= 2);

	heap[--heap_max] = heap[1];
}


local void gen_bitlen(deflate_state *s, tree_desc *desc) {
	ct_data *tree        = desc->dyn_tree;
	int max_code         = desc->max_code;
	const ct_data *stree = desc->stat_desc->static_tree;
	const intf *extra    = desc->stat_desc->extra_bits;
	int base             = desc->stat_desc->extra_base;
	int max_length       = desc->stat_desc->max_length;
	int h;              /* heap index */
	int n, m;           /* iterate over the tree elements */
	int bits;           /* bit length */
	int xbits;          /* extra bits */
	ush f;              /* frequency */
	int overflow = 0;   /* number of elements with bit length too large */

	for (int32 i = 0; i <= FLATE_BITS_MAX; i++)
		s->bl_count[i] = 0;

	tree[s->heap[s->heap_max]].len = 0;

	for (h = s->heap_max + 1; h < FLATE_HEAP_SIZE; h++) {
		n = s->heap[h];
		bits = tree[tree[n].dad].len + 1;
		if (bits > max_length)
			bits = max_length, overflow++;
		tree[n].Len = (ush)bits;
		if (n > max_code)
			continue;
		s->bl_count[bits]++;
		xbits = 0;
		if (n >= base)
			xbits = extra[n - base];
		f = tree[n].Freq;
		s->opt_len += (ulg)f * (unsigned)(bits + xbits);
		if (stree)
			s->static_len += (ulg)f * (unsigned)(stree[n].Len + xbits);
	}
	if (overflow == 0)
		return;

	do {
		bits = max_length - 1;
		while (s->bl_count[bits] == 0) bits--;
		s->bl_count[bits]--;
		s->bl_count[bits + 1] += 2;
		s->bl_count[max_length]--;
		overflow -= 2;
	} while (overflow > 0);

	for (bits = max_length; bits != 0; bits--) {
		n = s->bl_count[bits];
		while (n != 0) {
			m = s->heap[--h];
			if (m > max_code) continue;
			if ((unsigned) tree[m].Len != (unsigned) bits) {
				s->opt_len += ((ulg)bits - tree[m].Len) * tree[m].Freq;
				tree[m].Len = (ush)bits;
			}
			n--;
		}
	}
}

#define UPDATE_HASH(s,h,c) (h = (((h)<<s->hash_shift) ^ (c)) & s->hash_mask)

#define INSERT_STRING(s, str, match_head) \
	(UPDATE_HASH(s, s->ins_h, s->window[(str) + (MIN_MATCH-1)]), \
	s->prev[(str) & s->w_mask] = match_head = s->head[s->ins_h], \
	s->head[s->ins_h] = (Pos)(str))

static uInt longest_match(deflate_state *s, IPos cur_match) {
	unsigned chain_length = s->max_chain_length;/* max hash chain length */
	register Byte *scan = s->window + s->strstart; /* current string */
	register Byte *match;                       /* matched string */
	register int len;                           /* length of current match */
	int best_len = s->prev_length;              /* best match length so far */
	int nice_match = s->nice_match;             /* stop if match long enough */
	IPos limit = s->strstart > (IPos)MAX_DIST(s) ?
		s->strstart - (IPos)MAX_DIST(s) : NIL;
	Pos *prev = s->prev;
	uInt wmask = s->w_mask;

	register Byte *strend = s->window + s->strstart + MAX_MATCH;
	register Byte scan_end1  = scan[best_len-1];
	register Byte scan_end   = scan[best_len];

	if (s->prev_length >= s->good_match)
		chain_length >>= 2;
	if ((uInt)nice_match > s->lookahead)
		nice_match = s->lookahead;

	do {
		match = s->window + cur_match;
		if (match[best_len] != scan_end
				|| match[best_len-1] != scan_end1
				|| *match != *scan
				|| *++match != scan[1])
			continue;
		scan += 2;
		match++;

		do {
		} while (*++scan == *++match && *++scan == *++match &&
				*++scan == *++match && *++scan == *++match &&
				*++scan == *++match && *++scan == *++match &&
				*++scan == *++match && *++scan == *++match &&
				scan < strend);

		len = MAX_MATCH - (int)(strend - scan);
		scan = strend - MAX_MATCH;
		if (len > best_len) {
			s->match_start = cur_match;
			best_len = len;
			if (len >= nice_match)
				break;
			scan_end1 = scan[best_len-1];
			scan_end = scan[best_len];
		}
	} while ((cur_match = prev[cur_match & wmask]) > limit
			&& --chain_length != 0);

	if ((uInt)best_len <= s->lookahead)
		return best_len;

	return s->lookahead;
}


static void fill_window(deflate_state *s) {
	register unsigned n, m;
	register Pos *p;
	unsigned more;
	uInt wsize = s->w_size;

	do {
		more = (unsigned)(s->window_size -(ulg)s->lookahead -(ulg)s->strstart);
		if (more == 0 && s->strstart == 0 && s->lookahead == 0) {
			more = wsize;
		} else if (more == (unsigned)(-1)) {
			more--;
		} else if (s->strstart >= wsize+MAX_DIST(s)) {
			memcpy((char *)s->window, (char *)s->window+wsize,
					(unsigned)wsize);
			s->match_start -= wsize;
			s->strstart    -= wsize;
			s->block_start -= (long) wsize;

			n = s->hash_size;
			p = &s->head[n];
			do {
				m = *--p;
				*p = (Pos)(m >= wsize ? m-wsize : NIL);
			} while (--n);

			n = wsize;
			p = &s->prev[n];
			do {
				m = *--p;
				*p = (Pos)(m >= wsize ? m-wsize : NIL);
			} while (--n);
			more += wsize;
		}
		if (s->strm->avail_in == 0)
			return;

		n = read_buf(s->strm, s->window + s->strstart + s->lookahead, more);
		s->lookahead += n;

		if (s->lookahead >= MIN_MATCH) {
			s->ins_h = s->window[s->strstart];
			UPDATE_HASH(s, s->ins_h, s->window[s->strstart+1]);
		}
	} while (s->lookahead < MIN_LOOKAHEAD && s->strm->avail_in != 0);
}



#define FLUSH_BLOCK_ONLY(s, eof) { \
	zlib_tr_flush_block(s, (s->block_start >= 0L ? \
		(char *)&s->window[(unsigned)s->block_start] : \
		NULL), \
		(ulg)((long)s->strstart - s->block_start), \
		(eof)); \
	s->block_start = s->strstart; \
	flush_pending(s->strm); \
	Tracev((stderr,"[FLUSH]")); \
}

#define FLUSH_BLOCK(s, eof) { \
	FLUSH_BLOCK_ONLY(s, eof); \
	if (s->strm->avail_out == 0) return (eof) ? finish_started : need_more; \
	}

static block_state deflate_fast(deflate_state *s, int flush) {
	IPos hash_head = NIL; /* head of the hash chain */
	int bflush;           /* set if current block must be flushed */

	for (;;) {
		if (s->lookahead < MIN_LOOKAHEAD) {
			fill_window(s);
			if (s->lookahead < MIN_LOOKAHEAD && flush == Z_NO_FLUSH) {
				return need_more;
			}
			if (s->lookahead == 0) break;
		}
		if (s->lookahead >= MIN_MATCH) {
			INSERT_STRING(s, s->strstart, hash_head);
		}

		if (hash_head != NIL && s->strstart - hash_head <= MAX_DIST(s)) {
			if (s->strategy != Z_HUFFMAN_ONLY) {
				s->match_length = longest_match (s, hash_head);
			}
		}
		if (s->match_length >= MIN_MATCH) {
			check_match(s, s->strstart, s->match_start, s->match_length);

			bflush = zlib_tr_tally(s, s->strstart - s->match_start,
					s->match_length - MIN_MATCH);

			s->lookahead -= s->match_length;

			if (s->match_length <= s->max_insert_length &&
					s->lookahead >= MIN_MATCH) {
				s->match_length--;
				do {
					s->strstart++;
					INSERT_STRING(s, s->strstart, hash_head);
				} while (--s->match_length != 0);
				s->strstart++; 
			} else {
				s->strstart += s->match_length;
				s->match_length = 0;
				s->ins_h = s->window[s->strstart];
				UPDATE_HASH(s, s->ins_h, s->window[s->strstart+1]);
			}
		} else {
			bflush = zlib_tr_tally (s, 0, s->window[s->strstart]);
			s->lookahead--;
			s->strstart++; 
		}
		if (bflush) FLUSH_BLOCK(s, 0);
	}
	FLUSH_BLOCK(s, flush == Z_FINISH);

	return flush == Z_FINISH ? finish_done : block_done;
}


static block_state deflate_slow(deflate_state *s, int flush) {
	IPos hash_head = NIL;    /* head of hash chain */
	int bflush;              /* set if current block must be flushed */

	for (;;) {
		if (s->lookahead < MIN_LOOKAHEAD) {
			fill_window(s);
			if (s->lookahead < MIN_LOOKAHEAD && flush == Z_NO_FLUSH) {
				return need_more;
			}
			if (s->lookahead == 0) break;
		}
		if (s->lookahead >= MIN_MATCH) {
			INSERT_STRING(s, s->strstart, hash_head);
		}

		s->prev_length = s->match_length, s->prev_match = s->match_start;
		s->match_length = MIN_MATCH-1;

		if (hash_head != NIL && s->prev_length < s->max_lazy_match &&
				s->strstart - hash_head <= MAX_DIST(s)) {
			if (s->strategy != Z_HUFFMAN_ONLY) {
				s->match_length = longest_match (s, hash_head);
			}
			if (s->match_length <= 5 && (s->strategy == Z_FILTERED ||
						(s->match_length == MIN_MATCH &&
						 s->strstart - s->match_start > TOO_FAR))) {
				s->match_length = MIN_MATCH-1;
			}
		}

		if (s->prev_length >= MIN_MATCH && s->match_length <= s->prev_length) {
			uInt max_insert = s->strstart + s->lookahead - MIN_MATCH;
			check_match(s, s->strstart-1, s->prev_match, s->prev_length);
			bflush = zlib_tr_tally(s, s->strstart -1 - s->prev_match,
					s->prev_length - MIN_MATCH);

			s->lookahead -= s->prev_length-1;
			s->prev_length -= 2;
			do {
				if (++s->strstart <= max_insert) {
					INSERT_STRING(s, s->strstart, hash_head);
				}
			} while (--s->prev_length != 0);
			s->match_available = 0;
			s->match_length = MIN_MATCH-1;
			s->strstart++;

			if (bflush)
				FLUSH_BLOCK(s, 0);
		} else if (s->match_available) {
			if (zlib_tr_tally (s, 0, s->window[s->strstart-1])) {
				FLUSH_BLOCK_ONLY(s, 0);
			}
			s->strstart++;
			s->lookahead--;
			if (s->strm->avail_out == 0) return need_more;
		} else {
			s->match_available = 1;
			s->strstart++;
			s->lookahead--;
		}
	}
	if (s->match_available) {
		zlib_tr_tally (s, 0, s->window[s->strstart-1]);
		s->match_available = 0;
	}
	FLUSH_BLOCK(s, flush == Z_FINISH);

	return flush == Z_FINISH ? finish_done : block_done;
}


#define d_code(dist) \
   ((dist) < 256 ? _dist_code[dist] : _dist_code[256+((dist)>>7)])


int main(void) {

(void)extra_lbits;
(void)extra_dbits;
(void)extra_blbits;
(void)bl_order;
(void)lbase;
(void)dbase;
(void)base_length;
(void)base_distance;
(void)length_code;
(void)distance_code;
(void)static_ltree;
(void)static_dtree;

	return 0;
}
