/* deflate.c - deflate compress algorithm implementations */

#include <libf/config.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/al/deflate.h>
#include <libf/ds/bits_add.h>


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

static const uint8 extra_bl_bits[DEFLATE_BL_CODES] = {
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	2, 3, 7
	};

static const uint8 extra_bl_order[DEFLATE_BL_CODES] = {
	16, 17, 18,  0,  8,  7,  9,  6,
	10,  5, 11,  4, 12,  3, 13,  2,
	14,  1, 15
	};

/* extra length bits */
static const uint8 extra_len_bits[DEFLATE_LEN_CODES] = {
	0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 2, 2, 2, 2,
	3, 3, 3, 3, 4, 4, 4, 4,
	5, 5, 5, 5, 0
	};

/* extra distance bits */
static const uint8 extra_dist_bits[DEFLATE_DIST_CODES] = {
	 0,  0,  0,  0,  1,  1,  2,  2,
	 3,  3,  4,  4,  5,  5,  6,  6,
	 7,  7,  8,  8,  9,  9, 10, 10,
	11, 11, 12, 12, 13, 13
	};

/* base length */
static const uint8 base_len[DEFLATE_LEN_CODES] = {
	  0,   1,   2,   3,   4,   5,   6,   7,
	  8,  10,  12,  14,  16,  20,  24,  28,
	 32,  40,  48,  56,  64,  80,  96, 112,
	128, 160, 192, 224,   0
	};

/* base distance */
static const uint16 base_dist[DEFLATE_DIST_CODES] = {
	   0,     1,     2,     3,     4,     6,     8,    12,
	  16,    24,    32,    48,    64,    96,   128,   192,
	 256,   384,   512,   768,  1024,  1536,  2048,  3072,
	4096,  6144,  8192, 12288, 16384, 24576
	};

/* length code table */
static const uint8 len_code[DEFLATE_LEN_SIZE] = {
	 0,  1,  2,  3,  4,  5,  6,  7,  8,  8,  9,  9, 10, 10, 11, 11,
	12, 12, 12, 12, 13, 13, 13, 13, 14, 14, 14, 14, 15, 15, 15, 15,
	16, 16, 16, 16, 16, 16, 16, 16, 17, 17, 17, 17, 17, 17, 17, 17,
	18, 18, 18, 18, 18, 18, 18, 18, 19, 19, 19, 19, 19, 19, 19, 19,
	20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
	21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21,
	22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
	23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23,
	24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
	24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
	25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25,
	25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25,
	26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26,
	26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26,
	27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27,
	27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 28
	};

/* distance code table */
static const uint8 dist_code[DEFLATE_DIST_SIZE] = {
	 0,  1,  2,  3,  4,  4,  5,  5,  6,  6,  6,  6,  7,  7,  7,  7,
	 8,  8,  8,  8,  8,  8,  8,  8,  9,  9,  9,  9,  9,  9,  9,  9,
	10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
	11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
	12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
	12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
	13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
	13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
	14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
	14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
	14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
	14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
	15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
	 0,  0, 16, 17, 18, 18, 19, 19, 20, 20, 20, 20, 21, 21, 21, 21,
	22, 22, 22, 22, 22, 22, 22, 22, 23, 23, 23, 23, 23, 23, 23, 23,
	24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
	25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25,
	26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26,
	26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26,
	27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27,
	27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27,
	28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
	28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
	28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
	28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
	29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29,
	29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29,
	29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29,
	29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29
	};

