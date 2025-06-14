#include <stdio.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/al/base16.h>
#include <libf/al/ed25519_fast.h>


static void print_hex2u8(const char *s, const uint8 *p) {
	char buf[1024];
	for (int32 i = 0; i < ED25519_LEN; i++)
		snprintf(&buf[i * 2], sizeof(buf), "%02x", p[i]);
	printf("%s%s\n", s, buf);
}

static void hex2u8(uint8 *p, const char *s, uint32 len) {
	uint32 n = ED25519_LEN;
	XSYMBOL(memset)(p, 0, ED25519_LEN);
	FSYMBOL(base16_dec)(s, len, (char *)p, &n);
}

int main(void) {
	struct ed25519_point xyz1, xyz_b;
	uint8 r[ED25519_LEN];
	const char *s = NULL;
	XSYMBOL(memcpy)(xyz_b.x, ED25519_FAST_BX, ED25519_LEN);
	XSYMBOL(memcpy)(xyz_b.y, ED25519_FAST_BY, ED25519_LEN);
	XSYMBOL(memcpy)(xyz_b.z, ED25519_FAST_BZ, ED25519_LEN);
	XSYMBOL(memcpy)(xyz_b.t, ED25519_FAST_BT, ED25519_LEN);

	/* compress and decompress */
	s = "920ed63056ff7dc15c03c92a02c60da65be28b443ee783ef53f7c5b351b7413b";
	hex2u8((uint8 *)xyz1.x, s, XSYMBOL(strlen)(s));
	s = "7572604e0408ee853b05190663b3406cacaf6eedbf5f36d527d338ec90eecc44";
	hex2u8((uint8 *)xyz1.y, s, XSYMBOL(strlen)(s));
	s = "5fd1a3ba4e8837f28ff65c8431c8e4843d327e9089ac338a94c9fabed991c545";
	hex2u8((uint8 *)xyz1.z, s, XSYMBOL(strlen)(s));
	s = "04287fd126a9398b5dcc3520ac1b5fc5022736e06e000b99b188ea816ae51e60";
	hex2u8((uint8 *)xyz1.t, s, XSYMBOL(strlen)(s));
	print_hex2u8("compress:\nx: ", (uint8 *)xyz1.x);
	print_hex2u8("y: ", (uint8 *)xyz1.y);
	print_hex2u8("z: ", (uint8 *)xyz1.z);
	print_hex2u8("t: ", (uint8 *)xyz1.t);

	FSYMBOL(ed25519_fast_point_compress)(&xyz1, (uint32 *)r);
	print_hex2u8("[de]compress:\nr: ", r);

	FSYMBOL(ed25519_fast_point_decompress)((uint32 *)r, &xyz1);
	print_hex2u8("x: ", (uint8 *)xyz1.x);
	print_hex2u8("y: ", (uint8 *)xyz1.y);
	print_hex2u8("z: ", (uint8 *)xyz1.z);
	print_hex2u8("t: ", (uint8 *)xyz1.t);

	/* decompress */
	s = "250ce9aad7fa9ab34f61921937f8c8a318e5b259e8b3d56bb4049667e4bb6879";
	hex2u8(r, s, XSYMBOL(strlen)(s));
	print_hex2u8("decompress:\nr: ", r);

	FSYMBOL(ed25519_fast_point_decompress)((uint32 *)r, &xyz1);
	print_hex2u8("x: ", (uint8 *)xyz1.x);
	print_hex2u8("y: ", (uint8 *)xyz1.y);
	print_hex2u8("z: ", (uint8 *)xyz1.z);
	print_hex2u8("t: ", (uint8 *)xyz1.t);

	/* scalar multiplication  */
	s = "b06773f23e3f29048b625d52f319e5ade6d0fcd09c74895a42d5f80a3da4fc62";
	hex2u8(r, s, XSYMBOL(strlen)(s));
	print_hex2u8("scalar-mul:\nr: ", r);

	FSYMBOL(ed25519_fast_scalar_mul)((uint32 *)r, &xyz_b, &xyz1);
	print_hex2u8("x: ", (uint8 *)xyz1.x);
	print_hex2u8("y: ", (uint8 *)xyz1.y);
	print_hex2u8("z: ", (uint8 *)xyz1.z);
	print_hex2u8("t: ", (uint8 *)xyz1.t);

	/* compress */
	FSYMBOL(ed25519_fast_point_compress)(&xyz1, (uint32 *)r);
	print_hex2u8("compress:\nr: ", r);

	/* scalar multiplication  */
	s = "c719d8726497f23c2d0db4c7a62a97d3ef397ca029f47e1076cd606e1a896e04";
	hex2u8(r, s, XSYMBOL(strlen)(s));
	print_hex2u8("scalar-mul:\nr: ", r);

	FSYMBOL(ed25519_fast_scalar_mul)((uint32 *)r, &xyz_b, &xyz1);
	print_hex2u8("x: ", (uint8 *)xyz1.x);
	print_hex2u8("y: ", (uint8 *)xyz1.y);
	print_hex2u8("z: ", (uint8 *)xyz1.z);
	print_hex2u8("t: ", (uint8 *)xyz1.t);

	/* compress */
	FSYMBOL(ed25519_fast_point_compress)(&xyz1, (uint32 *)r);
	print_hex2u8("compress:\nr: ", r);

	return 0;
}
