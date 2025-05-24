#!/bin/sh
#
# creturn new C header/include
#


path="$1"
name="$2"

_path=$(echo "$path/$name" |sed 's/\./_/g;s/\//_/g' |tr 'a-z' 'A-Z')

cat <<__EOF__
/* $name - definitions */

#ifndef $_path
#define $_path

#include <libf/config.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>


......


#endif
__EOF__
