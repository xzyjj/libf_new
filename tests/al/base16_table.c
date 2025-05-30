#include <stdio.h>
#include <libf/sl/xstdint.h>


static uint8 base16_table[256];

static void base16_table_init(void) {
	for (int32 i = 0; i < 256; i++) {
		if (i >= 'A' && i <= 'F') {
			base16_table[i] = i - 'A' + 10;
		} else if (i >= 'a' && i <= 'f') {
			base16_table[i] = i - 'a' + 10;
		} else if (i >= '0' && i <= '9') {
			base16_table[i] = i - '0';
		} else {
			base16_table[i] = 255;
		}
	}
}

static void base16_table_print(char *s1, char *s2) {
	printf("/* %s */\n", s1);
	printf("static const uint8 base%s_table = {\n\t", s2);
	for (int32 i = 0; i < 256; ) {
		printf("%3d, ", base16_table[i++]);
		if (!(i % 12))
			printf("\n\t");
	}
	printf("};\n\n");
}

int main(void) {
	base16_table_init();
	base16_table_print("", "16");

	return 0;
}