/* static length tree */
static const struct deflate_ctdata static_ltree[DEFLATE_L_CODES] = {
	{ {  12 }, { 8 } }, { { 140 }, { 8 } }, { {  76 }, { 8 } },
	{ { 204 }, { 8 } }, { {  44 }, { 8 } }, { { 172 }, { 8 } },
	{ { 108 }, { 8 } }, { { 236 }, { 8 } }, { {  28 }, { 8 } },
	{ { 156 }, { 8 } }, { {  92 }, { 8 } }, { { 220 }, { 8 } },
	{ {  60 }, { 8 } }, { { 188 }, { 8 } }, { { 124 }, { 8 } },
	{ { 252 }, { 8 } }, { {   2 }, { 8 } }, { { 130 }, { 8 } },
	{ {  66 }, { 8 } }, { { 194 }, { 8 } }, { {  34 }, { 8 } },
	{ { 162 }, { 8 } }, { {  98 }, { 8 } }, { { 226 }, { 8 } },
	{ {  18 }, { 8 } }, { { 146 }, { 8 } }, { {  82 }, { 8 } },
	{ { 210 }, { 8 } }, { {  50 }, { 8 } }, { { 178 }, { 8 } },
	{ { 114 }, { 8 } }, { { 242 }, { 8 } }, { {  10 }, { 8 } },
	{ { 138 }, { 8 } }, { {  74 }, { 8 } }, { { 202 }, { 8 } },
	{ {  42 }, { 8 } }, { { 170 }, { 8 } }, { { 106 }, { 8 } },
	{ { 234 }, { 8 } }, { {  26 }, { 8 } }, { { 154 }, { 8 } },
	{ {  90 }, { 8 } }, { { 218 }, { 8 } }, { {  58 }, { 8 } },
	{ { 186 }, { 8 } }, { { 122 }, { 8 } }, { { 250 }, { 8 } },
	{ {   6 }, { 8 } }, { { 134 }, { 8 } }, { {  70 }, { 8 } },
	{ { 198 }, { 8 } }, { {  38 }, { 8 } }, { { 166 }, { 8 } },
	{ { 102 }, { 8 } }, { { 230 }, { 8 } }, { {  22 }, { 8 } },
	{ { 150 }, { 8 } }, { {  86 }, { 8 } }, { { 214 }, { 8 } },
	{ {  54 }, { 8 } }, { { 182 }, { 8 } }, { { 118 }, { 8 } },
	{ { 246 }, { 8 } }, { {  14 }, { 8 } }, { { 142 }, { 8 } },
	{ {  78 }, { 8 } }, { { 206 }, { 8 } }, { {  46 }, { 8 } },
	{ { 174 }, { 8 } }, { { 110 }, { 8 } }, { { 238 }, { 8 } },
	{ {  30 }, { 8 } }, { { 158 }, { 8 } }, { {  94 }, { 8 } },
	{ { 222 }, { 8 } }, { {  62 }, { 8 } }, { { 190 }, { 8 } },
	{ { 126 }, { 8 } }, { { 254 }, { 8 } }, { {   1 }, { 8 } },
	{ { 129 }, { 8 } }, { {  65 }, { 8 } }, { { 193 }, { 8 } },
	{ {  33 }, { 8 } }, { { 161 }, { 8 } }, { {  97 }, { 8 } },
	{ { 225 }, { 8 } }, { {  17 }, { 8 } }, { { 145 }, { 8 } },
	{ {  81 }, { 8 } }, { { 209 }, { 8 } }, { {  49 }, { 8 } },
	{ { 177 }, { 8 } }, { { 113 }, { 8 } }, { { 241 }, { 8 } },
	{ {   9 }, { 8 } }, { { 137 }, { 8 } }, { {  73 }, { 8 } },
	{ { 201 }, { 8 } }, { {  41 }, { 8 } }, { { 169 }, { 8 } },
	{ { 105 }, { 8 } }, { { 233 }, { 8 } }, { {  25 }, { 8 } },
	{ { 153 }, { 8 } }, { {  89 }, { 8 } }, { { 217 }, { 8 } },
	{ {  57 }, { 8 } }, { { 185 }, { 8 } }, { { 121 }, { 8 } },
	{ { 249 }, { 8 } }, { {   5 }, { 8 } }, { { 133 }, { 8 } },
	{ {  69 }, { 8 } }, { { 197 }, { 8 } }, { {  37 }, { 8 } },
	{ { 165 }, { 8 } }, { { 101 }, { 8 } }, { { 229 }, { 8 } },
	{ {  21 }, { 8 } }, { { 149 }, { 8 } }, { {  85 }, { 8 } },
	{ { 213 }, { 8 } }, { {  53 }, { 8 } }, { { 181 }, { 8 } },
	{ { 117 }, { 8 } }, { { 245 }, { 8 } }, { {  13 }, { 8 } },
	{ { 141 }, { 8 } }, { {  77 }, { 8 } }, { { 205 }, { 8 } },
	{ {  45 }, { 8 } }, { { 173 }, { 8 } }, { { 109 }, { 8 } },
	{ { 237 }, { 8 } }, { {  29 }, { 8 } }, { { 157 }, { 8 } },
	{ {  93 }, { 8 } }, { { 221 }, { 8 } }, { {  61 }, { 8 } },
	{ { 189 }, { 8 } }, { { 125 }, { 8 } }, { { 253 }, { 8 } },
	{ {  19 }, { 9 } }, { { 275 }, { 9 } }, { { 147 }, { 9 } },
	{ { 403 }, { 9 } }, { {  83 }, { 9 } }, { { 339 }, { 9 } },
	{ { 211 }, { 9 } }, { { 467 }, { 9 } }, { {  51 }, { 9 } },
	{ { 307 }, { 9 } }, { { 179 }, { 9 } }, { { 435 }, { 9 } },
	{ { 115 }, { 9 } }, { { 371 }, { 9 } }, { { 243 }, { 9 } },
	{ { 499 }, { 9 } }, { {  11 }, { 9 } }, { { 267 }, { 9 } },
	{ { 139 }, { 9 } }, { { 395 }, { 9 } }, { {  75 }, { 9 } },
	{ { 331 }, { 9 } }, { { 203 }, { 9 } }, { { 459 }, { 9 } },
	{ {  43 }, { 9 } }, { { 299 }, { 9 } }, { { 171 }, { 9 } },
	{ { 427 }, { 9 } }, { { 107 }, { 9 } }, { { 363 }, { 9 } },
	{ { 235 }, { 9 } }, { { 491 }, { 9 } }, { {  27 }, { 9 } },
	{ { 283 }, { 9 } }, { { 155 }, { 9 } }, { { 411 }, { 9 } },
	{ {  91 }, { 9 } }, { { 347 }, { 9 } }, { { 219 }, { 9 } },
	{ { 475 }, { 9 } }, { {  59 }, { 9 } }, { { 315 }, { 9 } },
	{ { 187 }, { 9 } }, { { 443 }, { 9 } }, { { 123 }, { 9 } },
	{ { 379 }, { 9 } }, { { 251 }, { 9 } }, { { 507 }, { 9 } },
	{ {   7 }, { 9 } }, { { 263 }, { 9 } }, { { 135 }, { 9 } },
	{ { 391 }, { 9 } }, { {  71 }, { 9 } }, { { 327 }, { 9 } },
	{ { 199 }, { 9 } }, { { 455 }, { 9 } }, { {  39 }, { 9 } },
	{ { 295 }, { 9 } }, { { 167 }, { 9 } }, { { 423 }, { 9 } },
	{ { 103 }, { 9 } }, { { 359 }, { 9 } }, { { 231 }, { 9 } },
	{ { 487 }, { 9 } }, { {  23 }, { 9 } }, { { 279 }, { 9 } },
	{ { 151 }, { 9 } }, { { 407 }, { 9 } }, { {  87 }, { 9 } },
	{ { 343 }, { 9 } }, { { 215 }, { 9 } }, { { 471 }, { 9 } },
	{ {  55 }, { 9 } }, { { 311 }, { 9 } }, { { 183 }, { 9 } },
	{ { 439 }, { 9 } }, { { 119 }, { 9 } }, { { 375 }, { 9 } },
	{ { 247 }, { 9 } }, { { 503 }, { 9 } }, { {  15 }, { 9 } },
	{ { 271 }, { 9 } }, { { 143 }, { 9 } }, { { 399 }, { 9 } },
	{ {  79 }, { 9 } }, { { 335 }, { 9 } }, { { 207 }, { 9 } },
	{ { 463 }, { 9 } }, { {  47 }, { 9 } }, { { 303 }, { 9 } },
	{ { 175 }, { 9 } }, { { 431 }, { 9 } }, { { 111 }, { 9 } },
	{ { 367 }, { 9 } }, { { 239 }, { 9 } }, { { 495 }, { 9 } },
	{ {  31 }, { 9 } }, { { 287 }, { 9 } }, { { 159 }, { 9 } },
	{ { 415 }, { 9 } }, { {  95 }, { 9 } }, { { 351 }, { 9 } },
	{ { 223 }, { 9 } }, { { 479 }, { 9 } }, { {  63 }, { 9 } },
	{ { 319 }, { 9 } }, { { 191 }, { 9 } }, { { 447 }, { 9 } },
	{ { 127 }, { 9 } }, { { 383 }, { 9 } }, { { 255 }, { 9 } },
	{ { 511 }, { 9 } }, { {   0 }, { 7 } }, { {  64 }, { 7 } },
	{ {  32 }, { 7 } }, { {  96 }, { 7 } }, { {  16 }, { 7 } },
	{ {  80 }, { 7 } }, { {  48 }, { 7 } }, { { 112 }, { 7 } },
	{ {   8 }, { 7 } }, { {  72 }, { 7 } }, { {  40 }, { 7 } },
	{ { 104 }, { 7 } }, { {  24 }, { 7 } }, { {  88 }, { 7 } },
	{ {  56 }, { 7 } }, { { 120 }, { 7 } }, { {   4 }, { 7 } },
	{ {  68 }, { 7 } }, { {  36 }, { 7 } }, { { 100 }, { 7 } },
	{ {  20 }, { 7 } }, { {  84 }, { 7 } }, { {  52 }, { 7 } },
	{ { 116 }, { 7 } }, { {   3 }, { 8 } }, { { 131 }, { 8 } },
	{ {  67 }, { 8 } }, { { 195 }, { 8 } }, { {  35 }, { 8 } },
	{ { 163 }, { 8 } }, { {  99 }, { 8 } }, { { 227 }, { 8 } }
	};

