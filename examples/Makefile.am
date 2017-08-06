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

AM_LDFLAGS= -L../library/wampcc -L../library/json -lwampcc -lwampcc_json \
	-lpthread $(JANSSON_LIBS) $(LIBUV_LIBS) $(OPENSSL_LIBS)

noinst_PROGRAMS=basic_embedded_router basic_publisher basic_subscriber	\
basic_caller basic_callee router wampcc_tester ssl_client ssl_server

basic_embedded_router_SOURCES=basic/basic_embedded_router.cc
basic_publisher_SOURCES=basic/basic_publisher.cc
basic_subscriber_SOURCES=basic/basic_subscriber.cc
basic_caller_SOURCES=basic/basic_caller.cc
basic_callee_SOURCES=basic/basic_callee.cc
router_SOURCES=wamp_router/router.cc
wampcc_tester_SOURCES=tester/wampcc_tester.cc
ssl_client_SOURCES=ssl_socket/ssl_client.cc
ssl_server_SOURCES=ssl_socket/ssl_server.cc
