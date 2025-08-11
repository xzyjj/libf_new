#include <stdio.h>
#include <libf/sl/xstdint.h>


static uint8 ini_table[256];

static void ini_table_init(void) {
	for (int32 i = 0; i < 256; i++) {
		if (i >= 'A' && i <= 'Z') {
			ini_table[i] = 0;
		} else if (i >= 'a' && i <= 'z') {
			ini_table[i] = 0;
		} else if (i >= '0' && i <= '9') {
			ini_table[i] = 0;
		} else if (i == '_' || i == '.') {
			ini_table[i] = 0;
		} else if (i > 127) {
			ini_table[i] = 0;
		} else {
			ini_table[i] = 1;
		}
	}
}

static void ini_key_table_init(void) {
	for (int32 i = 0; i < 256; i++) {
		if (i >= 0x20 && i < 127) {
			ini_table[i] = 0;
		} else if (i > 127) {
			ini_table[i] = 0;
		} else {
			ini_table[i] = 1;
		}
	}
}

static void ini_table_print(char *s1, char *s2) {
	printf("/* %s */\n", s1);
	printf("static const uint8 %s_table = {\n\t", s2);
	for (int32 i = 0; i < 256; ) {
		printf("%d, ", ini_table[i++]);
		if (!(i % 16))
			printf("\n\t");
	}
	printf("};\n\n");
}

int main(void) {
	ini_table_init();
	ini_table_print("", "ini_name");
	ini_table_print("", "ini_key");
	ini_key_table_init();
	ini_table_print("", "ini_value");

	return 0;
}
