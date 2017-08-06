#!/usr/bin/env bash

find . -name Makefile.in -delete;
find . -name Makefile -delete;

[ -d m4 ] && rm -rf m4

for f in COPYING README depcomp missing INSTALL install-sh config.sub config.guess compile \
    config.h.in configure aclocal.m4 configure aclocal.m4 ltmain.sh ar-lib \
    autom4te.cache COPYING config.log test-driver stamp-h1 config.status libtool \
    config.h config.h.in~
do
    [ -d "$f" ] && rm -rf "$f"
    rm -f "$f"
done

