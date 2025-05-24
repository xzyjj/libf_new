#!/bin/sh
#
# get C static functions
#


for i in "$@"; do
	echo "/* $i */"

	sed -n '/^\/\* @func:.*(.*static.*).*-/,/ {$/p' "$i" \
	|sed '/^\/\*/,/\*\/$/d' \
	|sed 's/ {$/;/'
done
