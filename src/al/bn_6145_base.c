/* bn_6145_base.c - big number calculation (6145-bit) implementations */

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/al/bn_6145.h>


/* @func: bn_6145_init - big number init
* @param1: bn_6145_t # target
* @return: void
*/
void FSYMBOL(bn_6145_init)(bn_6145_t t) {
	XSYMBOL(memset)(t, 0, sizeof(bn_6145_t));
} /* end */

/* @func: bn_6145_move - big number move
* @param1: bn_6145_t       # target
* @param2: const bn_6145_t # source
* @return: void
*/
void FSYMBOL(bn_6145_move)(bn_6145_t a, const bn_6145_t b) {
	XSYMBOL(memcpy)(a, b, sizeof(bn_6145_t));
} /* end */

/* @func: bn_6145_u8copy - big number uintB copy
* @param1: bn_6145_t     # target
* @param2: const uint8 * # source
* @param2: uint32        # copy length (byte)
* @return: void
*/
void FSYMBOL(bn_6145_u8copy)(bn_6145_t a, const uint8 *b, uint32 len) {
	FSYMBOL(bn_6145_init)(a);
	XSYMBOL(memcpy)(a, b, len);
} /* end */

/* @func: bn_6145_swap - big number swap
* @param1: bn_6145_t # number
* @param2: bn_6145_t # number
* @return: void
*/
void FSYMBOL(bn_6145_swap)(bn_6145_t a, bn_6145_t b) {
	for (int32 i = 0; i < BN_6145_MAX; i++) {
		uint32 tmp = b[i];
		a[i] = b[i];
		b[i] = tmp;
	}
} /* end */

/* @func: bn_6145_set1 - big number low-bit assign
* @param1: bn_6145_t # target
* @param2: uint32    # number
* @return: void
*/
void FSYMBOL(bn_6145_set1)(bn_6145_t t, uint32 n) {
	FSYMBOL(bn_6145_init)(t);
	t[0] = n;
} /* end */

/* @func: bn_6145_bits - get the bit length of big number
* @param1: const bn_6145_t # number
* @return: int32           # bits length
*/
int32 FSYMBOL(bn_6145_bits)(const bn_6145_t n) {
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

	for (int32 i = BN_6145_SIZE - 1; i >= 0; i--) {
		if (n[i]) {
			for (int32 k = 31; k >= 0; k--) {
				if (n[i] & pow2[k])
					return (i * 32) + k + 1;
			}
		}
	}

	return 0;
} /* end */

/* @func: bn_6145_lsh - big number shift left
* @param1: bn_6145_t # target
* @param2: uint32    # bits number
* @return: void
*/
void FSYMBOL(bn_6145_lsh)(bn_6145_t t, uint32 n) {
	bn_6145_t r;
	int32 m, w, x;

	if (!n)
		return;

	FSYMBOL(bn_6145_init)(r);

	m = n / 32;
	w = n % 32;
	if (w) {
		x = 32 - w;
		uint32 carry = 0;
		for (int32 i = m; i < BN_6145_SIZE; i++) {
			uint32 tmp = t[i - m] >> x;
			r[i] = (t[i - m] << w) | carry;
			carry = tmp;
		}
	} else {
		for (int32 i = m; i < BN_6145_SIZE; i++)
			r[i] = t[i - m];
	}

	r[BN_6145_SIG] = t[BN_6145_SIG];
	FSYMBOL(bn_6145_move)(t, r);
} /* end */

/* @func: bn_6145_rsh - big number shift right
* @param1: bn_6145_t # target
* @param2: uint32    # bits number
* @return: void
*/
void FSYMBOL(bn_6145_rsh)(bn_6145_t t, uint32 n) {
	bn_6145_t r;
	int32 m, w, x, u;

	if (!n)
		return;

	FSYMBOL(bn_6145_init)(r);

	m = n / 32;
	w = n % 32;
	if (w) {
		x = 32 - w;
		u = (1 << w) - 1;
		uint32 carry = 0;
		for (int32 i = BN_6145_SIZE - 1; i >= m; i--) {
			uint32 tmp = t[i] & u;
			r[i - m] = (t[i] >> w) | (carry << x);
			carry = tmp;
		}
	} else {
		for (int32 i = m; i < BN_6145_SIZE; i++)
			r[i - m] = t[i];
	}

	r[BN_6145_SIG] = t[BN_6145_SIG];
	FSYMBOL(bn_6145_move)(t, r);
} /* end */

/* @func: bn_6145_and - big number and operation
* @param1: bn_6145_t       # result
* @param2: const bn_6145_t # number
* @param3: const bn_6145_t # number
* @return: void
*/
void FSYMBOL(bn_6145_and)(bn_6145_t r,
		const bn_6145_t a, const bn_6145_t b) {
	for (int32 i = 0; i < BN_6145_SIZE; i++)
		r[i] = a[i] & b[i];
	r[BN_6145_SIG] = 0;
} /* end */

/* @func: bn_6145_or - big number or operation
* @param1: bn_6145_t       # result
* @param2: const bn_6145_t # number
* @param3: const bn_6145_t # number
* @return: void
*/
void FSYMBOL(bn_6145_or)(bn_6145_t r,
		const bn_6145_t a, const bn_6145_t b) {
	for (int32 i = 0; i < BN_6145_SIZE; i++)
		r[i] = a[i] | b[i];
	r[BN_6145_SIG] = 0;
} /* end */

/* @func: bn_6145_xor - big number xor operation
* @param1: bn_6145_t       # result
* @param2: const bn_6145_t # number
* @param3: const bn_6145_t # number
* @return: void
*/
void FSYMBOL(bn_6145_xor)(bn_6145_t r,
		const bn_6145_t a, const bn_6145_t b) {
	for (int32 i = 0; i < BN_6145_SIZE; i++)
		r[i] = a[i] ^ b[i];
	r[BN_6145_SIG] = 0;
} /* end */

/* @func: bn_6145_not - big number not operation
* @param1: bn_6145_t       # result
* @param2: const bn_6145_t # number
* @return: void
*/
void FSYMBOL(bn_6145_not)(bn_6145_t r,
		const bn_6145_t a) {
	for (int32 i = 0; i < BN_6145_SIZE; i++)
		r[i] = ~a[i];
	r[BN_6145_SIG] = 0;
} /* end */

/* @func: bn_6145_strtonum - string convert big number
* @param1: bn_6145_t    # result
* @param2: const char * # string pointer
* @param3: char **      # end pointer
* @param4: int32        # base type
* @return: void
*/
void FSYMBOL(bn_6145_strtonum)(bn_6145_t t,
		const char *s, char **e, int32 b) {
	for (; *s == ' '; s++);

	int32 neg = 0;
	if (*s == '-' || *s == '+')
		neg = (*s++ == '-') ? 1 : 0;

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

	FSYMBOL(bn_6145_init)(t);
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

		FSYMBOL(bn_u6145_mul1)(t, t, b);
		FSYMBOL(bn_u6145_add1)(t, t, d);
	}

	if (e) /* end position */
		*e = (char *)s;

	t[BN_6145_SIG] = neg;
} /* end */