/* static distance tree */
static const struct deflate_ctdata static_dtree[DEFLATE_D_CODES] = {
	{ {  0 }, { 5 } }, { { 16 }, { 5 } }, { {  8 }, { 5 } },
	{ { 24 }, { 5 } }, { {  4 }, { 5 } }, { { 20 }, { 5 } },
	{ { 12 }, { 5 } }, { { 28 }, { 5 } }, { {  2 }, { 5 } },
	{ { 18 }, { 5 } }, { { 10 }, { 5 } }, { { 26 }, { 5 } },
	{ {  6 }, { 5 } }, { { 22 }, { 5 } }, { { 14 }, { 5 } },
	{ { 30 }, { 5 } }, { {  1 }, { 5 } }, { { 17 }, { 5 } },
	{ {  9 }, { 5 } }, { { 25 }, { 5 } }, { {  5 }, { 5 } },
	{ { 21 }, { 5 } }, { { 13 }, { 5 } }, { { 29 }, { 5 } },
	{ {  3 }, { 5 } }, { { 19 }, { 5 } }, { { 11 }, { 5 } },
	{ { 27 }, { 5 } }, { {  7 }, { 5 } }, { { 23 }, { 5 } }
	};
/* end */

/* @func: _bit_reverse (static) - bit reverse conversion
* @param1: uint16 # input
* @param2: uint16 # length
* @return: uint16 # reverse input
*/
static uint16 _bit_reverse(uint16 c, uint16 len) {
	/* 10001010 -> 01010001 */
	uint16 r = 0;
	while (len--) {
		r |= c & 1;
		r <<= 1;
		c >>= 1;
	}

	return r >> 1;
} /* end */

