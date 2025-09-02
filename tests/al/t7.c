#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/ds/bits_add.h>


#undef DEFLATE_MATCH_MIN
#define DEFLATE_MATCH_MIN 3
#undef DEFLATE_MATCH_MAX
#define DEFLATE_MATCH_MAX 258

#undef DEFLATE_LEN_CODES
#define DEFLATE_LEN_CODES 29
#undef DEFLATE_DIST_CODES
#define DEFLATE_DIST_CODES 30
#undef DEFLATE_BITLEN_CODES
#define DEFLATE_BITLEN_CODES 19

#undef DEFLATE_LEN_SIZE
#define DEFLATE_LEN_SIZE (DEFLATE_MATCH_MAX - DEFLATE_MATCH_MIN + 1)
#undef DEFLATE_DIST_SIZE
#define DEFLATE_DIST_SIZE ((DEFLATE_MATCH_MAX - DEFLATE_MATCH_MIN + 1) * 2)

#undef DEFLATE_BITS_MAX
#define DEFLATE_BITS_MAX 15
#undef DEFLATE_LITERALS
#define DEFLATE_LITERALS 256
#undef DEFLATE_END_BLOCK
#define DEFLATE_END_BLOCK 256

#undef DEFLATE_L_CODES
#define DEFLATE_L_CODES (DEFLATE_LITERALS + 1 + DEFLATE_LEN_CODES + 2)
#undef DEFLATE_D_CODES
#define DEFLATE_D_CODES DEFLATE_DIST_CODES

#undef DEFLATE_HEAP_SIZE
#define DEFLATE_HEAP_SIZE (DEFLATE_L_CODES * 2 + 1)
#undef DEFLATE_L_SIZE
#define DEFLATE_L_SIZE DEFLATE_HEAP_SIZE
#undef DEFLATE_D_SIZE
#define DEFLATE_D_SIZE (DEFLATE_D_CODES * 2 + 1)

static const uint8 extra_len_bits[DEFLATE_LEN_CODES] = {
	0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 2, 2, 2, 2,
	3, 3, 3, 3, 4, 4, 4, 4,
	5, 5, 5, 5, 0
	};

static const uint8 extra_dist_bits[DEFLATE_DIST_CODES] = {
	 0,  0,  0,  0,  1,  1,  2,  2,
	 3,  3,  4,  4,  5,  5,  6,  6,
	 7,  7,  8,  8,  9,  9, 10, 10,
	11, 11, 12, 12, 13, 13
	};

static const uint8 extra_bitlen_bits[DEFLATE_BITLEN_CODES] = {
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	2, 3, 7
	};

static const uint8 extra_bitlen_order[DEFLATE_BITLEN_CODES] = {
	16, 17, 18,  0,  8,  7,  9,  6,
	10,  5, 11,  4, 12,  3, 13,  2,
	14,  1, 15
	};

static const uint8 base_len[DEFLATE_LEN_CODES] = {
	  0,   1,   2,   3,   4,   5,   6,   7,
	  8,  10,  12,  14,  16,  20,  24,  28,
	 32,  40,  48,  56,  64,  80,  96, 112,
	128, 160, 192, 224,   0
	};

static const uint16 base_dist[DEFLATE_DIST_CODES] = {
	   0,     1,     2,     3,     4,     6,     8,    12,
	  16,    24,    32,    48,    64,    96,   128,   192,
	 256,   384,   512,   768,  1024,  1536,  2048,  3072,
	4096,  6144,  8192, 12288, 16384, 24576
	};

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
	27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 28, 
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
	29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 
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
	{ { 163 }, { 8 } }, { {  99 }, { 8 } }, { { 227 }, { 8 } }, 
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
	{ { 27 }, { 5 } }, { {  7 }, { 5 } }, { { 23 }, { 5 } }, 
	};


#undef MATCH_MIN
#define MATCH_MIN 3
#undef MATCH_MAX
#define MATCH_MAX 258

#undef DICT_SIZE
#define DICT_SIZE 32768

