#include <stdio.h>
#include <libf/sl/xstdint.h>


/*
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

#undef DEFLATE_MATCH_MIN
#define DEFLATE_MATCH_MIN 3
#undef DEFLATE_MATCH_MAX
#define DEFLATE_MATCH_MAX 258

#undef DEFLATE_LEN_CODES
#define DEFLATE_LEN_CODES 29
#undef DEFLATE_DIST_CODES
#define DEFLATE_DIST_CODES 30

#undef DEFLATE_LEN_SIZE
#define DEFLATE_LEN_SIZE (DEFLATE_MATCH_MAX - DEFLATE_MATCH_MIN + 1)
#undef DEFLATE_DIST_SIZE
#define DEFLATE_DIST_SIZE ((DEFLATE_MATCH_MAX - DEFLATE_MATCH_MIN + 1) * 2)

#undef DEFLATE_BITS_MAX
#define DEFLATE_BITS_MAX 15
#undef DEFLATE_LITERALS
#define DEFLATE_LITERALS 256

/* NOTE: codes 256 End + 1 */

#undef DEFLATE_L_CODES
#define DEFLATE_L_CODES (DEFLATE_LITERALS + 1 + DEFLATE_LEN_CODES + 2)
#undef DEFLATE_D_CODES
#define DEFLATE_D_CODES DEFLATE_DIST_CODES

/* NOTE: codes 286 and 287 reserved placeholders */

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

uint8 base_len[DEFLATE_LEN_CODES];     /* base length */
uint16 base_dist[DEFLATE_DIST_CODES];  /* base distance */

uint8 len_code[DEFLATE_LEN_SIZE];   /* length code table */
uint8 dist_code[DEFLATE_DIST_SIZE]; /* distance code table */

struct deflate_ctdata static_ltree[DEFLATE_L_CODES];
struct deflate_ctdata static_dtree[DEFLATE_D_CODES];

static uint16 _bit_reverse(uint16 c, uint16 len) {
	uint16 r = 0;
	while (len--) {
		r |= c & 1;
		r <<= 1;
		c >>= 1;
	}

	return r >> 1;
}

/* canonical huffman codes generation */
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

static void deflate_len_code_init(void) {
	uint32 code, len;

	/* length code table */
	len = 0;
	for (code = 0; code < (DEFLATE_LEN_CODES - 1); code++) {
		base_len[code] = len;
		for (int32 j = 0; j < (1 << extra_len_bits[code]); j++)
			len_code[len++] = code;
	}
	base_len[code] = 0;
	len_code[len - 1] = code;
}

static void deflate_dist_code_init(void) {
	uint32 code, len;

	/* distance code table */
	len = 0;
	for (code = 0; code < 16; code++) {
		base_dist[code] = len;
		for (int32 j = 0; j < (1 << extra_dist_bits[code]); j++)
			dist_code[len++] = code;
	}

	len >>= 7;
	for (; code < DEFLATE_DIST_CODES; code++) {
		base_dist[code] = len << 7;
		for (int32 j = 0; j < (1 << (extra_dist_bits[code] - 7)); j++)
			dist_code[256 + len++] = code;
	}
}

static void deflate_base_len_print(char *s1, char *s2) {
	printf("/* %s */\n", s1);
	printf("static const uint8 %sbase_len = {\n\t", s2);
	for (int32 i = 0; i < DEFLATE_LEN_CODES; ) {
		printf("%3u, ", base_len[i]);
		if (!(++i % 8))
			printf("\n\t");
	}
	printf("};\n\n");
}

static void deflate_base_dist_print(char *s1, char *s2) {
	printf("/* %s */\n", s1);
	printf("static const uint16 %sbase_dist = {\n\t", s2);
	for (int32 i = 0; i < DEFLATE_DIST_CODES; ) {
		printf("%5u, ", base_dist[i]);
		if (!(++i % 8))
			printf("\n\t");
	}
	printf("};\n\n");
}

static void deflate_len_code_print(char *s1, char *s2) {
	printf("/* %s */\n", s1);
	printf("static const uint8 %slen_code = {\n\t", s2);
	for (int32 i = 0; i < DEFLATE_LEN_SIZE; ) {
		printf("%2u, ", len_code[i]);
		if (!(++i % 16))
			printf("\n\t");
	}
	printf("};\n\n");
}

static void deflate_dist_code_print(char *s1, char *s2) {
	printf("/* %s */\n", s1);
	printf("static const uint8 %sdist_code = {\n\t", s2);
	for (int32 i = 0; i < DEFLATE_DIST_SIZE; ) {
		printf("%2u, ", dist_code[i]);
		if (!(++i % 16))
			printf("\n\t");
	}
	printf("};\n\n");
}

static void deflate_ltree_init(void) {
	uint16 bitlen_count[DEFLATE_BITS_MAX + 1];
	uint32 code;

	for (code = 0; code <= DEFLATE_BITS_MAX; code++)
		bitlen_count[code] = 0;

	for (code = 0; code < 144; code++) { /* 0~143: 8-bit length */
		static_ltree[code].dl.len = 8;
		bitlen_count[8]++;
	}
	for (; code < 256; code++) { /* 144~255: 9-bit length */
		static_ltree[code].dl.len = 9;
		bitlen_count[9]++;
	}
	for (; code < 280; code++) { /* 256~279: 7-bit length */
		static_ltree[code].dl.len = 7;
		bitlen_count[7]++;
	}
	for (; code < 288; code++) { /* 280~287: 8-bit length */
		static_ltree[code].dl.len = 8;
		bitlen_count[8]++;
	}

	_gen_codes(static_ltree, bitlen_count, code);
}

static void deflate_dtree_init(void) {
	uint32 code;
	
	for (code = 0; code < DEFLATE_D_CODES; code++) {
		static_dtree[code].dl.len = 5;
		static_dtree[code].fc.code = _bit_reverse(code, 5);
	}
}

static void deflate_ltree_print(char *s1, char *s2) {
	printf("/* %s */\n", s1);
	printf("static const struct deflate_ctdata %stree = {\n\t", s2);
	for (int32 i = 0; i < DEFLATE_L_CODES; ) {
		printf("{ { %3u }, { %u } }, ",
			static_ltree[i].fc.code,
			static_ltree[i].dl.len);
		if (!(++i % 3))
			printf("\n\t");
	}
	printf("};\n\n");
}

static void deflate_dtree_print(char *s1, char *s2) {
	printf("/* %s */\n", s1);
	printf("static const struct deflate_ctdata %stree = {\n\t", s2);
	for (int32 i = 0; i < DEFLATE_D_CODES; ) {
		printf("{ { %2u }, { %u } }, ",
			static_dtree[i].fc.code,
			static_dtree[i].dl.len);
		if (!(++i % 3))
			printf("\n\t");
	}
	printf("};\n\n");
}

int main(void) {
	deflate_len_code_init();
	deflate_dist_code_init();

	deflate_base_len_print("base length", "");
	deflate_base_dist_print("base distance", "");
	
	deflate_len_code_print("length code table", "");
	deflate_dist_code_print("distance code table", "");

	deflate_ltree_init();
	deflate_ltree_print("static literal/length tree", "static_l");

	deflate_dtree_init();
	deflate_dtree_print("static distance tree", "static_d");

	return 0;
}