/* @func: _gen_codes (static) - generate codes based on length
* @param1: struct deflate_ctdata * # deflate tree
* @param2: const uint16 *        # bit-length count
* @param3: int32                 # codes number
* @return: void
*/
static void _gen_codes(struct deflate_ctdata *tree,
		const uint16 *bitlen_count, int32 n) {
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
} /* end */

/* @func: _gen_bitlen (static) - generate bit-length based on dyntree
* @param1: struct deflate_dyntree_ctx *  # dyntree struct context
* @param2: struct deflate_dyntree_desc * # dyntree description
* @return: void
*/
static void _gen_bitlen(struct deflate_dyntree_ctx *ctx,
		struct deflate_dyntree_desc *desc) {
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
		if (n > code_max) /* parent node */
			continue;

		/* leaf node */
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
} /* end */

/* @func: _heapdown (static) - minheap down shift
* @param1: struct deflate_dyntree_ctx * # dyntree struct context
* @param2: struct deflate_ctdata *      # dyntree ctdata
* @param3: int32                        # parent node
* @return: void
*/
static void _heapdown(struct deflate_dyntree_ctx *ctx,
		struct deflate_ctdata *tree, int32 k) {
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
} /* end */

/* @func: _build_tree (static) - build dynamic huffman tree
* @param1: struct deflate_dyntree_ctx *  # dyntree struct context
* @param2: struct deflate_dyntree_desc * # dyntree description
* @return: void
*/
static void _build_tree(struct deflate_dyntree_ctx *ctx,
		struct deflate_dyntree_desc *desc) {
	const struct deflate_ctdata *stree = desc->stree;
	struct deflate_ctdata *tree = desc->tree;
	int32 elems = desc->elems, code_max = -1;
	int32 n, m, node;

	ctx->size = 0;
	ctx->size_max = DEFLATE_HEAP_SIZE;
	desc->opt_slen = 0;
	desc->opt_dlen = 0;

	/* non-zero freq add to the heap */
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
		_heapdown(ctx, tree, k);

	/*
	* canonical Huffman coding, generating a coding
	* tree based on the codes length.
	*
	* freq:
	*   'A'(65): 2
	*   'B'(66): 2
	*   'C'(67): 7
	*   'D'(68): 4
	*
	* heap-back: 290 289 67 288 68 66 65
	*      290
	*      / \
	*    67  289
	*        / \
	*      68   288
	*           / \
	*         66   65
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

#undef MAX
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

	node = elems;
	do {
		/* min nodes */
		n = ctx->heap[1];
		ctx->heap[1] = ctx->heap[ctx->size--];
		_heapdown(ctx, tree, 1);
		m = ctx->heap[1];

		/* heap-back */
		ctx->heap[--(ctx->size_max)] = n;
		ctx->heap[--(ctx->size_max)] = m;

		/* parent node */
		tree[node].fc.freq = tree[n].fc.freq + tree[m].fc.freq;
		tree[n].dl.dad = tree[m].dl.dad = node;
		ctx->depth[node] = MAX(ctx->depth[n], ctx->depth[m]) + 1;

		ctx->heap[1] = node++;
		_heapdown(ctx, tree, 1);
	} while (ctx->size > 1);
	ctx->heap[--(ctx->size_max)] = ctx->heap[1]; /* root */

	for (int32 i = 0; i <= DEFLATE_BITS_MAX; i++)
		ctx->bitlen_count[i] = 0;

	_gen_bitlen(ctx, desc);

	/* statistical the static and dynamic size */
	for (int32 i = 0; i < elems; i++) {
		int32 f = tree[i].fc.freq;
		if (f) {
			if (stree)
				desc->opt_slen += f * stree[i].dl.len;
			desc->opt_dlen += f * tree[i].dl.len;
		}
	}

	_gen_codes(tree, ctx->bitlen_count, desc->code_max + 1);
} /* end */

