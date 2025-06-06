#include <stdio.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xfloat.h>
#include <libf/sl/xstring.h>
#include <libf/sl/xstdio.h>
#include <libf/sl/xstdio_sys.h>


int main(void) {
	/* signed */
	printf("hhd: %hhd %hhd\n", INT8_MIN, INT8_MAX);
	XSYMBOL(printf)("hhd: %hhd %hhd\n", INT8_MIN, INT8_MAX);
	printf("hhd: %hhd %hhd\n", INT32_MIN, INT32_MAX);
	XSYMBOL(printf)("hhd: %hhd %hhd\n", INT32_MIN, INT32_MAX);

	printf("hd: %hd %hd\n", INT16_MIN, INT16_MAX);
	XSYMBOL(printf)("hd: %hd %hd\n", INT16_MIN, INT16_MAX);
	printf("hd: %hd %hd\n", INT32_MIN, INT32_MAX);
	XSYMBOL(printf)("hd: %hd %hd\n", INT32_MIN, INT32_MAX);

	printf("d: %d %d\n", INT32_MIN, INT32_MAX);
	XSYMBOL(printf)("d: %d %d\n", INT32_MIN, INT32_MAX);
	printf("d: %d %d\n", INT64_MIN, INT64_MAX);
	XSYMBOL(printf)("d: %d %d\n", INT64_MIN, INT64_MAX);

	printf("ld: %ld %ld\n", INT64_MIN, INT64_MAX);
	XSYMBOL(printf)("ld: %ld %ld\n", INT64_MIN, INT64_MAX);

	printf("lld: %lld %lld\n", INT64L_MIN, INT64L_MAX);
	XSYMBOL(printf)("lld: %lld %lld\n", INT64L_MIN, INT64L_MAX);

	/* unsigned */
	printf("hhu: %hhu\n", UINT8_MAX);
	XSYMBOL(printf)("hhu: %hhu\n", UINT8_MAX);

	printf("hhu: %hhu\n", UINT32_MAX);
	XSYMBOL(printf)("hhu: %hhu\n", UINT32_MAX);

	printf("hu: %hu\n", UINT16_MAX);
	XSYMBOL(printf)("hu: %hu\n", UINT16_MAX);
	printf("hu: %hu\n", UINT32_MAX);
	XSYMBOL(printf)("hu: %hu\n", UINT32_MAX);

	printf("u: %u\n", UINT32_MAX);
	XSYMBOL(printf)("u: %u\n", UINT32_MAX);
	printf("u: %u\n", UINT64_MAX);
	XSYMBOL(printf)("u: %u\n", UINT64_MAX);

	printf("lu: %lu\n", UINT64_MAX);
	XSYMBOL(printf)("lu: %lu\n", UINT64_MAX);

	/* signed */
	printf("hhx: %hhx %hhx\n", INT8_MIN, INT8_MAX);
	XSYMBOL(printf)("hhx: %hhx %hhx\n", INT8_MIN, INT8_MAX);
	printf("hhx: %hhx %hhx\n", INT32_MIN, INT32_MAX);
	XSYMBOL(printf)("hhx: %hhx %hhx\n", INT32_MIN, INT32_MAX);

	printf("hx: %hx %hx\n", INT16_MIN, INT16_MAX);
	XSYMBOL(printf)("hx: %hx %hx\n", INT16_MIN, INT16_MAX);
	printf("hx: %hx %hx\n", INT32_MIN, INT32_MAX);
	XSYMBOL(printf)("hx: %hx %hx\n", INT32_MIN, INT32_MAX);

	printf("x: %x %x\n", INT32_MIN, INT32_MAX);
	XSYMBOL(printf)("x: %x %x\n", INT32_MIN, INT32_MAX);
	printf("x: %x %x\n", INT64_MIN, INT64_MAX);
	XSYMBOL(printf)("x: %x %x\n", INT64_MIN, INT64_MAX);

	printf("lx: %lx %lx\n", INT64_MIN, INT64_MAX);
	XSYMBOL(printf)("lx: %lx %lx\n", INT64_MIN, INT64_MAX);

	printf("llx: %llx %llx\n", INT64L_MIN, INT64L_MAX);
	XSYMBOL(printf)("llx: %llx %llx\n", INT64L_MIN, INT64L_MAX);

	/* unsigned hex */
	printf("llu: %llu\n", UINT64L_MAX);
	XSYMBOL(printf)("llu: %llu\n", UINT64L_MAX);

	printf("hhx: %hhx\n", UINT8_MAX);
	XSYMBOL(printf)("hhx: %hhx\n", UINT8_MAX);

	printf("hhx: %hhx\n", UINT32_MAX);
	XSYMBOL(printf)("hhx: %hhx\n", UINT32_MAX);

	printf("hx: %hx\n", UINT16_MAX);
	XSYMBOL(printf)("hx: %hx\n", UINT16_MAX);
	printf("hx: %hx\n", UINT32_MAX);
	XSYMBOL(printf)("hx: %hx\n", UINT32_MAX);

	printf("x: %x\n", UINT32_MAX);
	XSYMBOL(printf)("x: %x\n", UINT32_MAX);
	printf("x: %x\n", UINT64_MAX);
	XSYMBOL(printf)("x: %x\n", UINT64_MAX);

	printf("lx: %lx\n", UINT64_MAX);
	XSYMBOL(printf)("lx: %lx\n", UINT64_MAX);

	printf("llx: %llx\n", UINT64L_MAX);	
	XSYMBOL(printf)("llx: %llx\n", UINT64L_MAX);

	/* floating */
	printf("f: %.380f\n", XFP_DBL_MAX);
	XSYMBOL(printf)("f: %.380f\n", XFP_DBL_MAX);

	printf("f: %.380f\n", -XFP_DBL_MAX);
	XSYMBOL(printf)("f: %.380f\n", -XFP_DBL_MAX);

	printf("f: %.380f\n", XFP_DBL_MIN);
	XSYMBOL(printf)("f: %.380f\n", XFP_DBL_MIN);

	printf("f: %.380f\n", -XFP_DBL_MIN);
	XSYMBOL(printf)("f: %.380f\n", -XFP_DBL_MIN);

	printf("f: %.380f\n", -XFP_DBL_MIN);
	XSYMBOL(printf)("f: %.380f\n", -XFP_DBL_MIN);

	/* character and string */
	printf("c: %c\n", 'w');
	XSYMBOL(printf)("c: %c\n", 'w');

	printf("c: %9c\n", 'w');
	XSYMBOL(printf)("c: %9c\n", 'w');

	printf("s: %s\n", "Hello, World");
	XSYMBOL(printf)("s: %s\n", "Hello, World");

	printf("s: %19s\n", "Hello, World");
	XSYMBOL(printf)("s: %19s\n", "Hello, World");

	printf("s: %-19s-\n", "Hello, World");
	XSYMBOL(printf)("s: %-19s-\n", "Hello, World");

	printf("s: %33s-\n", "Hello, World");
	XSYMBOL(printf)("s: %33s-\n", "Hello, World");

	printf("s: %33s ω %s-\n", "Hello, World ω", NULL);
	XSYMBOL(printf)("s: %33s ω %s-\n", "Hello, World ω", NULL);

	/* snprintf */
	char buf[128];
	XSYMBOL(memset)(buf, 'A', 128);

	snprintf(buf, 108, "h%99dh ωω\xe3\n", 123);
	printf("%s\n", buf);

	XSYMBOL(snprintf)(buf, 108, "h%99dh ωω\xe3\n", 123);
	printf("%s\n", buf);

	return 0;
}
