#include <stdio.h>
#include <libf/sl/xstdint.h>


static uint8 xml_table[256];

static void xml_table_init(void) {
	for (int32 i = 0; i < 256; i++) {
		if (i >= 'A' && i <= 'Z') {
			xml_table[i] = 0;
		} else if (i >= 'a' && i <= 'z') {
			xml_table[i] = 0;
		} else if (i >= '0' && i <= '9') {
			xml_table[i] = 0;
		} else if (i == '_' || i == '-' || i == ':' || i == '.') {
			xml_table[i] = 0;
		} else if (i > 127) {
			xml_table[i] = 0;
		} else {
			xml_table[i] = 1;
		}
	}
}

static void xml_table_print(char *s1, char *s2) {
	printf("/* %s */\n", s1);
	printf("static const uint8 %s_table = {\n\t", s2);
	for (int32 i = 0; i < 256; ) {
		printf("%d, ", xml_table[i++]);
		if (!(i % 16))
			printf("\n\t");
	}
	printf("};\n\n");
}

int main(void) {
	xml_table_init();
	xml_table_print("", "xml_name");

	return 0;
}
