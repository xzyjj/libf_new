/* xsignal_sys.h - definitions */

#ifndef LIBF_SL_XSIGNAL_SYS_H
#define LIBF_SL_XSIGNAL_SYS_H

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xtype_sys.h>


#ifdef __cplusplus
extern "C" {
#endif

/* xsignal_proc_sys.c */
extern int32 XSYMBOL(kill)(pid32_t pid, int32 sig);

#ifdef __cplusplus
}
#endif


#endif