#undef LAZY_MAX
#define LAZY_MAX 32
#undef NICE_LEN
#define NICE_LEN 128

struct lz77_tok {
	uint16 len;
	uint16 dist;
	uint8 literal;
};

uint32 out = 0, lazy_max = 0, nice_len = 3;
struct lz77_tok tok[65536];

static uint32 _lz77_match(uint8 *dict, uint32 dict_len,
		const uint8 *look, uint32 look_pos, uint32 look_len,
		uint32 *best_dist, uint32 nice_len) {
	uint32 best_len = 0;
	uint32 len_max = look_len - look_pos;

	if (len_max > MATCH_MAX)
		len_max = MATCH_MAX;
	if (len_max < MATCH_MIN)
		return 0;

	for (uint32 i = 0; i < dict_len; i++) {
		uint32 len = 0;
		while (len < len_max
				&& dict[i + len] == look[look_pos + len])
			len++;

		if (len > best_len) {
			best_len = len;
			*best_dist = dict_len - i;
			if (best_len >= nice_len) /* nice */
				break;
		}
	}

	return (best_len < MATCH_MIN) ? 0 : best_len;
}

#undef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

void lz77_enc(const uint8 *s, uint32 len) {
	uint8 dict[DICT_SIZE];
	uint32 pos = 0, dict_len = 0,
		best_len, best_dist,
		next_len, next_dist;

	len = MIN(len, DICT_SIZE);

//	printf("enc: %s\n", (char *)s);
	out = 0;
	while (pos < len) {
		if ((len - pos) > MATCH_MIN) {
			best_len = _lz77_match(dict, dict_len,
				s, pos, len,
				&best_dist, nice_len);
			if (best_len < lazy_max && (pos + 1) < len) {
				next_len = _lz77_match(dict, dict_len,
					s, pos + 1, len,
					&next_dist, NICE_LEN);

				if (next_len > best_len) {
					best_len = 0;
				} else if (next_len == best_len && next_dist < best_dist) {
					best_len = 0;
				} else if (best_len == MATCH_MIN && next_len >= MATCH_MIN) {
					best_len = 0;
				}
			}
		} else {
			best_len = 0;
		}

		if (best_len > MATCH_MIN) {
			tok[out].len = best_len;
			tok[out].dist = best_dist;
			out++;

			printf(" <%u,%u>", best_dist, best_len);
			for (uint32 i = 0; i < best_len; i++)
				dict[dict_len + i] = s[pos + i];
			dict_len += best_len;
			pos += best_len;
		} else {
			tok[out].len = 0;
			tok[out].literal = s[pos];
			out++;

			if (s[pos] > 0x20 && s[pos] < 0x7f) {
				printf(" %c", s[pos]);
			} else {
				printf(" (%02x)", s[pos]);
			}
			dict[dict_len++] = s[pos++];
		}
	}

	printf("\n");
}

void lz77_dec(const uint8 *s, uint32 len) {
	uint8 dict[DICT_SIZE];
	uint32 pos = 0, t = 0;

	for (uint32 i = 0; i < out; i++) {
		if (tok[i].len) {
			t += 3;
			uint32 k = pos - tok[i].dist;
			for (uint32 j = 0; j < tok[i].len; j++)
				dict[pos++] = dict[k + j];
		} else {
			t += 1;
			dict[pos++] = tok[i].literal;
		}
	}
	dict[pos] = '\0';

	printf("dec: %s\n", dict);

	printf("ratio: %u/%u = %u%%\n", t, pos, 100 - (t * 100) / pos);
	printf("cmp: %d\n", XSYMBOL(memcmp)(s, dict, len));
}

uint8 bit_buf[65536];
uint32 bit_buf_len = 0;
struct bits_add_ctx add_ctx;

void send_bits(uint32 v, uint32 len) {
	if (FSYMBOL(bits_add)(&add_ctx, v, len)) {
		XSYMBOL(memcpy)(bit_buf + bit_buf_len,
			BITS_ADD_BUF(&add_ctx),
			BITS_ADD_BUFSIZE);
		bit_buf_len += BITS_ADD_BUFSIZE;
		BITS_ADD_FLUSH(&add_ctx);

		if (BITS_ADD_BREM(&add_ctx))
			FSYMBOL(bits_add)(&add_ctx, v, len);
	}
}

