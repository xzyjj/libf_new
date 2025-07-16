#include <stdio.h>
#include <time.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>


char buf[2 << 20];
char buf2[2 << 20];

int main(void) {
	clock_t start, end;
	double time;

	for (int32 i = 0; i < 1000; i++) {
		XSYMBOL(memset)(buf, 0, sizeof(buf));
		XSYMBOL(memset)(buf2, 0, sizeof(buf2));
	}

	/* t, s */
	start = clock();
	for (int32 i = 0; i < 1000; i++)
		XSYMBOL(memcpy)(buf2, buf, sizeof(buf));
	end = clock();
	time = (double)(end - start) / CLOCKS_PER_SEC;
	printf("libf: time: %.6f (%.2fMiB/s)\n", time / 1000,
		(sizeof(buf) / (time / 1000)) / 1024 / 1024);

	start = clock();
	for (int32 i = 0; i < 1000; i++)
		memcpy(buf2, buf, sizeof(buf));
	end = clock();
	time = (double)(end - start) / CLOCKS_PER_SEC;
	printf("libc: time: %.6f (%.2fMiB/s)\n", time / 1000,
		(sizeof(buf) / (time / 1000)) / 1024 / 1024);

	/* t + 3, s */
	start = clock();
	for (int32 i = 0; i < 1000; i++)
		XSYMBOL(memcpy)(buf2 + 3, buf, sizeof(buf) - 10);
	end = clock();
	time = (double)(end - start) / CLOCKS_PER_SEC;
	printf("libf: time: %.6f (%.2fMiB/s)\n", time / 1000,
		(sizeof(buf) / (time / 1000)) / 1024 / 1024);

	start = clock();
	for (int32 i = 0; i < 1000; i++)
		memcpy(buf2 + 3, buf, sizeof(buf) - 10);
	end = clock();
	time = (double)(end - start) / CLOCKS_PER_SEC;
	printf("libc: time: %.6f (%.2fMiB/s)\n", time / 1000,
		(sizeof(buf) / (time / 1000)) / 1024 / 1024);

	/* t + 3, s + 7 */
	start = clock();
	for (int32 i = 0; i < 1000; i++)
		XSYMBOL(memcpy)(buf2 + 3, buf + 7, sizeof(buf) - 10);
	end = clock();
	time = (double)(end - start) / CLOCKS_PER_SEC;
	printf("libf: time: %.6f (%.2fMiB/s)\n", time / 1000,
		(sizeof(buf) / (time / 1000)) / 1024 / 1024);

	start = clock();
	for (int32 i = 0; i < 1000; i++)
		memcpy(buf2 + 3, buf + 7, sizeof(buf) - 10);
	end = clock();
	time = (double)(end - start) / CLOCKS_PER_SEC;
	printf("libc: time: %.6f (%.2fMiB/s)\n", time / 1000,
		(sizeof(buf) / (time / 1000)) / 1024 / 1024);

	return 0;
}
