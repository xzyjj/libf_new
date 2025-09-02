#include <stdio.h>
#include <libf/sl/xstdint.h>


enum {
	TOKEN_NUM,
	TOKEN_ADD,
	TOKEN_SUB,
	TOKEN_MUL,
	TOKEN_DIV,
	TOKEN_MOD
};

void parser(const int32 *tok, int32 n) {
	if (tok[i].)
	printf("%d\n", tok[i]);
}

int main(void) {
	int32 tok[] = {
		TOKEN_NUM, TOKEN_ADD, TOKEN_NUM
		};

	parser(tok, sizeof(tok) / 4);

	return 0;
} 