void send_bits_flush(void) {
	uint32 t = BITS_ADD_GETBITS(&add_ctx);
	if (t) {
		t = BITS_ADD_BTASIZE(t);
		XSYMBOL(memcpy)(bit_buf + bit_buf_len,
			BITS_ADD_BUF(&add_ctx),
			t);
		bit_buf_len += t;
	}
}

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

#if 0

void lz77_to_huffman(const uint8 *s, uint32 len) {
	uint32 pos = 0, t_pos = 0, t = 0, code, dist;

	len = MIN(len, DICT_SIZE);

	bit_buf_len = 0;
	BITS_ADD_INIT(&add_ctx);
	send_bits(0x78, 8);
	send_bits(0x01, 8);

	send_bits(0x01, 1);
	send_bits(0x01, 2);

	for (uint32 i = 0; i < out; i++) {
		if (tok[i].len) {
			code = tok[i].len - MATCH_MIN;
			code = len_code[code];
			send_bits(static_ltree[256 + 1 + code].fc.code,
				static_ltree[256 + 1 + code].dl.len);

			t = extra_len_bits[code];
			if (t)
				send_bits(tok[i].len - MATCH_MIN - base_len[code], t);

			dist = tok[i].dist - 1;
			code = ((dist) < 256 ? dist_code[dist]
				: dist_code[256 + ((dist) >> 7)]);
			send_bits(static_dtree[code].fc.code,
				static_dtree[code].dl.len);

			t = extra_dist_bits[code];
			if (t)
				send_bits(dist - base_dist[code], t);

			t_pos += tok[i].len;
			pos += 3;
		} else {
			code = tok[i].literal;
			send_bits(static_ltree[code].fc.code,
				static_ltree[code].dl.len);

			t_pos++;
			pos++;
		}
	}

	send_bits(static_ltree[DEFLATE_END_BLOCK].fc.code,
		static_ltree[DEFLATE_END_BLOCK].dl.len);
	send_bits_flush();

	for (uint32 i = 0; i < bit_buf_len; i++)
		printf(" %02x", bit_buf[i]);
	printf(" %08x\n", adler32(adler32(1, NULL, 0), s, len));

	for (uint32 i = 0; i < bit_buf_len; i++)
		printf("%02x", bit_buf[i]);
	printf("%08x\n", adler32(adler32(1, NULL, 0), s, len));

	printf("ratio: %u/%u = %d%%, %u/%u = %d%%\n",
		bit_buf_len, pos, 100 - ((bit_buf_len * 100) / pos),
		bit_buf_len, t_pos, 100 - ((bit_buf_len * 100) / t_pos)
		);
}

#else

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

struct dyntree_ctx dyn_ctx;

struct deflate_ctdata ltree[DEFLATE_L_SIZE];
struct deflate_ctdata dtree[DEFLATE_D_SIZE];

struct dyntree_desc desc_ltree = { ltree, DEFLATE_L_CODES, DEFLATE_BITS_MAX, 0 };
struct dyntree_desc desc_dtree = { dtree, DEFLATE_D_CODES, 5, 0 };


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

printf("%d %d\n", n, len);

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

