/* xstdlib_random.c - random number generation implementations */

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstdlib.h>


/* @func: random_r - reentrant random number generator
* @param1: struct random_ctx * # random context struct
* @param2: int32 *             # random number result
* @return: int32               # 0: no error, -1: param error
*/
int32 XSYMBOL(random_r)(struct random_ctx *p, int32 *v) {
	if (!(p && v)) {
		/* errno */;
		return -1;
	}

	/* LCG (Linear Congruential Generator) */
	int32 *state = p->state;
	if (p->type == 0) {
		*v = ((state[0] * 1103515245) + 12345) & INT32_MAX;
		state[0] = *v;
		return 0;
	}

	/* LFSR (Linear Feedback Shift Register) */
	int32 *fptr = p->fptr, *bptr = p->bptr, *eptr = p->eptr;
	*v = (*fptr += *bptr) >> 1;
	if (++fptr >= eptr) {
		fptr = state;
		bptr++;
	} else if (++bptr >= eptr) {
		bptr = state;
	}
	p->fptr = fptr;
	p->bptr = bptr;

	return 0;
} /* end */
