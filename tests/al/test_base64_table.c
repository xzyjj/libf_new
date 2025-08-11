#include <stdio.h>
#include <libf/sl/xstdint.h>


static uint8 base64_table[256];

static void base64_table_init(void) {
	for (int32 i = 0; i < 256; i++) {
		if (i >= 'A' && i <= 'Z') {
			base64_table[i] = i - 'A';
		} else if (i >= 'a' && i <= 'z') {
			base64_table[i] = i - 'a' + 26;
		} else if (i >= '0' && i <= '9') {
			base64_table[i] = i - '0' + 52;
		} else if (i == '+') {
			base64_table[i] = i - '+' + 62;
		} else if (i == '/') {
			base64_table[i] = i - '/' + 63;
		} else if (i == '=') {
			base64_table[i] = 254;
		} else {
			base64_table[i] = 255;
		}
	}
}

static void base64_table_print(char *s1, char *s2) {
	printf("/* %s */\n", s1);
	printf("static const uint8 base%s_table = {\n\t", s2);
	for (int32 i = 0; i < 256; ) {
		printf("%3d, ", base64_table[i++]);
		if (!(i % 12))
			printf("\n\t");
	}
	printf("};\n\n");
}

int main(void) {
	base64_table_init();
	base64_table_print("", "64");

	return 0;
}
