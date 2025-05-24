#!/bin/sh
#
# create new C
#


name="$1"

cat <<__EOF__
/* $name - implementations */

#include <libf/config.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>


......


__EOF__
