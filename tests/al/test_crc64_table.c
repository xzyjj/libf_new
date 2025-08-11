#include <stdio.h>
#include <libf/sl/xstdint.h>


static uint64L crc64_table[256];

static void crc64_table_msb(uint64L n) {
	for (int32 i = 0; i < 256; i++) {
		uint64L c = i;
		c <<= 56;
		for (int32 k = 8; k > 0; k--) {
			if (c & 0x8000000000000000) {
				c = (c << 1) ^ n;
			} else {
				c <<= 1;
			}
		}
		crc64_table[i] = c;
	}
}

static void crc64_table_lsb(uint64L n) {
	for (int32 i = 0; i < 256; i++) {
		uint64L c = i;
		for (int32 k = 8; k > 0; k--) {
			if (c & 0x01) {
				c = (c >> 1) ^ n;
			} else {
				c >>= 1;
			}
		}
		crc64_table[i] = c;
	}
}

static void crc64_table_print(char *s1, char *s2) {
	printf("/* %s */\n", s1);
	printf("static const uint64L crc64_table%s = {\n\t", s2);
	for (int32 i = 0; i < 256; ) {
		printf("0x%016llxULL, ", crc64_table[i++]);
		if (!(i % 2))
			printf("\n\t");
	}
	printf("};\n\n");
}

int main(void) {
	/*
	* RefIn == false, not reversed
	* RefIn == true, after reversed
	*
	* Binary: 00000011 after 11000000
	*/

	crc64_table_msb(0x000000000000001b);
	crc64_table_print("crc64 not reversed: 0x000000000000001b (msb)", "_m");
	crc64_table_lsb(0xd800000000000000);
	crc64_table_print("crc64 after reversed: 0xd800000000000000 (lsb)", "_l");

	crc64_table_msb(0x42f0e1eba9ea3693);
	crc64_table_print("crc64 not reversed: 0x42f0e1eba9ea3693 (msb)", "_me");
	crc64_table_lsb(0xc96c5795d7870f42);
	crc64_table_print("crc64 after reversed: 0xc96c5795d7870f42 (lsb)", "_le");

	return 0;
}
