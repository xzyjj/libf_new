/* xsort_qsort.c - data table sorting implementations */

#include <libf/config.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/sl/xstdlib.h>


/* @func: _qsort_swap (static) - memory swap
* @param1: void * # member pointer
* @param2: void * # member pointer
* @param3: uint64 # memory swap size
* @return: void
*/
static void _qsort_swap(void *a, void *b, uint64 n) {
	char buf[32];
	for (; n > 32; n -= 32) {
		XSYMBOL(memcpy)(buf, a, 32);
		XSYMBOL(memcpy)(a, b, 32);
		XSYMBOL(memcpy)(b, buf, 32);
		a = (char *)a + 32;
		b = (char *)b + 32;
	}

	if (n > 0) {
		XSYMBOL(memcpy)(buf, a, n);
		XSYMBOL(memcpy)(a, b, n);
		XSYMBOL(memcpy)(b, buf, n);
	}
} /* end */

/* @func: _qsort (static) - heapsort siftdown
* @param1: void * # base array
* @param2: uint64 # number of array members
* @param3: uint64 # members size
* @param4: int32 (*)(const void *, const voie *) # callback cmp
* @param5: uint64 # heapsort binary size
* @return: void
*/
static void _qsort(void *b, uint64 n, uint64 w,
		int32 (*cmp)(const void *, const void *), uint64 k) {
	for (uint64 j = 0; ((k * 2) + 1) <= n; ) {
		j = (2 * k) + 1;
		if (j < n && cmp((char *)b + (j * w),
				(char *)b + ((j + 1) * w)) < 0)
			j++;
		if (j == k || cmp((char *)b + (k * w),
				(char *)b + (j * w)) >= 0)
			break;
		_qsort_swap((char *)b + (w * j), (char *)b + (k * w), w);
		k = j;
	}
} /* end */

/* @func: qsort - sort a table of data (heapsort)
* @param1: void * # base array
* @param2: uint64 # number of array members
* @param3: uint64 # members size
* @param4: int32 (*)(const void *, const voie *) # callback cmp
* @return: void
*/
void XSYMBOL(qsort)(void *b, uint64 n, uint64 w,
		int32 (*cmp)(const void *, const void *)) {
	if (n-- <= 1)
		return;

	/* build binary heap */
	for (int64 k = n / 2; k >= 0; k--)
		_qsort(b, n, w, cmp, k);

	while (1) {
		_qsort_swap(b, (char *)b + (n * w), w);
		if (!--n)
			break;
		_qsort(b, n, w, cmp, 0);
	}
} /* end */

/* @func: bsearch - binary search a sorted table
* @param1: const void * # key value
* @param2: const void * # base sorted array
* @param3: uint64       # number of array members
* @param4: uint64       # members size
* @param5: int32 (*)(const void *, const void *) # callback cmp
* @return: void *       # return the key's members / NULL pointer
*/
void *XSYMBOL(bsearch)(const void *k, const void *b, uint64 n, uint64 w,
		int32 (*cmp)(const void *, const void *)) {
	while (n > 0) {
		char *p = (char *)b + (w * (n / 2));
		int32 m = cmp(k, p);
		if (m < 0) {
			n /= 2;
		} else if (m > 0) {
			b = p + w;
			n -= (n / 2) + 1;
		} else {
			return p;
		}
	}

	return NULL;
} /* end */
