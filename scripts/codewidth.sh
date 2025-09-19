#!/bin/sh
#
# get the maximum length line in the file
#  max: >80
#


for i in "$@"; do
	awk -F '\n' '
{
	s = $0;
	n = gsub(/\t/, "", s);
	len = length(s) + (n * 8);
	if (len > 80) {
		printf "file: %s, line: %d, width: %d\n",
			FILENAME, NR, len
	}
}
	' "$i"
done