#if 0
void _scan_tree(struct deflate_ctdata *bl_tree, struct deflate_ctdata *tree,
		int32 n) {
	int32 prevlen = -1;          /* last emitted length */
	int32 curlen = 0;                /* length of current code */
	int32 nextlen = tree[0].dl.len; /* length of next code */
	int32 count = 0;             /* repeat count of the current code */
	int32 count_min = 4;
	int32 count_max = 7;

	if (!nextlen) {
		count_min = 3;
		count_max = 138;
	}
	tree[n].dl.len = 0xffff;

	for (int32 i = 0; i < n; i++) {
		curlen = nextlen;
		nextlen = tree[i  + 1].dl.len;
		if (++count < count_max && curlen == nextlen) {
			continue;
		} else if (count < count_min) {
			bl_tree[curlen].fc.freq += count;
		} else if (curlen) {
			if (curlen != prevlen)
				bl_tree[curlen].fc.freq++;
			bl_tree[DEFLATE_REP_3_6].fc.freq++;
		} else if (count < 11) {
			bl_tree[DEFLATE_REPZ_3_10].fc.freq++;
		} else {
			bl_tree[DEFLATE_REPZ_11_138].fc.freq++;
		}

		count = 0;
		prevlen = curlen;
		if (nextlen == 0) {
			count_min = 3;
			count_max = 138;
		} else if (curlen == nextlen) {
			count_min = 3;
			count_max = 6;
		} else {
			count_min = 4;
			count_max = 7;
		}
	}
}
#endif



