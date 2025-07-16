#include <stdio.h>
#include <time.h>
#include <libf/sl/xstdint.h>
#include <libf/al/x448_fast.h>


int main(void) {
	clock_t start, end;
	double time;

	uint32 p[14] = {
		0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
		0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
		0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
		0xffffffff, 0xffffffff
		};

	start = clock();
	for (int32 i = 0; i < 1000; i++) {
		FSYMBOL(x448_fast_clamp_key)(p);
		FSYMBOL(x448_fast_scalar_mul)(p, X448_FAST_B, p);
	}
	end = clock();
	time = (double)(end - start) / CLOCKS_PER_SEC;
	printf("time: %.6f (%.2f/s)\n", time / 1000, 1000 / time);

	return 0;
}
