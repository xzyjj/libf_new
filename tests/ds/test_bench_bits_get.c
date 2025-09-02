#include <stdio.h>
#include <time.h>
#include <libf/sl/xstdint.h>
#include <libf/ds/bits_get.h>


int main(void) {
	clock_t start, end;
	double time;
	uint8 buf[16];
	uint32 v;
	BITS_GET_NEW(ctx, buf, 16);

	start = clock();
	for (int32 i = 0; i < 10000000; i++) {
		if (FSYMBOL(bits_get)(&ctx, &v, (i & 11) + 1, 0))
			BITS_GET_RESET(&ctx);
	}
	end = clock();
	time = (double)(end - start) / CLOCKS_PER_SEC;

	uint64 n = 0;
	for (int32 i = 0; i < 10000000; i++) {
		n += (i & 11) + 1;
	}
	printf("time: %.6f (%.2fMiB/s)\n", time,
		(n / time) / 8 / 1000 / 1000);

	return 0;
}