#undef deflate_lz77_tok
struct deflate_lz77_tok {
	uint16 ll;   /* literal/length */
	uint16 dist; /* distance */
};

#undef deflate_lz77_ctx
struct deflate_lz77_ctx {
	uint8 dict[DEFLATE_DICT_SIZE]; /* dict buffer */
	struct deflate_lz77_tok *tok; /* token */
	int32 tok_size; /* token number */
	int32 dict_len; /* dict buffer length */
	int32 lazy_max; /* lazy match max */
	int32 nice_len; /* nice match length */
};

#undef deflate_ctx
struct deflate_ctx {
	struct deflate_ctdata ltree[DEFLATE_L_SIZE];
	struct deflate_ctdata dtree[DEFLATE_D_SIZE];
	struct deflate_dyntree_ctx dyntree_ctx;
	struct deflate_dyntree_desc ltree_desc;
	struct deflate_dyntree_desc dtree_desc;

	struct deflate_lz77_tok lz77_tok[DEFLATE_DIST_SIZE];
	struct deflate_lz77_ctx lz77_ctx;

	uint8 bits_buf[DEFLATE_BITS_BUFSIZE];
	uint32 bits_len;
	struct bits_add_ctx bits_ctx;
};

/* @func: _lz77_match (static) - lz77 dictionary buffer matching
* @param1: struct deflate_lz77_ctx * # lz77 struct context
* @param2: oonst uint8 *             # lookahead buffer
* @param3: int32                     # lookahead pos
* @param4: int32                     # lookahead length
* @param5: int32 *                   # distance
* @return: int32                     # length
*/
static int32 _lz77_match(struct deflate_lz77_ctx *ctx,
		const uint8 *look, int32 look_pos, int32 look_len,
		int32 *best_dist) {
	int32 best_len = 0;
	int32 len_max = look_len - look_pos;

	if (len_max > DEFLATE_MATCH_MAX)
		len_max = DEFLATE_MATCH_MAX;
	if (len_max < DEFLATE_MATCH_MIN)
		return 0;

	uint8 *dict = ctx->dict;
	int32 n = ctx->dict_len;
	for (int32 i = 0; i < n; i++) {
		int32 len = 0;
		while (len < len_max
				&& dict[i + len] == look[look_pos + len])
			len++;

		if (len > best_len) {
			best_len = len;
			*best_dist = n - i;
			if (best_len >= ctx->nice_len) /* nice */
				break;
		}
	}

	return (best_len < DEFLATE_MATCH_MIN) ? 0 : best_len;
} /* end */

/* @func: _lz77_enc (static) - lz77 compress encode
* @param1: struct deflate_lz77_ctx * # lz77 struct context
* @param2: const uint8 *             # input buffer
* @param3: int32                    # input length (max: DEFLATE_DICT_SIZE)
* @return: void
*/
static void _lz77_enc(struct deflate_lz77_ctx *ctx, const uint8 *s, int32 len) {
	struct deflate_lz77_tok *tok = ctx->tok;
	int32 pos = 0, out = 0;
	int32 best_len, best_dist, next_len, next_dist;

	ctx->dict_len = 0;
	while (pos < len) {
		if ((len - pos) > DEFLATE_MATCH_MIN) {
			best_len = _lz77_match(ctx, s, pos, len, &best_dist);

			if (best_len < ctx->lazy_max && (pos + 1) < len) {
				next_len = _lz77_match(ctx,
					s, pos + 1, len, &next_dist);

				if (next_len > best_len) {
					best_len = 0;
				} else if (next_len == best_len
						&& next_dist < best_dist) {
					best_len = 0;
				} else if (best_len == DEFLATE_MATCH_MIN
						&& next_len >= DEFLATE_MATCH_MIN) {
					best_len = 0;
				}
			}
		} else {
			best_len = 0;
		}

		if (best_len > DEFLATE_MATCH_MIN) {
			tok[out].ll = best_len;
			tok[out].dist = best_dist;
			out++;

			XSYMBOL(memcpy)(ctx->dict + ctx->dict_len,
				s + pos, best_len);
			ctx->dict_len += best_len;
			pos += best_len;
		} else {
			ctx->tok[out].ll = s[pos];
			ctx->tok[out].dist = 0;
			out++;

			ctx->dict[ctx->dict_len++] = s[pos++];
		}
	}
	ctx->tok_size = out;
} /* end */




