#include <stdio.h>
#include <libf/sl/xstdint.h>


static uint8 lex_table[256];

static void lex_table_init(void) {
	for (int32 i = 0; i < 256; i++) {
		if (i >= '0' && i <= '9') {
			lex_table[i] = 1;
		} else if (i >= 'A' && i <= 'Z') {
			lex_table[i] = 2;
		} else if (i >= 'a' && i <= 'z') {
			lex_table[i] = 2;
		} else if (i == '_') {
			lex_table[i] = 2;
		} else if (i >= '!' && i <= '/') {
			lex_table[i] = 3;
		} else if (i >= ':' && i <= '?') {
			lex_table[i] = 3;
		} else if (i >= '[' && i <= '_') {
			lex_table[i] = 3;
		} else if (i >= '{' && i <= '~') {
			lex_table[i] = 3;
		} else if (i == ' ' || i == '\n' || i == '\t' || i == '\r') {
			lex_table[i] = 4;
		} else if (i == 0x0c) {
			lex_table[i] = 4;
		} else if (i > 127) {
			lex_table[i] = 2;
		} else {
			lex_table[i] = 0;
		}
	}
}

static void lex_table_print(char *s1, char *s2) {
	printf("/* %s */\n", s1);
	printf("static const uint8 %s_table = {\n\t", s2);
	for (int32 i = 0; i < 256; ) {
		printf("%d, ", lex_table[i++]);
		if (!(i % 16))
			printf("\n\t");
	}
	printf("};\n\n");
}

int main(void) {
	lex_table_init();
	lex_table_print("", "lex");

	return 0;
}
