#include <stdio.h>
#include <time.h>
#include <libf/sl/xstdint.h>
#include <libf/al/x25519_fast.h>


int main(void) {
	clock_t start, end;
	double time;

	uint32 p[8] = {
		0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
		0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
		};

	start = clock();
	for (int32 i = 0; i < 1000; i++) {
		FSYMBOL(x25519_fast_clamp_key)(p);
		FSYMBOL(x25519_fast_scalar_mul)(p, X25519_FAST_B, p);
	}
	end = clock();
	time = (double)(end - start) / CLOCKS_PER_SEC;
	printf("time: %.6f\n", time / 1000);

	return 0;
}
