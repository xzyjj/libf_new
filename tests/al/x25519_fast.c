#include <stdio.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/al/bn_512.h>


static void _print(char *s, const uint32 n[8]) {
	bn_uint512_t v;
	FSYMBOL(bn_uint512_zero)(v);
	for (int32 i = 0; i < 8; i++)
		v[i] = n[i];

	char buf[1 << 16];
	FSYMBOL(bn_uint512_numtostr)(buf, v);
	printf("%s%s\n", s, buf);
}

void print(char *s, bn_uint512_t v) {
	char buf[1 << 16];
	FSYMBOL(bn_uint512_numtostr)(buf, v);
	printf("%s%s\n", s, buf);
}

static void _x25519_swap(uint32 a[8], uint32 b[8], uint32 bit) {
	uint32 t0[8], t1[8];

	bit = (~bit & 1) - 1;
	for (int32 i = 0; i < 8; i++) {
		uint32 t = bit & (a[i] ^ b[i]);
		t0[i] = a[i] ^ t;
		t1[i] = b[i] ^ t;
	}

	for (int32 i = 0; i < 8; i++) {
		a[i] = t0[i];
		b[i] = t1[i];
	}
}

static void _x25519_add(uint32 r[8], const uint32 a[8], const uint32 b[8]) {
	uint32 rr[8];
	uint32 carry = 0;
	uint64L tmp = 0;

	for (int32 i = 0; i < 8; i++) {
		tmp = (uint64L)a[i] + b[i] + carry,
		rr[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}

	carry *= 38;
	for (int32 i = 0; i < 8; i++) {
		tmp = (uint64L)rr[i] + carry;
		rr[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}

	carry *= 38;
	for (int32 i = 0; i < 8; i++) {
		tmp = (uint64L)rr[i] + carry;
		rr[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}

	for (int32 i = 0; i < 8; i++)
		r[i] = rr[i];
}

static void _x25519_sub(uint32 r[8], const uint32 a[8], const uint32 b[8]) {
	uint32 rr[8];
	uint32 carry = 0;
	uint64L tmp = 0;

	for (int32 i = 0; i < 8; i++) {
		tmp = (int64L)a[i] - b[i] + (int32)carry,
		rr[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}

	tmp = (uint64L)rr[0] - (carry & 38);
	rr[0] = tmp & 0xffffffff;
	carry = tmp >> 32;
	for (int32 i = 1; i < 8; i++) {
		tmp = (uint64L)rr[i] + (int32)carry;
		rr[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}

	tmp = (uint64L)rr[0] - (carry & 38);
	rr[0] = tmp & 0xffffffff;
	carry = tmp >> 32;
	for (int32 i = 1; i < 8; i++) {
		tmp = (uint64L)rr[i] + (int32)carry;
		rr[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}

	for (int32 i = 0; i < 8; i++)
		r[i] = rr[i];
}

static void _x25519_mul(uint32 r[8], const uint32 a[8], const uint32 b[8]) {
	uint32 rr[16];
	uint32 carry = 0;
	uint64L tmp = 0;

	for (int32 i = 0; i < 16; i++)
		rr[i] = 0;

	for (int32 i = 0; i < 8; i++) {
		tmp = (uint64L)a[0] * b[i] + carry;
		rr[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}
	rr[8] = carry;

	for (int32 i = 1; i < 8; i++) {
		carry = 0;
		for (int32 j = 0; j < 8; j++) {
			tmp = (uint64L)a[i] * b[j] + rr[i + j] + carry;
			rr[i + j] = tmp & 0xffffffff;
			carry = tmp >> 32;
		}
		rr[i + 8] = carry;
	}

	carry = 0;
	for (int32 i = 0; i < 8; i++) {
		tmp = (uint64L)rr[i + 8] * 38 + rr[i] + carry;
		rr[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}

	tmp = (uint64L)carry * 38 + rr[0];
	rr[0] = tmp & 0xffffffff;
	carry = tmp >> 32;
	for (int32 i = 1; i < 8; i++) {
		tmp = (uint64L)rr[i] + carry;
		rr[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}

	tmp = (uint64L)carry * 38 + rr[0];
	rr[0] = tmp & 0xffffffff;
	carry = tmp >> 32;
	for (int32 i = 1; i < 8; i++) {
		tmp = (uint64L)rr[i] + carry;
		rr[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}

	for (int32 i = 0; i < 8; i++)
		r[i] = rr[i];
}

static void _x25519_mul121665(uint32 r[8], const uint32 a[8], const uint32 b[8]) {
	uint32 rr[8];
	uint32 carry = 0;
	uint64L tmp = 0;

	for (int32 i = 0; i < 8; i++) {
		tmp = (uint64L)b[i] * 121665 + a[i] + carry;
		rr[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}

	tmp = (uint64L)carry * 38 + rr[0];
	rr[0] = tmp & 0xffffffff;
	carry = tmp >> 32;
	for (int32 i = 1; i < 8; i++) {
		tmp = (uint64L)rr[i] + carry;
		rr[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}

	tmp = (uint64L)carry * 38 + rr[0];
	rr[0] = tmp & 0xffffffff;
	carry = tmp >> 32;
	for (int32 i = 1; i < 8; i++) {
		tmp = (uint64L)rr[i] + carry;
		rr[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}

	for (int32 i = 0; i < 8; i++)
		r[i] = rr[i];
}

static void _x25519_inv(uint32 r[8], const uint32 z[8]) {
	uint32 z2[8], z9[8], z11[8];
	uint32 z2_5_0[8], z2_10_0[8], z2_20_0[8], z2_50_0[8], z2_100_0[8];
	uint32 t0[8], t1[8];

	/* 2 */
	_x25519_mul(z2, z, z);
	/* 4 */
	_x25519_mul(t1, z2, z2);
	/* 8 */
	_x25519_mul(t0, t1, t1);
	/* 9 */
	_x25519_mul(z9, t0, z);
	/* 11 */
	_x25519_mul(z11, z9, z2);
	/* 22 */
	_x25519_mul(t0, z11, z11);
	/* 2**5 - 2**0 */
	_x25519_mul(z2_5_0, t0, z9);

	/* 2**6 - 2**1 */
	_x25519_mul(t0, z2_5_0, z2_5_0);
	/* 2**7 - 2**2 */
	_x25519_mul(t1, t0, t0);
	/* 2**8 - 2**3 */
	_x25519_mul(t0, t1, t1);
	/* 2**9 - 2**4 */
	_x25519_mul(t1, t0, t0);
	/* 2**10 - 2**5 */
	_x25519_mul(t0, t1, t1);
	/* 2**10 - 2**0 */
	_x25519_mul(z2_10_0, t0, z2_5_0);

	/* 2**11 - 2**1 */
	_x25519_mul(t0, z2_10_0, z2_10_0);
	/* 2**12 - 2**2 */
	_x25519_mul(t1, t0, t0);
	/* 2**20 - 2**10 */
	for (int32 i = 2; i < 10; i += 2) {
		_x25519_mul(t0, t1, t1);
		_x25519_mul(t1, t0, t0);
	}
	/* 2**20 - 2**0 */
	_x25519_mul(z2_20_0, t1, z2_10_0);

	/* 2**21 - 2**1 */
	_x25519_mul(t0, z2_20_0, z2_20_0);
	/* 2**22 - 2**2 */
	_x25519_mul(t1, t0, t0);
	/* 2**40 - 2**20 */
	for (int32 i = 2; i < 20; i += 2) {
		_x25519_mul(t0, t1, t1);
		_x25519_mul(t1, t0, t0);
	}
	/* 2**40 - 2**0 */
	_x25519_mul(t0,t1,z2_20_0);

	/* 2**41 - 2**1 */
	_x25519_mul(t1, t0, t0);
	/* 2**42 - 2**2 */
	_x25519_mul(t0, t1, t1);
	/* 2**50 - 2**10 */
	for (int32 i = 2; i < 10; i += 2) {
		_x25519_mul(t1, t0, t0);
		_x25519_mul(t0, t1, t1);
	}
	/* 2**50 - 2**0 */
	_x25519_mul(z2_50_0, t0, z2_10_0);

	/* 2**51 - 2**1 */
	_x25519_mul(t0, z2_50_0, z2_50_0);
	/* 2**52 - 2**2 */
	_x25519_mul(t1, t0, t0);
	/* 2**100 - 2**50 */
	for (int32 i = 2; i < 50; i += 2) {
		_x25519_mul(t0, t1, t1);
		_x25519_mul(t1, t0, t0);
	}
	/* 2**100 - 2**0 */
	_x25519_mul(z2_100_0, t1, z2_50_0);

	/* 2**101 - 2**1 */
	_x25519_mul(t1, z2_100_0, z2_100_0);
	/* 2**102 - 2**2 */
	_x25519_mul(t0, t1, t1);
	/* 2**200 - 2**100 */
	for (int32 i = 2; i < 100; i += 2) {
		_x25519_mul(t1, t0, t0);
		_x25519_mul(t0, t1, t1);
	}
	/* 2**200 - 2**0 */
	_x25519_mul(t1, t0, z2_100_0);

	/* 2**201 - 2**1 */
	_x25519_mul(t0, t1, t1);
	/* 2**202 - 2**2 */
	_x25519_mul(t1, t0, t0);
	/* 2**250 - 2**50 */
	for (int32 i = 2; i < 50; i += 2) {
		_x25519_mul(t0, t1, t1);
		_x25519_mul(t1, t0, t0);
	}
	/* 2**250 - 2**0 */
	_x25519_mul(t0, t1, z2_50_0);

	/* 2**251 - 2**1 */
	_x25519_mul(t1, t0, t0);
	/* 2**252 - 2**2 */
	_x25519_mul(t0, t1, t1);
	/* 2**253 - 2**3 */
	_x25519_mul(t1, t0, t0);
	/* 2**254 - 2**4 */
	_x25519_mul(t0, t1, t1);
	/* 2**255 - 2**5 */
	_x25519_mul(t1, t0, t0);
	/* 2**255 - 21 */
	_x25519_mul(r, t1, z11);
}

static void _x25519_mulmod(uint32 r[8]) {
	static const uint32 p[8] = {
		0xffffffed, 0xffffffff, 0xffffffff, 0xffffffff,
		0xffffffff, 0xffffffff, 0xffffffff, 0x7fffffff
		};
	uint32 rr[8], t[8];
	uint32 carry = 0;
	uint64L tmp = 0;

	/* rr = r - p */
	for (int32 i = 0; i < 8; i++) {
		tmp = (int64L)r[i] - p[i] + (int32)carry,
		rr[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}

	/* t : carry */
	t[0] = carry & 0xffffffed;
	for (int32 i = 1; i < 7; i++)
		t[i] = carry;
	t[7] = carry >> 1;

	/* r = rr + t */
	for (int32 i = 0; i < 8; i++) {
		tmp = (uint64L)rr[i] + t[i] + carry,
		r[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}
}


void x25519_scalar_mul(const uint32 k[8], const uint32 b[8], uint32 r[8]) {
	uint32 x1[8], z1[8], x2[8], z2[8];
	for (int32 i = 0; i < 8; i++) {
		x1[i] = 0;
		z1[i] = 0;
		x2[i] = 0;
		z2[i] = 0;
	}
	x1[0] = 1;
	for (int32 i = 0; i < 8; i++)
		x2[i] = b[i];
	z2[0] = 1;

	uint32 A[8], AA[8], B[8], BB[8], E[8], C[8], D[8], DA[8], CB[8];
	for (int32 i = 255; i >= 0; i--) {
		uint32 k_t = (k[i / 32] >> (i % 32)) & 1;
		_x25519_swap(x1, x2, k_t);
		_x25519_swap(z1, z2, k_t);

		/* A = x1 + z1 */
		_x25519_add(A, x1, z1);
		/* AA = A ** 2 */
		_x25519_mul(AA, A, A);

		/* B = x1 - z1 */
		_x25519_sub(B, x1, z1);
		/* BB = B ** 2 */
		_x25519_mul(BB, B, B);

		/* E = AA - BB */
		_x25519_sub(E, AA, BB);
		/* C = x2 + z2 */
		_x25519_add(C, x2, z2);
		/* D = x2 - z2 */
		_x25519_sub(D, x2, z2);

		/* DA = D * A */
		_x25519_mul(DA, D, A);
		/* CB = C * B */
		_x25519_mul(CB, C, B);

		/* x2 = DA * CB */
		_x25519_add(x2, DA, CB);
		/* x2 = x2 ** 2 */
		_x25519_mul(x2, x2, x2);

		/* z2 = DA - CB */
		_x25519_sub(z2, DA, CB);
		/* z2 = z2 ** 2 */
		_x25519_mul(z2, z2, z2);
		/* z2 = z2 * b */
		_x25519_mul(z2, z2, b);

		/* x1 = AA * BB */
		_x25519_mul(x1, AA, BB);

		/* z1 = AA + 121665 * E */
		_x25519_mul121665(z1, AA, E);
		/* z1 = z1 * E */
		_x25519_mul(z1, z1, E);

		_x25519_swap(x1, x2, k_t);
		_x25519_swap(z1, z2, k_t);
	}

	_print("z1: ", z1);

	_x25519_inv(z1, z1);

	_print("z1: ", z1);
	_print("x: ", x1);

	_x25519_mul(r, z1, x1);
	_x25519_mulmod(r);

	_print("r: ", r);
}

void x25519_clamp_key(uint32 k[8]) {
	k[0] &= ~0x07;
	k[7] |= 0x40000000;
	k[7] &= 0x7fffffff;
}

void x25519_base_mask(uint32 b[8]) {
	b[7] &= 0x7fffffff;
}

int main(void) {
	bn_uint512_t t;
	uint32 a[10], b[10], r[10];
	XSYMBOL(memset)(a, 0, sizeof(uint32) * 10);
	XSYMBOL(memset)(b, 0, sizeof(uint32) * 10);
	a[7] = 1;
	b[7] = 5;

	FSYMBOL(bn_uint512_zero)(t);
	XSYMBOL(memcpy)(t, a, sizeof(uint32) * 8);
	print("a: ", t);

	FSYMBOL(bn_uint512_zero)(t);
	XSYMBOL(memcpy)(t, b, sizeof(uint32) * 8);
	print("b: ", t);

	_x25519_sub(a, a, b);
	_x25519_add(r, a, b);

	FSYMBOL(bn_uint512_zero)(t);
	XSYMBOL(memcpy)(t, r, sizeof(uint32) * 8);
	print("r: ", t);

	_x25519_inv(r, r);

	FSYMBOL(bn_uint512_zero)(t);
	XSYMBOL(memcpy)(t, r, sizeof(uint32) * 8);
	print("r: ", t);

	FSYMBOL(bn_uint512_zero)(t);
	XSYMBOL(memcpy)(t, a, sizeof(uint32) * 8);
	print("a: ", t);

	FSYMBOL(bn_uint512_zero)(t);
	XSYMBOL(memcpy)(t, b, sizeof(uint32) * 8);
	print("b: ", t);

	_x25519_swap(a, b, 1);

	FSYMBOL(bn_uint512_zero)(t);
	XSYMBOL(memcpy)(t, a, sizeof(uint32) * 8);
	print("a: ", t);

	FSYMBOL(bn_uint512_zero)(t);
	XSYMBOL(memcpy)(t, b, sizeof(uint32) * 8);
	print("b: ", t);

	uint32 u[10], pub[10];

	XSYMBOL(memset)(a, 0, sizeof(uint32) * 10);
	XSYMBOL(memset)(b, 0, sizeof(uint32) * 10);
	XSYMBOL(memset)(u, 0, sizeof(uint32) * 10);
	XSYMBOL(memset)(pub, 0, sizeof(uint32) * 10);
	u[0] = 9;
	x25519_clamp_key(a);

	_print("a: ", a);
	_print("u: ", u);
	x25519_scalar_mul(a, u, pub);
	_print("pub: ", pub);

	return 0;
}
