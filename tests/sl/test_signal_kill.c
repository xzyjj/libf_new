#include <stdio.h>
#include <libf/sl/xstdlib.h>
#include <libf/sl/xsignal_sys.h>


int main(int32 argc, char *argv[]) {
	int32 pid, sig;
	pid = XSYMBOL(atoi)(argv[1]);
	sig = XSYMBOL(atoi)(argv[2]);

	int32 r = XSYMBOL(kill)(pid, sig);
	printf("ret: %d -- pid: %d sig: %d\n", r, pid, sig);

	return 0;
}
