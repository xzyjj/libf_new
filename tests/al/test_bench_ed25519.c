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

	uint32 s_len = 1 << 16;
	char s[1<<17];
	XSYMBOL(memset)(s, 'A', s_len);

	/* sign */
	start = clock();
	for (int32 i = 0; i < 1000; i++) {
		FSYMBOL(ed25519_fast_sign)((uint8 *)p, (uint8 *)s, s_len, sign);
	}
	end = clock();
	time = (double)(end - start) / CLOCKS_PER_SEC;
	printf("sign time: %.6f (%.2f/s)\n", time / 1000, 1000 / time);

	/* verify */
	start = clock();
	for (int32 i = 0; i < 1000; i++) {
		FSYMBOL(ed25519_fast_verify)((uint8 *)p, sign, (uint8 *)s, s_len);
	}
	end = clock();
	time = (double)(end - start) / CLOCKS_PER_SEC;
	printf("verify time: %.6f (%.2f/s)\n", time / 1000, 1000 / time);

	return 0;
}