/* @func: _send_bits (static) - send to the bits buffer
* @param1: struct deflate_ctx * # deflate struct context
* @param2: uint32               # input bits
* @param3: uint32               # bits length
* @return: void
*/
static void _send_bits(struct deflate_ctx *ctx, uint32 v, uint32 len) {
	if (FSYMBOL(bits_add)(&ctx->bits_ctx, v, len)) {
		XSYMBOL(memcpy)(ctx->bits_buf + ctx->bits_len,
			BITS_ADD_BUF(&ctx->bits_ctx), BITS_ADD_BUFSIZE);
		ctx->bits_len += BITS_ADD_BUFSIZE;
		BITS_ADD_FLUSH(&ctx->bits_ctx);

		if (BITS_ADD_BREM(&ctx->bits_ctx))
			FSYMBOL(bits_add)(&ctx->bits_ctx, v, len);
	}
} /* end */

/* @func: _send_bits_finish (static) - processing the bits buffer and end
* @param1: struct deflate_ctx * # deflate struct context
* @return: void
*/
static void _send_bits_finish(struct deflate_ctx *ctx) {
	uint32 n = BITS_ADD_GETBITS(&ctx->bits_ctx);
	if (n) {
		n = BITS_ADD_BTASIZE(n);
		XSYMBOL(memcpy)(ctx->bits_buf + ctx->bits_len,
			BITS_ADD_BUF(&ctx->bits_ctx), n);
		ctx->bits_len += n;
	}
} /* end */

/* @func: _compress_block_init (static) - compress block init
* @param1: struct deflate_ctx * # deflate struct context
* @return: void
*/
static void _compress_block_init(struct deflate_ctx *ctx) {
	for (int32 i = 0; i < DEFLATE_L_CODES; i++)
		ctx->ltree[i].fc.freq = 0;
	for (int32 i = 0; i < DEFLATE_D_CODES; i++)
		ctx->dtree[i].fc.freq = 0;
	ctx->ltree[DEFLATE_END_BLOCK].fc.freq = 1;

	ctx->bits_len = 0;
	BITS_ADD_INIT(&ctx->bits_ctx);
} /* end */

/* @func: _compress_block (static) - compress data block
* @param1: struct deflate_ctx *          # deflate struct context
* @param2: const struct deflate_ctdata * # literal/length tree
* @param3: const struct deflate_ctdata * # distance tree
* @return: void
*/
static void _compress_block(struct deflate_ctx *ctx,
		const struct deflate_ctdata *ltree,
		const struct deflate_ctdata *dtree) {
	struct deflate_lz77_tok *tok = ctx->lz77_ctx.tok;
	int32 n = ctx->lz77_ctx.tok_size;
	int32 len, dist, code, extra;

	for (int32 i = 0; i < n; i++) {
		if (tok[i].dist) {
			len = tok[i].ll - DEFLATE_MATCH_MIN;
			dist = tok[i].dist - 1;

			/* length */
			code = DEFLATE_LITERALS + 1 + len_code[len];
			_send_bits(ctx, ltree[code].fc.code,
				ltree[code].dl.len);

			/* length extra */
			code = len_code[len];
			extra = extra_len_bits[code];
			if (extra)
				_send_bits(ctx, len - base_len[code], extra);

			/* distance */
			code = (dist < 256) ? dist_code[dist]
				: dist_code[256 + ((dist) >> 7)];
			_send_bits(ctx, dtree[code].fc.code,
				dtree[code].dl.len);

			/* distance extra */
			extra = extra_dist_bits[code];
			if (extra)
				_send_bits(ctx, dist - base_dist[code], extra);
		} else {
			/* literal */
			code = tok[i].ll;
			_send_bits(ctx, ltree[code].fc.code,
				ltree[code].dl.len);
		}
	}

	/* end block */
	_send_bits(ctx, ltree[DEFLATE_END_BLOCK].fc.code,
		ltree[DEFLATE_END_BLOCK].dl.len);
}






