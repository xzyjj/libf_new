/* bn_u4096.c - big number calculation (4096bit) implementations */

#include <libf/config.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/al/bn_4096.h>


/* @func: bn_uint4096_zero - big number init
* @param1: bn_uint4096_t # target
* @return:
*/
void FSYMBOL(bn_uint4096_zero)(bn_uint4096_t t) {
	XSYMBOL(memset)(t, 0, sizeof(uint32) * (BN_4096_SIZE + 2));
} /* end */

/* @func: bn_uint4096_bits - get the bit length of big number
* @param1: bn_uint4096_t # number
* @return: uint32        # bits length
*/
uint32 FSYMBOL(bn_uint4096_bits)(const bn_uint4096_t n) {
	static const uint32 pow2[32] = {
		0x00000001, 0x00000002, 0x00000004,
		0x00000008, 0x00000010, 0x00000020,
		0x00000040, 0x00000080, 0x00000100,
		0x00000200, 0x00000400, 0x00000800,
		0x00001000, 0x00002000, 0x00004000,
		0x00008000, 0x00010000, 0x00020000,
		0x00040000, 0x00080000, 0x00100000,
		0x00200000, 0x00400000, 0x00800000,
		0x01000000, 0x02000000, 0x04000000,
		0x08000000, 0x10000000, 0x20000000,
		0x40000000, 0x80000000
		};

	for (int32 i = BN_4096_SIZE - 1; i >= 0; i--) {
		if (n[i]) {
			for (int32 k = 31; k >= 0; k--) {
				if (n[i] & pow2[k])
					return (i * 32) + k;
			}
		}
	}

	return 0;
} /* end */

/* @func: bn_uint4096_move - big number move
* @param1: bn_uint4096_t # target
* @param2: bn_uint4096_t # source
* @return: void
*/
void FSYMBOL(bn_uint4096_move)(bn_uint4096_t t, const bn_uint4096_t s) {
	XSYMBOL(memcpy)(t, s, sizeof(uint32) * BN_4096_SIZE);
	t[BN_4096_SIZE] = 0;
} /* end */

/* @func: bn_uint4096_cmp - big number compare
* @param1: bn_uint4096_t # number (a)
* @param2: bn_uint4096_t # number (b)
* @return: int32         # 0: a==b, 1: a>b, -1: a<b
*/
int32 FSYMBOL(bn_uint4096_cmp)(const bn_uint4096_t a, const bn_uint4096_t b) {
	for (int32 i = BN_4096_SIZE - 1; i >= 0; i--) {
		if (a[i] > b[i])
			return 1;
		if (a[i] < b[i])
			return -1;
	}

	return 0;
} /* end */

/* @func: bn_uint4097_cmp - big number compare
* @param1: bn_uint4096_t # number (a)
* @param2: bn_uint4096_t # number (b)
* @return: int32         # 0: a==b, 1: a>b, -1: a<b
*/
int32 FSYMBOL(bn_uint4097_cmp)(const bn_uint4096_t a, const bn_uint4096_t b) {
	for (int32 i = BN_4096_SIZE; i >= 0; i--) {
		if (a[i] > b[i])
			return 1;
		if (a[i] < b[i])
			return -1;
	}

	return 0;
} /* end */

