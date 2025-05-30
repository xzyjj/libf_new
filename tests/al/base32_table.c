#include <stdio.h>
#include <libf/sl/xstdint.h>


static uint8 base32_table[256];

static void base32_table_init(void) {
	for (int32 i = 0; i < 256; i++) {
		if (i >= 'A' && i <= 'Z') {
			base32_table[i] = i - 'A';
		} else if (i >= '2' && i <= '7') {
			base32_table[i] = i - '2' + 26;
		} else if (i == '=') {
			base32_table[i] = 254;
		} else {
			base32_table[i] = 255;
		}
	}
}

static void base32_table_print(char *s1, char *s2) {
	printf("/* %s */\n", s1);
	printf("static const uint8 base%s_table = {\n\t", s2);
	for (int32 i = 0; i < 256; ) {
		printf("%3d, ", base32_table[i++]);
		if (!(i % 12))
			printf("\n\t");
	}
	printf("};\n\n");
}

int main(void) {
	base32_table_init();
	base32_table_print("", "32");

	return 0;
}
