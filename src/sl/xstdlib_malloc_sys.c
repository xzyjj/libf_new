/* xstdlib_malloc_sys.c - memory allocator implementations */

#include <libf/config.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstdlib_sys.h>
#include <libf/sl/xatomic.h>
#include <libf/sl/internal.h>

#include <sys/mman.h>
#include <sched.h>


/* @def: malloc */
static void *_mmap(uint64 size, void *arg);
static void _munmap(void *p, uint64 size, void *arg);

UMALLOC_NEW(_umalloc_ctx, _mmap, _munmap, NULL);

static volatile int32 _lock = 0;
/* end */

/* @func: _mmap (static) - heap memory mapping
* @param1: uint64 # memory size
* @param2: void * # callback arg
* @return: void * # mapping addr
*/
static void *_mmap(uint64 size, void *arg) {
	(void)arg;

	size = ((size + 4095) / 4096) * 4096;
	void *p = mmap(NULL, size, PROT_READ | PROT_WRITE,
		MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

	return p;
} /* end */

/* @func: _munmap (static) - heap memory unmapping
* @param1: void * # mapping addr
* @param2: uint64 # memory size
* @param3: void * # callback arg
* @return: void
*/
static void _munmap(void *p, uint64 size, void *arg) {
	(void)arg;

	size = ((size + 4095) / 4096) * 4096;
	munmap(p, size);
} /* end */

/* @func: malloc - memory allocator
* @param1: uint64 # allocate size
* @return: void * # allocated pointer
*/
void *XSYMBOL(malloc)(uint64 size) {
	int32 lock = XSYMBOL(atomic_cas)(&_lock, 0, 1);
	if (lock) {
		while (XSYMBOL(atomic_cas)(&_lock, 0, 1))
			sched_yield();
	}

	void *p = XSYMBOL(internal_umalloc)(&_umalloc_ctx, size);
	while (!XSYMBOL(atomic_cas)(&_lock, 1, 0))
		sched_yield();

	return p;
} /* end */

/* @func: free - free allocated memory
* @param2: void * # allocated pointer
* @return: void
*/
void XSYMBOL(free)(void *p) {
	if (!p) /* ptr == NULL */
		return;

	int32 lock = XSYMBOL(atomic_cas)(&_lock, 0, 1);
	if (lock) {
		while (XSYMBOL(atomic_cas)(&_lock, 0, 1))
			sched_yield();
	}

	XSYMBOL(internal_ufree)(&_umalloc_ctx, p);
	while (!XSYMBOL(atomic_cas)(&_lock, 1, 0))
		sched_yield();
} /* end */

/* @func: free_all - free all allocated memory
* @return: void
*/
void XSYMBOL(free_all)(void) {
	XSYMBOL(internal_ufree_all)(&_umalloc_ctx);
} /* end */

/* @func: malloc_idle - count idle chunk
* @param2: int32  # 0: chunk, 1: size, 2: inuse, 3: inuse size
* @return: uint64 # count size
*/
uint64 XSYMBOL(malloc_idle)(int32 type) {
	int32 lock = XSYMBOL(atomic_cas)(&_lock, 0, 1);
	if (lock) {
		while (XSYMBOL(atomic_cas)(&_lock, 0, 1))
			sched_yield();
	}

	uint64 n = XSYMBOL(internal_umalloc_idle)(&_umalloc_ctx, type);
	while (!XSYMBOL(atomic_cas)(&_lock, 1, 0))
		sched_yield();

	return n;
} /* end */
