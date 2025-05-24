#!/bin/sh
#
# get C extern functions
#


for i in "$@"; do
	echo
	echo "/* $i */"

	sed -n '/^\/\* @func: /,/ {$/p' "$i" \
	|sed '/^\/\* @func:.*(.*static.*).*-/,/ {$/d;/^\/\*/,/\*\/$/d' \
	|sed 's/ {$/;/;/XSYMBOL(/s/^/extern /'
done
