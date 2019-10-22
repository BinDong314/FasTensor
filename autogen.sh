#!/bin/bash

exists()
{
  command -v "$1" >/dev/null 2>&1
}

if exists libtoolize  ; then
	echo "libtoolize"
	libtoolize --force
elif exists glibtoolize ; then
	echo "glibtoolize"
	glibtoolize --force
fi

aclocal -I m4 --install
autoheader
automake --force-missing --add-missing --copy 
autoconf


