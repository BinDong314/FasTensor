#! /bin/bash

if [ -x libtoolize ]; then
	libtoolize --force
elif [ -x glibtoolize ]; then
	glibtoolize --force
fi

aclocal -I m4 --install
autoheader
automake --force-missing --add-missing --copy 
autoconf


