#include <stdio.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xfloat.h>
#include <libf/sl/xstring.h>
#include <libf/sl/xstdio_sys.h>


int main(void) {
	/* signed */
	printf("libc: hhd: %hhd %hhd\n", INT8_MIN, INT8_MAX);
	XSYMBOL(printf)("libf: hhd: %hhd %hhd\n", INT8_MIN, INT8_MAX);
	printf("libc: hhd: %hhd %hhd\n", INT32_MIN, INT32_MAX);
	XSYMBOL(printf)("libf: hhd: %hhd %hhd\n", INT32_MIN, INT32_MAX);

	printf("libc: hd: %hd %hd\n", INT16_MIN, INT16_MAX);
	XSYMBOL(printf)("libf: hd: %hd %hd\n", INT16_MIN, INT16_MAX);
	printf("libc: hd: %hd %hd\n", INT32_MIN, INT32_MAX);
	XSYMBOL(printf)("libf: hd: %hd %hd\n", INT32_MIN, INT32_MAX);

	printf("libc: d: %d %d\n", INT32_MIN, INT32_MAX);
	XSYMBOL(printf)("libf: d: %d %d\n", INT32_MIN, INT32_MAX);
	printf("libc: d: %d %d\n", INT64_MIN, INT64_MAX);
	XSYMBOL(printf)("libf: d: %d %d\n", INT64_MIN, INT64_MAX);

	printf("libc: ld: %ld %ld\n", INT64_MIN, INT64_MAX);
	XSYMBOL(printf)("libf: ld: %ld %ld\n", INT64_MIN, INT64_MAX);

	printf("libc: lld: %lld %lld\n", INT64L_MIN, INT64L_MAX);
	XSYMBOL(printf)("libf: lld: %lld %lld\n", INT64L_MIN, INT64L_MAX);

	/* unsigned */
	printf("libc: hhu: %hhu\n", UINT8_MAX);
	XSYMBOL(printf)("libf: hhu: %hhu\n", UINT8_MAX);

	printf("libc: hhu: %hhu\n", UINT32_MAX);
	XSYMBOL(printf)("libf: hhu: %hhu\n", UINT32_MAX);

	printf("libc: hu: %hu\n", UINT16_MAX);
	XSYMBOL(printf)("libf: hu: %hu\n", UINT16_MAX);
	printf("libc: hu: %hu\n", UINT32_MAX);
	XSYMBOL(printf)("libf: hu: %hu\n", UINT32_MAX);

	printf("libc: u: %u\n", UINT32_MAX);
	XSYMBOL(printf)("libf: u: %u\n", UINT32_MAX);
	printf("libc: u: %u\n", UINT64_MAX);
	XSYMBOL(printf)("libf: u: %u\n", UINT64_MAX);

	printf("libc: lu: %lu\n", UINT64_MAX);
	XSYMBOL(printf)("libf: lu: %lu\n", UINT64_MAX);

	/* signed */
	printf("libc: hhx: %hhx %hhx\n", INT8_MIN, INT8_MAX);
	XSYMBOL(printf)("libf: hhx: %hhx %hhx\n", INT8_MIN, INT8_MAX);
	printf("libc: hhx: %hhx %hhx\n", INT32_MIN, INT32_MAX);
	XSYMBOL(printf)("libf: hhx: %hhx %hhx\n", INT32_MIN, INT32_MAX);

	printf("libc: hx: %hx %hx\n", INT16_MIN, INT16_MAX);
	XSYMBOL(printf)("libf: hx: %hx %hx\n", INT16_MIN, INT16_MAX);
	printf("libc: hx: %hx %hx\n", INT32_MIN, INT32_MAX);
	XSYMBOL(printf)("libf: hx: %hx %hx\n", INT32_MIN, INT32_MAX);

	printf("libc: x: %x %x\n", INT32_MIN, INT32_MAX);
	XSYMBOL(printf)("libf: x: %x %x\n", INT32_MIN, INT32_MAX);
	printf("libc: x: %x %x\n", INT64_MIN, INT64_MAX);
	XSYMBOL(printf)("libf: x: %x %x\n", INT64_MIN, INT64_MAX);

	printf("libc: lx: %lx %lx\n", INT64_MIN, INT64_MAX);
	XSYMBOL(printf)("libf: lx: %lx %lx\n", INT64_MIN, INT64_MAX);

	printf("libc: llx: %llx %llx\n", INT64L_MIN, INT64L_MAX);
	XSYMBOL(printf)("libf: llx: %llx %llx\n", INT64L_MIN, INT64L_MAX);

	/* unsigned hex */
	printf("libc: llu: %llu\n", UINT64L_MAX);
	XSYMBOL(printf)("libf: llu: %llu\n", UINT64L_MAX);

	printf("libc: hhx: %hhx\n", UINT8_MAX);
	XSYMBOL(printf)("libf: hhx: %hhx\n", UINT8_MAX);

	printf("libc: hhx: %hhx\n", UINT32_MAX);
	XSYMBOL(printf)("libf: hhx: %hhx\n", UINT32_MAX);

	printf("libc: hx: %hx\n", UINT16_MAX);
	XSYMBOL(printf)("libf: hx: %hx\n", UINT16_MAX);
	printf("libc: hx: %hx\n", UINT32_MAX);
	XSYMBOL(printf)("libf: hx: %hx\n", UINT32_MAX);

	printf("libc: x: %x\n", UINT32_MAX);
	XSYMBOL(printf)("libf: x: %x\n", UINT32_MAX);
	printf("libc: x: %x\n", UINT64_MAX);
	XSYMBOL(printf)("libf: x: %x\n", UINT64_MAX);

	printf("libc: lx: %lx\n", UINT64_MAX);
	XSYMBOL(printf)("libf: lx: %lx\n", UINT64_MAX);

	printf("libc: llx: %llx\n", UINT64L_MAX);	
	XSYMBOL(printf)("libf: llx: %llx\n", UINT64L_MAX);

	/* floating */
	printf("libc: f: %.380f\n", XFP_DBL_MAX);
	XSYMBOL(printf)("libf: f: %.380f\n", XFP_DBL_MAX);

	printf("libc: f: %.380f\n", -XFP_DBL_MAX);
	XSYMBOL(printf)("libf: f: %.380f\n", -XFP_DBL_MAX);

	printf("libc: f: %.380f\n", XFP_DBL_MIN);
	XSYMBOL(printf)("libf: f: %.380f\n", XFP_DBL_MIN);

	printf("libc: f: %.380f\n", -XFP_DBL_MIN);
	XSYMBOL(printf)("libf: f: %.380f\n", -XFP_DBL_MIN);

	printf("libc: f: %.380f\n", -XFP_DBL_MIN);
	XSYMBOL(printf)("libf: f: %.380f\n", -XFP_DBL_MIN);

	/* character and string */
	printf("libc: c: %c\n", 'w');
	XSYMBOL(printf)("libf: c: %c\n", 'w');

	printf("libc: c: %9c\n", 'w');
	XSYMBOL(printf)("libf: c: %9c\n", 'w');

	printf("libc: s: %s\n", "Hello, World");
	XSYMBOL(printf)("libf: s: %s\n", "Hello, World");

	printf("libc: s: %19s\n", "Hello, World");
	XSYMBOL(printf)("libf: s: %19s\n", "Hello, World");

	printf("libc: s: %-19s-\n", "Hello, World");
	XSYMBOL(printf)("libf: s: %-19s-\n", "Hello, World");

	printf("libc: s: %33s-\n", "Hello, World");
	XSYMBOL(printf)("libf: s: %33s-\n", "Hello, World");

	printf("libc: s: %33s ω %s-\n", "Hello, World ω", NULL);
	XSYMBOL(printf)("libf: s: %33s ω %s-\n", "Hello, World ω", NULL);

	printf("libc: s: %*s ω %s-\n", -33, "Hello, World ω", NULL);
	XSYMBOL(printf)("libf: s: %*s ω %s-\n", -33, "Hello, World ω", NULL);

	return 0;
}
