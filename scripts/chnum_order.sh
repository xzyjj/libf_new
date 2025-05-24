#!/bin/sh
#
# change C #define number order
#


awk -F '\n' \
'
BEGIN {
	n = 1;
}

{
	if ($0 ~ /\s[0-9]*[0-9]$/) {
		sub(/[0-9]*[0-9]$/, n);
		print;
		n++;
	} else {
		print
	}
}
'
