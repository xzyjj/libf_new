/* bn_u8192.c - big number calculation (8192bit) implementations */

#include <libf/config.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/al/bn_8192.h>


/* @func: bn_uint8192_zero - big number init
* @param1: bn_uint8192_t # target
* @return:
*/
void FSYMBOL(bn_uint8192_zero)(bn_uint8192_t t) {
	XSYMBOL(memset)(t, 0, sizeof(uint32) * (BN_8192_SIZE + 2));
} /* end */

/* @func: bn_uint8192_move - big number move
* @param1: bn_uint8192_t # target
* @param2: bn_uint8192_t # source
* @return: void
*/
void FSYMBOL(bn_uint8192_move)(bn_uint8192_t t, const bn_uint8192_t s) {
	XSYMBOL(memcpy)(t, s, sizeof(uint32) * (BN_8192_SIZE + 1));
} /* end */

/* @func: bn_uint8192_cmp - big number compare
* @param1: bn_uint8192_t # number (a)
* @param2: bn_uint8192_t # number (b)
* @return: int32         # 0: a==b, 1: a>b, -1: a<b
*/
int32 FSYMBOL(bn_uint8192_cmp)(const bn_uint8192_t a, const bn_uint8192_t b) {
	for (int32 i = BN_8192_SIZE - 1; i >= 0; i--) {
		if (a[i] > b[i])
			return 1;
		if (a[i] < b[i])
			return -1;
	}

	return 0;
} /* end */

/* @func: bn_uint8193_cmp - big number compare
* @param1: bn_uint8192_t # number (a)
* @param2: bn_uint8192_t # number (b)
* @return: int32         # 0: a==b, 1: a>b, -1: a<b
*/
int32 FSYMBOL(bn_uint8193_cmp)(const bn_uint8192_t a, const bn_uint8192_t b) {
	for (int32 i = BN_8192_SIZE; i >= 0; i--) {
		if (a[i] > b[i])
			return 1;
		if (a[i] < b[i])
			return -1;
	}

	return 0;
} /* end */