/* @func: bn_uint4096_cmp_1 - big number compare
* @param1: bn_uint4096_t # number (a)
* @param2: uint32        # number (b)
* @return: int32         # 0: a==b, 1: a>b, -1: a<b
*/
int32 FSYMBOL(bn_uint4096_cmp_1)(const bn_uint4096_t a, uint32 b) {
	for (int32 i = BN_4096_SIZE - 1; i >= 0; i--) {
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

/* @func: bn_uint4096_lsh - shift big numbers left 1bit
* @param1: bn_uint4096_t # number
* @return: void
*/
void FSYMBOL(bn_uint4096_lsh)(bn_uint4096_t n) {
	uint32 carry = 0;
	for (int32 i = 0; i < BN_4096_SIZE; i++) {
		uint32 tmp = n[i] >> 31;
		n[i] = (n[i] << 1) | carry;
		carry = tmp;
	}
} /* end */

/* @func: bn_uint4097_lsh - shift big numbers left 1bit
* @param1: bn_uint4096_t # number
* @return: void
*/
void FSYMBOL(bn_uint4097_lsh)(bn_uint4096_t n) {
	uint32 carry = 0;
	for (int32 i = 0; i < (BN_4096_SIZE + 1); i++) {
		uint32 tmp = n[i] >> 31;
		n[i] = (n[i] << 1) | carry;
		carry = tmp;
	}
} /* end */

/* @func: bn_uint4096_rsh - shift big numbers right 1bit
* @param1: bn_uint4096_t # number
* @return: void
*/
void FSYMBOL(bn_uint4096_rsh)(bn_uint4096_t n) {
	uint32 carry = 0;
	for (int32 i = BN_4096_SIZE - 1; i >= 0; i--) {
		uint32 tmp = n[i] & 1;
		n[i] = (n[i] >> 1) | (carry << 31);
		carry = tmp;
	}
} /* end */

/* @func: bn_uint4097_rsh - shift big numbers right 1bit
* @param1: bn_uint4096_t # number
* @return: void
*/
void FSYMBOL(bn_uint4097_rsh)(bn_uint4096_t n) {
	uint32 carry = 0;
	for (int32 i = BN_4096_SIZE; i >= 0; i--) {
		uint32 tmp = n[i] & 1;
		n[i] = (n[i] >> 1) | (carry << 31);
		carry = tmp;
	}
} /* end */

/* @func: bn_uint4096_and - big number and operation
* @param1: bn_uint4096_t       # result
* @param2: const bn_uint4096_t # number
* @param3: const bn_uint4096_t # number
* @return: void
*/
void FSYMBOL(bn_uint4096_and)(bn_uint4096_t r,
		const bn_uint4096_t a, const bn_uint4096_t b) {
	bn_uint4096_t rr;
	FSYMBOL(bn_uint4096_zero)(rr);

	for (int32 i = 0; i < BN_4096_SIZE; i++) {
		rr[i] = a[i] & b[i];
	}

	FSYMBOL(bn_uint4096_move)(r, rr);
} /* end */

/* @func: bn_uint4096_or - big number or operation
* @param1: bn_uint4096_t       # result
* @param2: const bn_uint4096_t # number
* @param3: const bn_uint4096_t # number
* @return: void
*/
void FSYMBOL(bn_uint4096_or)(bn_uint4096_t r,
		const bn_uint4096_t a, const bn_uint4096_t b) {
	bn_uint4096_t rr;
	FSYMBOL(bn_uint4096_zero)(rr);

	for (int32 i = 0; i < BN_4096_SIZE; i++) {
		rr[i] = a[i] | b[i];
	}

	FSYMBOL(bn_uint4096_move)(r, rr);
} /* end */

/* @func: bn_uint4096_xor - big number xor operation
* @param1: bn_uint4096_t       # result
* @param2: const bn_uint4096_t # number
* @param3: const bn_uint4096_t # number
* @return: void
*/
void FSYMBOL(bn_uint4096_xor)(bn_uint4096_t r,
		const bn_uint4096_t a, const bn_uint4096_t b) {
	bn_uint4096_t rr;
	FSYMBOL(bn_uint4096_zero)(rr);

	for (int32 i = 0; i < BN_4096_SIZE; i++) {
		rr[i] = a[i] ^ b[i];
	}

	FSYMBOL(bn_uint4096_move)(r, rr);
} /* end */

/* @func: bn_uint4096_not - big number not operation
* @param1: bn_uint4096_t       # result
* @param2: const bn_uint4096_t # number
* @return: void
*/
void FSYMBOL(bn_uint4096_not)(bn_uint4096_t r,
		const bn_uint4096_t n) {
	bn_uint4096_t rr;
	FSYMBOL(bn_uint4096_zero)(rr);

	for (int32 i = 0; i < BN_4096_SIZE; i++) {
		rr[i] = ~n[i];
	}

	FSYMBOL(bn_uint4096_move)(r, rr);
} /* end */

/* @func: bn_uint4096_add - big number addition
* @param1: bn_uint4096_t       # sum
* @param2: const bn_uint4096_t # addend
* @param3: const bn_uint4096_t # addend
* @return: uint32              # carry overflow
*/
uint32 FSYMBOL(bn_uint4096_add)(bn_uint4096_t r,
		const bn_uint4096_t a, const bn_uint4096_t b) {
	bn_uint4096_t rr;
	FSYMBOL(bn_uint4096_move)(rr, a);

	if (!FSYMBOL(bn_uint4096_cmp_1)(b, 0)) {
		FSYMBOL(bn_uint4096_move)(r, rr);
		return 0;
	}

	uint64L carry = 0;
	for (int32 i = 0; i < BN_4096_SIZE; i++) {
		uint64L tmp = (uint64L)rr[i] + b[i] + carry;
		rr[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}

	FSYMBOL(bn_uint4096_move)(r, rr);

	return carry;
} /* end */

/* @func: bn_uint4096_add_1 - big number addition
* @param1: bn_uint4096_t       # sum
* @param2: const bn_uint4096_t # addend
* @param3: uint32              # addend
* @return: uint32              # carry overflow
*/
uint32 FSYMBOL(bn_uint4096_add_1)(bn_uint4096_t r,
		const bn_uint4096_t a, uint32 b) {
	bn_uint4096_t rr;
	FSYMBOL(bn_uint4096_move)(rr, a);

	if (!b) {
		FSYMBOL(bn_uint4096_move)(r, rr);
		return 0;
	}

	uint64L carry = 0;
	for (int32 i = 0; i < BN_4096_SIZE; i++) {
		uint64L tmp = (uint64L)rr[i] + b + carry;
		rr[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
		b = 0;
	}

	FSYMBOL(bn_uint4096_move)(r, rr);

	return carry;
} /* end */

/* @func: bn_uint4096_sub - big number subtraction
* @param1: bn_uint4096_t       # difference
* @param2: const bn_uint4096_t # minuend
* @param3: const bn_uint4096_t # subtract
* @return: void
*/
void FSYMBOL(bn_uint4096_sub)(bn_uint4096_t r,
		const bn_uint4096_t a, const bn_uint4096_t b) {
	bn_uint4096_t rr;
	FSYMBOL(bn_uint4096_move)(rr, a);

	if (!(FSYMBOL(bn_uint4096_cmp_1)(a, 0)
			&& FSYMBOL(bn_uint4096_cmp_1)(b, 0))) {
		FSYMBOL(bn_uint4096_move)(r, rr);
		return;
	}

	int32 borrow = 0;
	for (int32 i = 0; i < BN_4096_SIZE; i++) {
		uint64L tmp = (uint64L)rr[i] - b[i] - borrow;
		rr[i] = tmp & 0xffffffff;
		borrow = (tmp >> 32) ? 1 : 0;
	}

	FSYMBOL(bn_uint4096_move)(r, rr);
} /* end */

/* @func: bn_uint4096_sub_1 - big number subtraction
* @param1: bn_uint4096_t       # difference
* @param2: const bn_uint4096_t # minuend
* @param3: uint32              # subtract
* @return: void
*/
void FSYMBOL(bn_uint4096_sub_1)(bn_uint4096_t r,
		const bn_uint4096_t a, uint32 b) {
	bn_uint4096_t rr;
	FSYMBOL(bn_uint4096_move)(rr, a);

	if (!(FSYMBOL(bn_uint4096_cmp_1)(a, 0) && b)) {
		FSYMBOL(bn_uint4096_move)(r, rr);
		return;
	}

	int32 borrow = 0;
	for (int32 i = 0; i < BN_4096_SIZE; i++) {
		uint64L tmp = (uint64L)rr[i] - b - borrow;
		rr[i] = tmp & 0xffffffff;
		borrow = (tmp >> 32) ? 1 : 0;
		b = 0;
	}

	FSYMBOL(bn_uint4096_move)(r, rr);
} /* end */

/* @func: bn_uint4096_mul - big number multiplication
* @param1: bn_uint4096_t       # product
* @param3: const bn_uint4096_t # multiplicand
* @param2: const bn_uint4096_t # multiplier
* @return: uint32              # carry overflow
*/
uint32 FSYMBOL(bn_uint4096_mul)(bn_uint4096_t r,
		const bn_uint4096_t a, const bn_uint4096_t b) {
	bn_uint4096_t rr;
	FSYMBOL(bn_uint4096_zero)(rr);

	if (!(FSYMBOL(bn_uint4096_cmp_1)(a, 0)
			&& FSYMBOL(bn_uint4096_cmp_1)(b, 0))) {
		FSYMBOL(bn_uint4096_zero)(r);
		return 0;
	}

	uint32 carry = 0;
	for (int32 i = 0; i < BN_4096_SIZE; i++) {
		carry = 0;
		for (int32 j = 0; j < BN_4096_SIZE; j++) {
			if ((i + j) >= BN_4096_SIZE)
				break;
			uint64L tmp = (uint64L)a[i] * b[j] + rr[i + j] + carry;
			rr[i + j] = tmp & 0xffffffff;
			carry = tmp >> 32;
		}
	}

	FSYMBOL(bn_uint4096_move)(r, rr);

	return carry;
} /* end */

/* @func: bn_uint4096_mul_1 - big number multiplication
* @param1: bn_uint4096_t       # product
* @param3: const bn_uint4096_t # multiplicand
* @param2: uint32              # multiplier
* @return: uint32              # carry overflow
*/
uint32 FSYMBOL(bn_uint4096_mul_1)(bn_uint4096_t r,
		const bn_uint4096_t a, uint32 b) {
	bn_uint4096_t rr;
	FSYMBOL(bn_uint4096_zero)(rr);

	if (!(FSYMBOL(bn_uint4096_cmp_1)(a, 0) && b)) {
		FSYMBOL(bn_uint4096_zero)(r);
		return 0;
	}

	uint32 carry = 0;
	for (int32 i = 0; i < BN_4096_SIZE; i++) {
		uint64L tmp = (uint64L)a[i] * b + carry;
		rr[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}

	FSYMBOL(bn_uint4096_move)(r, rr);

	return carry;
} /* end */

/* @func: bn_uint4096_div - big number division
* @param1: bn_uint4096_t       # quotient
* @param2: bn_uint4096_t       # remainder
* @param3: const bn_uint4096_t # dividend
* @param4: const bn_uint4096_t # divisor
* @return: void
*/
void FSYMBOL(bn_uint4096_div)(bn_uint4096_t quo, bn_uint4096_t rem,
		const bn_uint4096_t a, const bn_uint4096_t b) {
	bn_uint4096_t rquo, rrem, sh_b, pow2;
	FSYMBOL(bn_uint4096_move)(rrem, a);
	FSYMBOL(bn_uint4096_move)(sh_b, b);
	FSYMBOL(bn_uint4096_move)(rem, rrem);
	FSYMBOL(bn_uint4096_zero)(quo);

	if (FSYMBOL(bn_uint4096_cmp)(rrem, sh_b) < 0)
		return;
	if (!(FSYMBOL(bn_uint4096_cmp_1)(rrem, 0)
			&& FSYMBOL(bn_uint4096_cmp_1)(sh_b, 0))) {
		FSYMBOL(bn_uint4096_zero)(rem);
		return;
	}

	uint32 sh = 0;
	uint32 psh = FSYMBOL(bn_uint4096_bits)(rrem)
		- FSYMBOL(bn_uint4096_bits)(sh_b);
	if (psh > 15) {
		FSYMBOL(bn_uint4096_zero)(pow2);
		pow2[psh / 32] = 1U << (psh % 32);
		FSYMBOL(bn_uint4096_mul)(sh_b, sh_b, pow2);
		sh = psh;
	}

	while (FSYMBOL(bn_uint4097_cmp)(rrem, sh_b) >= 0) {
		FSYMBOL(bn_uint4097_lsh)(sh_b);
		sh++;
	}

	FSYMBOL(bn_uint4096_zero)(rquo);
	for (; sh > 0; sh--) {
		FSYMBOL(bn_uint4097_rsh)(sh_b);
		FSYMBOL(bn_uint4097_lsh)(rquo);
		if (FSYMBOL(bn_uint4097_cmp)(rrem, sh_b) >= 0) {
			FSYMBOL(bn_uint4096_sub)(rrem, rrem, sh_b);
			rquo[0] |= 1;
		}
	}

	FSYMBOL(bn_uint4096_move)(quo, rquo);
	FSYMBOL(bn_uint4096_move)(rem, rrem);
} /* end */

/* @func: bn_uint4096_strtonum - string convert big number
* @param1: bn_uint4096_t # number
* @param2: char **       # end pointer
* @param3: int32         # base type
* @return: void
*/
void FSYMBOL(bn_uint4096_strtonum)(bn_uint4096_t t, const char *s, char **e,
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

	FSYMBOL(bn_uint4096_zero)(t);
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

		FSYMBOL(bn_uint4096_mul_1)(t, t, b);
		FSYMBOL(bn_uint4096_add_1)(t, t, d);
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

/* @func: bn_uint4096_numtostr - big number to string
* @param1: char *              # string buffer
* @param2: const bn_uint4096_t # number
* @return: void
*/
void FSYMBOL(bn_uint4096_numtostr)(char *buf, const bn_uint4096_t n) {
	bn_uint4096_t quo, rem, b;
	FSYMBOL(bn_uint4096_move)(quo, n);
	FSYMBOL(bn_uint4096_zero)(b);
	b[0] = 10;

	int32 pos = 0;
	do {
		FSYMBOL(bn_uint4096_div)(quo, rem, quo, b);
		int32 k = pos;
		pos = _out_decimal(pos, buf, rem[0]);
		for (int32 i = k, j = pos - 1; i < j; i++, j--) {
			char t = buf[i];
			buf[i] = buf[j];
			buf[j] = t;
		}
	} while (FSYMBOL(bn_uint4096_cmp_1)(quo, 0));
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

/* @func: bn_uint4096_numtostr_hex - big number to string (hexadecimal)
* @param1: char *              # string buffer
* @param2: const bn_uint4096_t # number
* @return: void
*/
void FSYMBOL(bn_uint4096_numtostr_hex)(char *buf, const bn_uint4096_t n) {
	bn_uint4096_t quo, rem, b;
	FSYMBOL(bn_uint4096_move)(quo, n);
	FSYMBOL(bn_uint4096_zero)(b);
	b[0] = 16;

	int32 pos = 0;
	do {
		FSYMBOL(bn_uint4096_div)(quo, rem, quo, b);
		int32 k = pos;
		pos = _out_hex(pos, buf, rem[0]);
		for (int32 i = k, j = pos - 1; i < j; i++, j--) {
			char t = buf[i];
			buf[i] = buf[j];
			buf[j] = t;
		}
	} while (FSYMBOL(bn_uint4096_cmp_1)(quo, 0));
	buf[pos] = '\0';

	for (int32 i = 0, j = pos - 1; i < j; i++, j--) {
		char t = buf[i];
		buf[i] = buf[j];
		buf[j] = t;
	}
} /* end */