/* @func: deflate_init -
* @param1: struct deflate_ctx * #
* @return: void
*/
void FSYMBOL(deflate_init)(struct deflate_ctx *ctx) {
	/* literal/length */
	ctx->ltree_desc.stree = static_ltree;
	ctx->ltree_desc.tree = ctx->ltree;
	ctx->ltree_desc.elems = DEFLATE_L_CODES;
	ctx->ltree_desc.bitlen_max = DEFLATE_BITS_MAX;

	/* distance */
	ctx->ltree_desc.stree = static_ltree;
	ctx->ltree_desc.tree = ctx->dtree;
	ctx->dtree_desc.elems = DEFLATE_D_CODES;
	ctx->dtree_desc.bitlen_max = 5;

	/* lz77 */
	ctx->lz77_ctx.tok = ctx->lz77_tok;
	ctx->lz77_ctx.lazy_max = 128;
	ctx->lz77_ctx.nice_len = 128;
} /* end */

void FSYMBOL(deflate_process)(struct deflate_ctx *ctx, const uint8 *s, uint32 len) {
	_lz77_enc(&ctx->lz77_ctx, s, len);
	_compress_block_init(ctx);
	_send_bits(ctx, 0x78, 8);
	_send_bits(ctx, 0x01, 8);
	_send_bits(ctx, 0x01, 1);
	_send_bits(ctx, 0x01, 2);
	_compress_block(ctx, static_ltree, static_dtree);
	_send_bits_finish(ctx);
}

#include <stdio.h>
#include <stdlib.h>

#undef BASE
#define BASE 65521
uint32 adler32(uint32 adler, const uint8 *data, uint32 len) {
	uint32 s1 = adler & 0xffff;
	uint32 s2 = (adler >> 16) & 0xffff;

	for (uint32 i = 0; i < len; i++) {
		s1 = (s1 + data[i]) % BASE;
		s2 = (s2 + s1) % BASE;
	}

	return (s2 << 16) | s1;
}

void lz77_dec(struct deflate_ctx *ctx, const uint8 *s, uint32 len) {
	struct deflate_lz77_tok *tok = ctx->lz77_ctx.tok;
	int32 n = ctx->lz77_ctx.tok_size;
	uint8 dict[DEFLATE_DICT_SIZE];
	int32 pos = 0, t = 0;

	for (int32 i = 0; i < n; i++) {
		if (tok[i].dist) {
			t += 3;
			uint32 k = pos - tok[i].dist;
			for (uint32 j = 0; j < tok[i].ll; j++)
				dict[pos++] = dict[k + j];
		} else {
			t += 1;
			dict[pos++] = tok[i].ll;
		}
	}
	dict[pos] = '\0';

	printf("dec: %s\n", dict);

	printf("ratio: %u/%u = %u%%\n", t, pos, 100 - (t * 100) / pos);
	printf("cmp: %d\n", XSYMBOL(memcmp)(s, dict, len));
}

int main(void) {
	uint8 *s = malloc(4 << 20);
	memset(s, 0, 4 << 20);
	uint32 len = fread(s, 1, ((4 << 20) - 1), stdin);

	struct deflate_ctx ctx;
	FSYMBOL(deflate_init)(&ctx);
	FSYMBOL(deflate_process)(&ctx, s, len);

	for (uint32 i = 0; i < ctx.bits_len; i++)
		printf(" %02x", ctx.bits_buf[i]);
	printf(" %08x\n", adler32(adler32(1, NULL, 0), s, len));

	printf("ratio: %u/%u = %d%%\n",
		ctx.bits_len, len, 100 - ((ctx.bits_len * 100) / len));

	lz77_dec(&ctx, s, len);

	return 0;
}
