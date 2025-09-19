/* xmman_sys.h - definitions */

#ifndef LIBF_SL_XMMAN_SYS_H
#define LIBF_SL_XMMAN_SYS_H

#include <libf/config.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>


/* @def: mman */
#undef XSYS_PROT_EXEC
#define XSYS_PROT_EXEC 0x01
#undef XSYS_PROT_NONE
#define XSYS_PROT_NONE 0x02
#undef XSYS_PROT_READ
#define XSYS_PROT_READ 0x04
#undef XSYS_PROT_WRITE
#define XSYS_PROT_WRITE 0x08

#undef XSYS_MAP_SHARED
#define XSYS_MAP_SHARED 0x01
#undef XSYS_MAP_PRIVATE
#define XSYS_MAP_PRIVATE 0x02
#undef XSYS_MAP_ANONYMOUS
#define XSYS_MAP_ANONYMOUS 0x04
#undef XSYS_MAP_ANON
#define XSYS_MAP_ANON XSYS_MAP_ANONYMOUS
#undef XSYS_MAP_FIXED
#define XSYS_MAP_FIXED 0x08
/* end */

#ifdef __cplusplus
extern "C" {
#endif

/* xmman_lock_sys.c */
extern int32 XSYMBOL(mlock)(const void *addr, uint64 len);
extern int32 XSYMBOL(munlock)(const void *addr, uint64 len);
extern int32 XSYMBOL(mlockall)(int32 flags);
extern int32 XSYMBOL(munlockall)(void);

/* xmman_mmap_sys.c */
extern void *XSYMBOL(mmap)(void *addr, uint64 len, int32 prot, int32 flags,
		int32 fd, uint64 off);
extern int32 XSYMBOL(munmap)(void *addr, uint64 len);
extern int32 XSYMBOL(mprotect)(void *addr, uint64 len, int32 prot);
extern int32 XSYMBOL(msync)(void *addr, uint64 len, int32 flags);
extern int32 XSYMBOL(mlock)(const void *addr, uint64 len);
extern int32 XSYMBOL(munlock)(const void *addr, uint64 len);
extern int32 XSYMBOL(mlockall)(int32 flags);
extern int32 XSYMBOL(munlockall)(void);

#ifdef __cplusplus
}
#endif


#endif
