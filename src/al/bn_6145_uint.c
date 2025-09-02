/* bn_6145_uint.c - big number calculation (6145-bit) implementations */

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/al/bn_6145.h>


/* @func: bn_u6145_cmp - big number compare
* @param1: const bn_6145_t # number a
* @param2: const bn_6145_t # number b
* @return: int32           # 0: a==b, 1: a>b, -1: a<b
*/
int32 FSYMBOL(bn_u6145_cmp)(const bn_6145_t a, const bn_6145_t b) {
	for (int32 i = BN_6145_SIZE - 1; i >= 0; i--) {
		if (a[i] > b[i])
			return 1;
		if (a[i] < b[i])
			return -1;
	}

	return 0;
} /* end */

/* @func: bn_u6145_cmp1 - big number compare
* @param1: const bn_6145_t # number a
* @param2: uint32          # number b
* @return: int32           # 0: a==b, 1: a>b, -1: a<b
*/
int32 FSYMBOL(bn_u6145_cmp1)(const bn_6145_t a, uint32 b) {
	for (int32 i = BN_6145_SIZE - 1; i >= 0; i--) {
		if (i) {
			if (a[i])
				return 1;
		} else {
			if (a[i] > b)
				return 1;
			if (a[i] < b)
				return -1;
		}
	}

	return 0;
} /* end */