void lz77_to_huffman(const uint8 *s, uint32 len) {
	uint32 pos = 0, t_pos = 0, t = 0, code, dist;

	len = MIN(len, DICT_SIZE);

	for (int32 i = 0; i < DEFLATE_L_CODES; i++)
		ltree[i].fc.freq = 0;
	for (int32 i = 0; i < DEFLATE_D_CODES; i++)
		dtree[i].fc.freq = 0;

	for (uint32 i = 0; i < out; i++) {
		if (tok[i].len) {
			code = tok[i].len - MATCH_MIN;
			ltree[256 + 1 + code].fc.freq++;

			dist = tok[i].dist - 1;
			code = ((dist) < 256 ? dist_code[dist]
				: dist_code[256 + ((dist) >> 7)]);
			dtree[code].fc.freq++;
		} else {
			code = tok[i].literal;
			ltree[code].fc.freq++;
		}
	}

	_build_tree(&dyn_ctx, &desc_ltree);
	_build_tree(&dyn_ctx, &desc_dtree);

	bit_buf_len = 0;
	BITS_ADD_INIT(&add_ctx);
	send_bits(0x78, 8);
	send_bits(0x01, 8);

	send_bits(0x01, 1);
	send_bits(0x01, 2);

	for (uint32 i = 0; i < out; i++) {
		if (tok[i].len) {
			code = tok[i].len - MATCH_MIN;
			code = len_code[code];
			send_bits(ltree[256 + 1 + code].fc.code,
				ltree[256 + 1 + code].dl.len);

			t = extra_len_bits[code];
			if (t)
				send_bits(tok[i].len - MATCH_MIN - base_len[code], t);

			dist = tok[i].dist - 1;
			code = ((dist) < 256 ? dist_code[dist]
				: dist_code[256 + ((dist) >> 7)]);
			send_bits(dtree[code].fc.code,
				dtree[code].dl.len);

			t = extra_dist_bits[code];
			if (t)
				send_bits(dist - base_dist[code], t);

			t_pos += tok[i].len;
			pos += 3;
		} else {
			code = tok[i].literal;
			send_bits(ltree[code].fc.code,
				ltree[code].dl.len);

			t_pos++;
			pos++;
		}
	}

	send_bits(static_ltree[DEFLATE_END_BLOCK].fc.code,
		static_ltree[DEFLATE_END_BLOCK].dl.len);
	send_bits_flush();

	for (uint32 i = 0; i < bit_buf_len; i++)
		printf(" %02x", bit_buf[i]);
	printf(" %08x\n", adler32(adler32(1, NULL, 0), s, len));

	for (uint32 i = 0; i < bit_buf_len; i++)
		printf("%02x", bit_buf[i]);
	printf("%08x\n", adler32(adler32(1, NULL, 0), s, len));

	printf("ratio: %u/%u = %d%%, %u/%u = %d%%\n",
		bit_buf_len, pos, 100 - ((bit_buf_len * 100) / pos),
		bit_buf_len, t_pos, 100 - ((bit_buf_len * 100) / t_pos)
		);
}

#endif

int main(int argc, char *argv[]) {
	clock_t start, end;
	double time;

	char *s = malloc(4 << 20);
	memset(s, 0, 4 << 20);
	uint32 n = fread(s, 1, ((4 << 20) - 1), stdin);

	if (argv[1]) {
		lazy_max = atoi(argv[1]);
		nice_len = argv[2] ? atoi(argv[2]) : 0;
	} else {
		lazy_max = 16;
		nice_len = 32;
	}

	lz77_enc((uint8 *)s, n);
	lz77_dec((uint8 *)s, n);
	lz77_to_huffman((uint8 *)s, n);

/*
	start = clock();
	for (int32 i = 0; i < 100; i++)
		lz77_enc((uint8 *)s, n);
	end = clock();
	time = (double)(end - start) / CLOCKS_PER_SEC;
	printf("enc: %.6fs (%.2fMiB/s)\n", time, ((double)(n * 100) / time) / 1024 / 1024);

	start = clock();
	for (int32 i = 0; i < 1000; i++)
		lz77_dec((uint8 *)s, n);
	end = clock();
	time = (double)(end - start) / CLOCKS_PER_SEC;
	printf("dec: %.6fs (%.2fMiB/s)\n", time, ((double)(n * 1000) / time) / 1024 / 1024);

	start = clock();
	for (int32 i = 0; i < 1000; i++)
		lz77_to_huffman((uint8 *)s, n);
	end = clock();
	time = (double)(end - start) / CLOCKS_PER_SEC;
	printf("huf: %.6fs (%.2fMiB/s)\n", time, ((double)(n * 1000) / time) / 1024 / 1024);
*/

	return 0;
}
