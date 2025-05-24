#include <stdio.h>
#include <libf/sl/xstdint.h>
#include <libf/al/bn_4096.h>


int main(void) {
	bn_uint4096_t a, b, r, rem;
	char buf[1 << 16];

	FSYMBOL(bn_uint4096_strtonum)(a,
		"2270180129378501419358040512020458674106123596276658390709402187921517148311"
		"9139894870133091111044901683400949483846818299518041763507948922590774925466"
		"0881718792594659210265970467004498198990968620394600177430944738110569912941"
		"2854289188085536270740767072259373777266697344097736124333639730805176309150"
		"6836310795312607239520365290032105848839507981452307299417185715796297454995"
		"0235053160409198591937180233074148804462179228008317660409386563445710347785"
		"5345712108053073639453592393265186603051504106096643731332367283153932350006"
		"7937107541955437362433248361242525945868802353916766181532375855504886901432"
		"221349733"
		,
		NULL, 10);

	FSYMBOL(bn_uint4096_zero)(b);
	FSYMBOL(bn_uint4096_strtonum)(b,
		"1234567890"
		,
		NULL, 10);

/*	FSYMBOL(bn_uint4096_numtostr)(a, buf);
	printf("a: %s\n", buf);
	FSYMBOL(bn_uint4096_numtostr)(b, buf);
	printf("b: %s\n", buf);
*/
	FSYMBOL(bn_uint4096_div)(r, rem, a, b);
/*	FSYMBOL(bn_uint4096_numtostr)(r, buf);
	printf("quo: %s\n", buf);
	FSYMBOL(bn_uint4096_numtostr)(rem, buf);
	printf("rem: %s\n", buf);
*/
	FSYMBOL(bn_uint4096_zero)(a);
	FSYMBOL(bn_uint4096_strtonum)(a,
		"12345678901234567890"
		,
		NULL, 10);

	FSYMBOL(bn_uint4096_zero)(b);
	FSYMBOL(bn_uint4096_strtonum)(b,
		"1234567890"
		,
		NULL, 10);

/*	FSYMBOL(bn_uint4096_numtostr)(a, buf);
	printf("a: %s\n", buf);
	FSYMBOL(bn_uint4096_numtostr)(b, buf);
	printf("b: %s\n", buf);
*/
	FSYMBOL(bn_uint4096_div)(r, rem, a, b);
/*	FSYMBOL(bn_uint4096_numtostr)(r, buf);
	printf("quo: %s\n", buf);
	FSYMBOL(bn_uint4096_numtostr)(rem, buf);
	printf("rem: %s\n", buf);
*/
	FSYMBOL(bn_uint4096_zero)(a);
	a[127] = 1;

	FSYMBOL(bn_uint4096_zero)(b);
	b[2] = 3;

/*	FSYMBOL(bn_uint4096_numtostr)(a, buf);
	printf("a: %s\n", buf);
	FSYMBOL(bn_uint4096_numtostr)(b, buf);
	printf("b: %s\n", buf);
*/
	FSYMBOL(bn_uint4096_div)(r, rem, a, b);
/*	FSYMBOL(bn_uint4096_numtostr)(r, buf);
	printf("quo: %s\n", buf);
	FSYMBOL(bn_uint4096_numtostr)(rem, buf);
	printf("rem: %s\n", buf);
*/
	return 0;
}
