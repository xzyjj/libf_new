#include <stdio.h>
#include <time.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/al/ed25519_fast.h>


int main(void) {
	clock_t start, end;
	double time;

	uint32 p[8] = {
		0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
		0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
		};
	uint8 sign[ED25519_SIGN_LEN];

	char *s = "Hello, World!";
	uint32 s_len = XSYMBOL(strlen)(s);

	start = clock();
	for (int32 i = 0; i < 1000; i++) {
		FSYMBOL(ed25519_fast_sign)((uint8 *)p, (uint8 *)s, s_len, sign);
	}
	end = clock();
	time = (double)(end - start) / CLOCKS_PER_SEC;
	printf("time: %.6f\n", time / 1000);

	return 0;
}