/* @func: bn_uint8192_cmp_1 - big number compare
* @param1: bn_uint8192_t # number (a)
* @param2: uint32        # number (b)
* @return: int32         # 0: a==b, 1: a>b, -1: a<b
*/
int32 FSYMBOL(bn_uint8192_cmp_1)(const bn_uint8192_t a, uint32 b) {
	for (int32 i = BN_8192_SIZE - 1; i >= 0; i--) {
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

/* @func: bn_uint8192_lsh - shift big numbers left 1bit
* @param1: bn_uint8192_t # number
* @return: void
*/
void FSYMBOL(bn_uint8192_lsh)(bn_uint8192_t n) {
	uint32 carry = 0;
	for (int32 i = 0; i < BN_8192_SIZE; i++) {
		uint32 tmp = n[i] >> 31;
		n[i] = (n[i] << 1) | carry;
		carry = tmp;
	}
} /* end */

/* @func: bn_uint8193_lsh - shift big numbers left 1bit
* @param1: bn_uint8192_t # number
* @return: void
*/
void FSYMBOL(bn_uint8193_lsh)(bn_uint8192_t n) {
	uint32 carry = 0;
	for (int32 i = 0; i < (BN_8192_SIZE + 1); i++) {
		uint32 tmp = n[i] >> 31;
		n[i] = (n[i] << 1) | carry;
		carry = tmp;
	}
} /* end */

/* @func: bn_uint8192_rsh - shift big numbers right 1bit
* @param1: bn_uint8192_t # number
* @return: void
*/
void FSYMBOL(bn_uint8192_rsh)(bn_uint8192_t n) {
	uint32 carry = 0;
	for (int32 i = BN_8192_SIZE - 1; i >= 0; i--) {
		uint32 tmp = n[i] & 1;
		n[i] = (n[i] >> 1) | (carry << 31);
		carry = tmp;
	}
} /* end */

/* @func: bn_uint8193_rsh - shift big numbers right 1bit
* @param1: bn_uint8192_t # number
* @return: void
*/
void FSYMBOL(bn_uint8193_rsh)(bn_uint8192_t n) {
	uint32 carry = 0;
	for (int32 i = BN_8192_SIZE; i >= 0; i--) {
		uint32 tmp = n[i] & 1;
		n[i] = (n[i] >> 1) | (carry << 31);
		carry = tmp;
	}
} /* end */

/* @func: bn_uint8192_and - big number and operation
* @param1: bn_uint8192_t       # result
* @param2: const bn_uint8192_t # number
* @param3: const bn_uint8192_t # number
* @return: void
*/
void FSYMBOL(bn_uint8192_and)(bn_uint8192_t r, bn_uint8192_t a, bn_uint8192_t b) {
	bn_uint8192_t rr;
	FSYMBOL(bn_uint8192_zero)(rr);

	for (int32 i = 0; i < BN_8192_SIZE; i++) {
		rr[i] = a[i] & b[i];
	}

	FSYMBOL(bn_uint8192_move)(r, rr);
} /* end */

/* @func: bn_uint8192_or - big number or operation
* @param1: bn_uint8192_t       # result
* @param2: const bn_uint8192_t # number
* @param3: const bn_uint8192_t # number
* @return: void
*/
void FSYMBOL(bn_uint8192_or)(bn_uint8192_t r, bn_uint8192_t a, bn_uint8192_t b) {
	bn_uint8192_t rr;
	FSYMBOL(bn_uint8192_zero)(rr);

	for (int32 i = 0; i < BN_8192_SIZE; i++) {
		rr[i] = a[i] | b[i];
	}

	FSYMBOL(bn_uint8192_move)(r, rr);
} /* end */

/* @func: bn_uint8192_xor - big number xor operation
* @param1: bn_uint8192_t       # result
* @param2: const bn_uint8192_t # number
* @param3: const bn_uint8192_t # number
* @return: void
*/
void FSYMBOL(bn_uint8192_xor)(bn_uint8192_t r, bn_uint8192_t a, bn_uint8192_t b) {
	bn_uint8192_t rr;
	FSYMBOL(bn_uint8192_zero)(rr);

	for (int32 i = 0; i < BN_8192_SIZE; i++) {
		rr[i] = a[i] ^ b[i];
	}

	FSYMBOL(bn_uint8192_move)(r, rr);
} /* end */

/* @func: bn_uint8192_not - big number not operation
* @param1: bn_uint8192_t       # result
* @param2: const bn_uint8192_t # number
* @return: void
*/
void FSYMBOL(bn_uint8192_not)(bn_uint8192_t r, bn_uint8192_t n) {
	bn_uint8192_t rr;
	FSYMBOL(bn_uint8192_zero)(rr);

	for (int32 i = 0; i < BN_8192_SIZE; i++) {
		rr[i] = ~n[i];
	}

	FSYMBOL(bn_uint8192_move)(r, rr);
} /* end */

/* @func: bn_uint8192_add - big number addition
* @param1: bn_uint8192_t       # sum
* @param2: const bn_uint8192_t # addend
* @param3: const bn_uint8192_t # addend
* @return: uint32              # carry overflow
*/
uint32 FSYMBOL(bn_uint8192_add)(bn_uint8192_t r, const bn_uint8192_t a,
		const bn_uint8192_t b) {
	bn_uint8192_t rr;
	FSYMBOL(bn_uint8192_move)(rr, a);

	if (!FSYMBOL(bn_uint8192_cmp_1)(b, 0)) {
		FSYMBOL(bn_uint8192_move)(r, rr);
		return 0;
	}

	uint64L carry = 0;
	for (int32 i = 0; i < BN_8192_SIZE; i++) {
		uint64L tmp = (uint64L)rr[i] + b[i] + carry;
		rr[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}

	FSYMBOL(bn_uint8192_move)(r, rr);

	return carry;
} /* end */

/* @func: bn_uint8192_add_1 - big number addition
* @param1: bn_uint8192_t       # sum
* @param2: const bn_uint8192_t # addend
* @param3: uint32              # addend
* @return: uint32              # carry overflow
*/
uint32 FSYMBOL(bn_uint8192_add_1)(bn_uint8192_t r, const bn_uint8192_t a, uint32 b) {
	bn_uint8192_t rr;
	FSYMBOL(bn_uint8192_move)(rr, a);

	if (!b) {
		FSYMBOL(bn_uint8192_move)(r, rr);
		return 0;
	}

	uint64L carry = 0;
	for (int32 i = 0; i < BN_8192_SIZE; i++) {
		uint64L tmp = (uint64L)rr[i] + b + carry;
		rr[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
		b = 0;
	}

	FSYMBOL(bn_uint8192_move)(r, rr);

	return carry;
} /* end */

/* @func: bn_uint8192_sub - big number subtraction
* @param1: bn_uint8192_t       # difference
* @param2: const bn_uint8192_t # minuend
* @param3: const bn_uint8192_t # subtract
* @return: void
*/
void FSYMBOL(bn_uint8192_sub)(bn_uint8192_t r, const bn_uint8192_t a,
		const bn_uint8192_t b) {
	bn_uint8192_t rr;
	FSYMBOL(bn_uint8192_move)(rr, a);

	if (!(FSYMBOL(bn_uint8192_cmp_1)(a, 0)
			&& FSYMBOL(bn_uint8192_cmp_1)(b, 0))) {
		FSYMBOL(bn_uint8192_move)(r, rr);
		return;
	}

	int32 borrow = 0;
	for (int32 i = 0; i < BN_8192_SIZE; i++) {
		uint64L tmp = (uint64L)rr[i] - b[i] - borrow;
		rr[i] = tmp & 0xffffffff;
		borrow = (tmp >> 32) ? 1 : 0;
	}

	FSYMBOL(bn_uint8192_move)(r, rr);
} /* end */

/* @func: bn_uint8192_sub_1 - big number subtraction
* @param1: bn_uint8192_t       # difference
* @param2: const bn_uint8192_t # minuend
* @param3: uint32              # subtract
* @return: void
*/
void FSYMBOL(bn_uint8192_sub_1)(bn_uint8192_t r, const bn_uint8192_t a, uint32 b) {
	bn_uint8192_t rr;
	FSYMBOL(bn_uint8192_move)(rr, a);

	if (!(FSYMBOL(bn_uint8192_cmp_1)(a, 0) && b)) {
		FSYMBOL(bn_uint8192_move)(r, rr);
		return;
	}

	int32 borrow = 0;
	for (int32 i = 0; i < BN_8192_SIZE; i++) {
		uint64L tmp = (uint64L)rr[i] - b - borrow;
		rr[i] = tmp & 0xffffffff;
		borrow = (tmp >> 32) ? 1 : 0;
		b = 0;
	}

	FSYMBOL(bn_uint8192_move)(r, rr);
} /* end */

/* @func: bn_uint8192_mul - big number multiplication
* @param1: bn_uint8192_t       # product
* @param3: const bn_uint8192_t # multiplicand
* @param2: const bn_uint8192_t # multiplier
* @return: uint32              # carry overflow
*/
uint32 FSYMBOL(bn_uint8192_mul)(bn_uint8192_t r, const bn_uint8192_t a,
		const bn_uint8192_t b) {
	bn_uint8192_t rr;
	FSYMBOL(bn_uint8192_zero)(rr);

	if (!(FSYMBOL(bn_uint8192_cmp_1)(a, 0)
			&& FSYMBOL(bn_uint8192_cmp_1)(b, 0))) {
		FSYMBOL(bn_uint8192_zero)(r);
		return 0;
	}

	uint32 carry = 0;
	for (int32 i = 0; i < BN_8192_SIZE; i++) {
		carry = 0;
		for (int32 j = 0; j < BN_8192_SIZE; j++) {
			if ((i + j) >= BN_8192_SIZE)
				break;
			uint64L tmp = (uint64L)a[i] * b[j] + rr[i + j] + carry;
			rr[i + j] = tmp & 0xffffffff;
			carry = tmp >> 32;
		}
	}

	FSYMBOL(bn_uint8192_move)(r, rr);

	return carry;
} /* end */

/* @func: bn_uint8192_mul_1 - big number multiplication
* @param1: bn_uint8192_t       # product
* @param3: const bn_uint8192_t # multiplicand
* @param2: uint32              # multiplier
* @return: uint32              # carry overflow
*/
uint32 FSYMBOL(bn_uint8192_mul_1)(bn_uint8192_t r, const bn_uint8192_t a, uint32 b) {
	bn_uint8192_t rr;
	FSYMBOL(bn_uint8192_zero)(rr);

	if (!(FSYMBOL(bn_uint8192_cmp_1)(a, 0) && b)) {
		FSYMBOL(bn_uint8192_zero)(r);
		return 0;
	}

	uint32 carry = 0;
	for (int32 i = 0; i < BN_8192_SIZE; i++) {
		uint64L tmp = (uint64L)a[i] * b + carry;
		rr[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}

	FSYMBOL(bn_uint8192_move)(r, rr);

	return carry;
} /* end */

/* @func: bn_uint8192_div - big number division
* @param1: bn_uint8192_t       # quotient
* @param2: bn_uint8192_t       # remainder
* @param3: const bn_uint8192_t # dividend
* @param4: const bn_uint8192_t # divisor
* @return: void
*/
void FSYMBOL(bn_uint8192_div)(bn_uint8192_t quo, bn_uint8192_t rem,
		const bn_uint8192_t a, const bn_uint8192_t b) {
	bn_uint8192_t rquo, rrem, sh_b;
	FSYMBOL(bn_uint8192_move)(rrem, a);
	FSYMBOL(bn_uint8192_move)(sh_b, b);
	FSYMBOL(bn_uint8192_move)(rem, rrem);
	FSYMBOL(bn_uint8192_zero)(quo);

	if (FSYMBOL(bn_uint8192_cmp)(rrem, sh_b) < 0)
		return;
	if (!(FSYMBOL(bn_uint8192_cmp_1)(rrem, 0)
			&& FSYMBOL(bn_uint8192_cmp_1)(sh_b, 0))) {
		FSYMBOL(bn_uint8192_zero)(rem);
		return;
	}

	int32 sh = 0;
	while (FSYMBOL(bn_uint8193_cmp)(rrem, sh_b) >= 0) {
		FSYMBOL(bn_uint8193_lsh)(sh_b);
		sh++;
	}

	FSYMBOL(bn_uint8192_zero)(rquo);
	for (; sh > 0; sh--) {
		FSYMBOL(bn_uint8193_rsh)(sh_b);
		FSYMBOL(bn_uint8193_lsh)(rquo);
		if (FSYMBOL(bn_uint8193_cmp)(rrem, sh_b) >= 0) {
			FSYMBOL(bn_uint8192_sub)(rrem, rrem, sh_b);
			rquo[0] |= 1;
		}
	}

	FSYMBOL(bn_uint8192_move)(quo, rquo);
	FSYMBOL(bn_uint8192_move)(rem, rrem);
} /* end */

/* @func: bn_uint8192_strtonum - string convert big number
* @param1: bn_uint8192_t # number
* @param2: char **       # end pointer
* @param3: int32         # base type
* @return: void
*/
void FSYMBOL(bn_uint8192_strtonum)(bn_uint8192_t t, const char *s, char **e,
		int32 b) {
	for (; *s == ' '; s++);

	if (b) {
		if (*s == '0') {
			s++;
			if (*s == 'B' || *s == 'b') {
				s++;
			} else if (*s == 'X' || *s == 'x') {
				s++;
			}
		}
	} else if (*s == '0') { /* b == 0 */
		s++;
		if (*s == 'B' || *s == 'b') {
			b = 2;
			s++;
		} else if (*s == 'X' || *s == 'x') {
			b = 16;
			s++;
		} else if (*s >= '0' && *s <= '7') {
			b = 8;
		} else {
			b = 10;
		}
	} else { /* default */
		b = 10;
	}

	for (; *s == '0'; s++);

	FSYMBOL(bn_uint8192_zero)(t);
	for (int32 d = 0; *s != '\0'; s++) {
		if (*s >= '0' && *s <= '9') {
			d = (*s - '0');
		} else if (*s >= 'A' && *s <= 'Z') {
			d = (*s - 'A') + 10;
		} else if (*s >= 'a' && *s <= 'z') {
			d = (*s - 'a') + 10;
		} else {
			break;
		}
		if (d >= b)
			break;

		FSYMBOL(bn_uint8192_mul_1)(t, t, b);
		FSYMBOL(bn_uint8192_add_1)(t, t, d);
	}

	if (e) /* end position */
		*e = (char *)s;
} /* end */

/* @func: _out_decimal (static) - unsigned int to decimal
* @param1: int32   # size offset
* @param2: char *  # buffer pointer
* @param3: uint64L # unsigned int value
* @return: int32   # string length (+offset)
*/
static int32 _out_decimal(int32 n, char *p, uint32 v) {
	if (v >= 10)
		n = _out_decimal(n, p, v / 10);
	p[n] = (v % 10) + '0';

	return ++n;
} /* end */

/* @func: bn_uint8192_numtostr - big number to string
* @param1: char *              # string buffer
* @param2: const bn_uint8192_t # number
* @return: void
*/
void FSYMBOL(bn_uint8192_numtostr)(char *buf, const bn_uint8192_t n) {
	bn_uint8192_t quo, rem, b;
	FSYMBOL(bn_uint8192_move)(quo, n);
	FSYMBOL(bn_uint8192_zero)(b);
	b[0] = 10;

	int32 pos = 0;
	do {
		FSYMBOL(bn_uint8192_div)(quo, rem, quo, b);
		int32 k = pos;
		pos = _out_decimal(pos, buf, rem[0]);
		for (int32 i = k, j = pos - 1; i < j; i++, j--) {
			char t = buf[i];
			buf[i] = buf[j];
			buf[j] = t;
		}
	} while (FSYMBOL(bn_uint8192_cmp_1)(quo, 0));
	buf[pos] = '\0';

	for (int32 i = 0, j = pos - 1; i < j; i++, j--) {
		char t = buf[i];
		buf[i] = buf[j];
		buf[j] = t;
	}
} /* end */

/* @func: _out_hex (static) - unsigned int to hexadecimal
* @param1: int32   # size offset
* @param2: char *  # buffer pointer
* @param3: uint64L # unsigned int value
* @return: int32   # string length (+offset)
*/
static int32 _out_hex(int32 n, char *p, uint64L v) {
	if (v >= 16)
		n = _out_hex(n, p, v / 16);
	v %= 16;
	p[n] = (v <= 9) ? (v + '0') : (v + 'a' - 10);

	return ++n;
} /* end */

/* @func: bn_uint8192_numtostr_hex - big number to string (hexadecimal)
* @param1: char *              # string buffer
* @param2: const bn_uint8192_t # number
* @return: void
*/
void FSYMBOL(bn_uint8192_numtostr_hex)(char *buf, const bn_uint8192_t n) {
	bn_uint8192_t quo, rem, b;
	FSYMBOL(bn_uint8192_move)(quo, n);
	FSYMBOL(bn_uint8192_zero)(b);
	b[0] = 16;

	int32 pos = 0;
	do {
		FSYMBOL(bn_uint8192_div)(quo, rem, quo, b);
		int32 k = pos;
		pos = _out_hex(pos, buf, rem[0]);
		for (int32 i = k, j = pos - 1; i < j; i++, j--) {
			char t = buf[i];
			buf[i] = buf[j];
			buf[j] = t;
		}
	} while (FSYMBOL(bn_uint8192_cmp_1)(quo, 0));
	buf[pos] = '\0';

	for (int32 i = 0, j = pos - 1; i < j; i++, j--) {
		char t = buf[i];
		buf[i] = buf[j];
		buf[j] = t;
	}
} /* end */