/* @func: bn_u6145_add - big number addition
* @param1: bn_6145_t       # sum
* @param2: const bn_6145_t # addend
* @param3: const bn_6145_t # addend
* @return: void
*/
void FSYMBOL(bn_u6145_add)(bn_6145_t r,
		const bn_6145_t a, const bn_6145_t b) {
	bn_6145_t rr;

	if (!FSYMBOL(bn_6145_bits)(a)) {
		FSYMBOL(bn_6145_move)(r, b);
		return;
	}
	if (!FSYMBOL(bn_6145_bits)(b)) {
		FSYMBOL(bn_6145_move)(r, a);
		return;
	}

	FSYMBOL(bn_6145_init)(rr);

	uint32 carry = 0;
	for (int32 i = 0; i < BN_6145_SIZE; i++) {
		uint64L tmp = (uint64L)a[i] + b[i] + carry;
		rr[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}

	FSYMBOL(bn_6145_move)(r, rr);
} /* end */

/* @func: bn_u6145_sub - big number subtraction
* @param1: bn_6145_t       # difference
* @param2: const bn_6145_t # minuend
* @param3: const bn_6145_t # subtract
* @return: void
*/
void FSYMBOL(bn_u6145_sub)(bn_6145_t r,
		const bn_6145_t a, const bn_6145_t b) {
	bn_6145_t rr;

	if (!FSYMBOL(bn_6145_bits)(a)) {
		FSYMBOL(bn_6145_move)(r, b);
		return;
	}
	if (!FSYMBOL(bn_6145_bits)(b)) {
		FSYMBOL(bn_6145_move)(r, a);
		return;
	}

	FSYMBOL(bn_6145_init)(rr);

	int32 borrow = 0;
	for (int32 i = 0; i < BN_6145_SIZE; i++) {
		uint64L tmp = (uint64L)a[i] - b[i] - borrow;
		rr[i] = tmp & 0xffffffff;
		borrow = (tmp >> 32) ? 1 : 0;
	}

	FSYMBOL(bn_6145_move)(r, rr);
} /* end */

/* @func: bn_u6145_mul - big number multiplication
* @param1: bn_6145_t       # product
* @param2: const bn_6145_t # multiplicand
* @param3: const bn_6145_t # multiplier
* @return: void
*/
void FSYMBOL(bn_u6145_mul)(bn_6145_t r,
		const bn_6145_t a, const bn_6145_t b) {
	bn_6145_t rr;
	int32 asize, bsize;

	asize = FSYMBOL(bn_6145_bits)(a);
	bsize = FSYMBOL(bn_6145_bits)(b);
	if (!asize || !bsize) {
		FSYMBOL(bn_6145_init)(r);
		return;
	}

	FSYMBOL(bn_6145_init)(rr);

	uint32 carry = 0;
	for (int32 i = 0; i < BN_6145_SIZE; i++) {
		carry = 0;
		for (int32 j = 0; j < BN_6145_SIZE; j++) {
			if ((i + j) >= BN_6145_SIZE)
				break;
			uint64L tmp = (uint64L)a[i] * b[j] + rr[i + j]
				+ carry;
			rr[i + j] = tmp & 0xffffffff;
			carry = tmp >> 32;
		}
	}

	FSYMBOL(bn_6145_move)(r, rr);
} /* end */

/* @func: bn_u6145_div - big number division
* @param1: bn_6145_t       # quotient
* @param2: bn_6145_t       # remainder
* @param3: const bn_6145_t # dividend
* @param4: const bn_6145_t # divisor
* @return: void
*/
void FSYMBOL(bn_u6145_div)(bn_6145_t quo, bn_6145_t rem,
		const bn_6145_t a, const bn_6145_t b) {
	bn_6145_t rquo, rrem, sh_b;
	FSYMBOL(bn_6145_move)(rrem, a);
	FSYMBOL(bn_6145_move)(sh_b, b);
	FSYMBOL(bn_6145_move)(rem, rrem);
	FSYMBOL(bn_6145_init)(quo);

	if (FSYMBOL(bn_u6145_cmp)(rrem, sh_b) < 0)
		return;
	if (!(FSYMBOL(bn_6145_bits)(rrem)
			&& FSYMBOL(bn_6145_bits)(sh_b))) {
		FSYMBOL(bn_6145_init)(rem);
		return;
	}

	uint32 sh = FSYMBOL(bn_6145_bits)(rrem)
		- FSYMBOL(bn_6145_bits)(sh_b);

	FSYMBOL(bn_6145_lsh)(sh_b, sh);
	while (FSYMBOL(bn_u6145_cmp)(rrem, sh_b) >= 0) {
		FSYMBOL(bn_6145_lsh)(sh_b, 1);
		sh++;
	}

	FSYMBOL(bn_6145_init)(rquo);
	for (; sh > 0; sh--) {
		FSYMBOL(bn_6145_rsh)(sh_b, 1);
		FSYMBOL(bn_6145_lsh)(rquo, 1);
		if (FSYMBOL(bn_u6145_cmp)(rrem, sh_b) >= 0) {
			FSYMBOL(bn_u6145_sub)(rrem, rrem, sh_b);
			rquo[0] |= 1;
		}
	}

	FSYMBOL(bn_6145_move)(quo, rquo);
	FSYMBOL(bn_6145_move)(rem, rrem);
} /* end */

/* @func: bn_u6145_add1 - big number addition
* @param1: bn_6145_t       # sum
* @param2: const bn_6145_t # addend
* @param3: uint32          # addend
* @return: void
*/
void FSYMBOL(bn_u6145_add1)(bn_6145_t r,
		const bn_6145_t a, uint32 b) {
	bn_6145_t rr;

	if (!b) {
		FSYMBOL(bn_6145_move)(r, a);
		return;
	}
	if (!FSYMBOL(bn_6145_bits)(a)) {
		FSYMBOL(bn_6145_set1)(r, b);
		return;
	}

	FSYMBOL(bn_6145_init)(rr);

	uint32 carry = b;
	for (int32 i = 0; i < BN_6145_SIZE; i++) {
		uint64L tmp = (uint64L)a[i] + carry;
		rr[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}

	FSYMBOL(bn_6145_move)(r, rr);
} /* end */

/* @func: bn_u6145_sub1 - big number subtraction
* @param1: bn_6145_t       # difference
* @param2: const bn_6145_t # minuend
* @param3: uint32          # subtract
* @return: void
*/
void FSYMBOL(bn_u6145_sub1)(bn_6145_t r,
		const bn_6145_t a, uint32 b) {
	bn_6145_t rr;

	if (!b) {
		FSYMBOL(bn_6145_move)(r, a);
		return;
	}
	if (!FSYMBOL(bn_6145_bits)(a)) {
		FSYMBOL(bn_6145_set1)(r, b);
		return;
	}

	FSYMBOL(bn_6145_init)(rr);

	int32 borrow = 0;
	for (int32 i = 0; i < BN_6145_SIZE; i++) {
		uint64L tmp = (uint64L)a[i] - b - borrow;
		rr[i] = tmp & 0xffffffff;
		borrow = (tmp >> 32) ? 1 : 0;
		b = 0;
	}

	FSYMBOL(bn_6145_move)(r, rr);
} /* end */

/* @func: bn_u6145_mul1 - big number multiplication
* @param1: bn_6145_t       # product
* @param2: const bn_6145_t # multiplicand
* @param3: uint32          # multiplier
* @return: void
*/
void FSYMBOL(bn_u6145_mul1)(bn_6145_t r,
		const bn_6145_t a, uint32 b) {
	bn_6145_t rr;

	if (!b || !FSYMBOL(bn_6145_bits)(a)) {
		FSYMBOL(bn_6145_init)(r);
		return;
	}

	FSYMBOL(bn_6145_init)(rr);

	uint32 carry = 0;
	for (int32 i = 0; i < BN_6145_SIZE; i++) {
		uint64L tmp = (uint64L)a[i] * b + carry;
		rr[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}

	FSYMBOL(bn_6145_move)(r, rr);
} /* end */

/* @func: bn_u6145_div1 - big number division
* @param1: bn_6145_t       # quotient
* @param2: bn_6145_t       # remainder
* @param3: const bn_6145_t # dividend
* @param4: uint32          # divisor
* @return: void
*/
void FSYMBOL(bn_u6145_div1)(bn_6145_t quo, bn_6145_t rem,
		const bn_6145_t a, uint32 b) {
	bn_6145_t tmp;
	FSYMBOL(bn_6145_set1)(tmp, b);

	FSYMBOL(bn_u6145_div)(quo, rem, a, tmp);
} /* end */
