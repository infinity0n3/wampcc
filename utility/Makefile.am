#
# Copyright (c) 2017 Darren Smith
#
# wampcc is free software; you can redistribute it and/or modify
# it under the terms of the MIT license. See LICENSE for details.
#

# NOTE: AM_CPPFLAGS is ignored in preference to a per-executable (or
# per-library) _CPPFLAGS variable if it is defined
#
# NOTE: it is not good practice to add "-Wall" and "-03" etc here.  Those
# choices should be made by the user when they invoke the configure script.
AM_CPPFLAGS = -Wall -g3 -ggdb -std=c++11 -O0 \
	-I$(top_srcdir)/include \
	$(JANSSON_CFLAGS) \
	$(LIBUV_CFLAGS) \
	$(OPENSSL_CFLAGS)

AM_LDFLAGS = -lpthread $(JANSSON_LIBS) $(LIBUV_LIBS) $(OPENSSL_LIBS)

bin_PROGRAMS = admin

admin_SOURCES=admin.cc
admin_LDADD=-lwampcc -lwampcc_json 
admin_LDFLAGS=-L../library/wampcc -L../library/json
