#include <stdio.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/al/bcrypt.h>


const char base64e[] =
	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789./";

void base64_enc(const uint8 *in, char *out, uint32 len) {
	for (uint32 i = 0, j = 0; i < len; i += 3, j += 4) {
		uint32 v = (in[i] << 16) | (in[i + 1] << 8) | in[i + 2];
		out[j]   = base64e[(v >> 18) & 0x3F];
		out[j + 1] = base64e[(v >> 12) & 0x3F];
		out[j + 2] = base64e[(v >> 6) & 0x3F];
		out[j + 3] = base64e[v & 0x3F];
	}
	out[((len + 2) / 3) * 4] = '\0';
}

int main(void) {
	uint8 buf[BCRYPT_HASHPASS_LEN];
	char bbuf[128];
	uint8 salt[16] = {
		0x18, 0x7b, 0xe1, 0x0b,
		0xf8, 0xd6, 0xa8, 0x8e,
		0xd1, 0x83, 0x22, 0xb1,
		0x28, 0x56, 0xec, 0x15
		};
	char *pass = "12345678";


	FSYMBOL(bcrypt_hashpass)((uint8 *)pass, XSYMBOL(strlen)(pass),
		salt, 16, buf, 8);

	for (int32 i = 0; i < BCRYPT_HASHPASS_LEN - 1; i++)
		printf("%02x", buf[i]);
	printf("\n");

	base64_enc(buf, bbuf, BCRYPT_HASHPASS_LEN - 1);
	printf("%s\n", bbuf);

	base64_enc(salt, bbuf, 16);
	printf("%s\n", bbuf);

	return 0;
}
