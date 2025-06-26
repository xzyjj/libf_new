/* xsignal_sys.h - standard library signals definitions */

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
extern int32 XSYMBOL(killpg)(pid32_t pgrp, int32 sig);

#ifdef __cplusplus
}
#endif


#endif
