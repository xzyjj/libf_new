#include <stdio.h>
#include <libf/sl/xstdint.h>


static uint32 crc32_table[256];

static void crc32_table_msb(uint32 n) {
	for (int32 i = 0; i < 256; i++) {
		uint32 c = i << 24;
		for (int32 k = 8; k > 0; k--) {
			if (c & 0x80000000) {
				c = (c << 1) ^ n;
			} else {
				c <<= 1;
			}
		}
		crc32_table[i] = c;
	}
}

static void crc32_table_lsb(uint32 n) {
	for (int32 i = 0; i < 256; i++) {
		uint32 c = i;
		for (int32 k = 8; k > 0; k--) {
			if (c & 0x01) {
				c = (c >> 1) ^ n;
			} else {
				c >>= 1;
			}
		}
		crc32_table[i] = c;
	}
}

static void crc32_table_print(char *s1, char *s2) {
	printf("/* %s */\n", s1);
	printf("static const uint32 crc32_table%s = {\n\t", s2);
	for (int32 i = 0; i < 256; ) {
		printf("0x%08x, ", crc32_table[i++]);
		if (!(i % 4))
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

	crc32_table_msb(0x04c11db7);
	crc32_table_print("crc32 not reversed: 0x04c11db7 (msb)", "_m");
	crc32_table_lsb(0xedb88320);
	crc32_table_print("crc32 after reversed: 0xedb88320 (lsb)", "_l");

	crc32_table_msb(0x1edc6f41);
	crc32_table_print("crc32 not reversed: 0x1edc6f41 (msb)", "_mc");
	crc32_table_lsb(0x82f63b78);
	crc32_table_print("crc32 after reversed: 0x82f63b78 (lsb)", "_lc");

	crc32_table_msb(0x741b8cd7);
	crc32_table_print("crc32 not reversed: 0x741b8cd7 (msb)", "_mk");
	crc32_table_lsb(0xeb31d82e);
	crc32_table_print("crc32 after reversed: 0xeb31d82e (lsb)", "_lk");

	crc32_table_msb(0x814141ab);
	crc32_table_print("crc32 not reversed: 0x814141ab (msb)", "_mq");
	crc32_table_lsb(0xd5828281);
	crc32_table_print("crc32 after reversed: 0xd5828281 (lsb)", "_lq");

	return 0;
}
