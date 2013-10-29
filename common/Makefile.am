AM_CPPFLAGS = -I/Users/caoj7/program/common-c/include
LDADD = -L/Users/caoj7/program/common-c/lib -lavro

lib_LTLIBRARIES = libhamster_avro.la

headers = \
	allocate.h \
	common.h \
	constants.h \
	finish.h \
	heartbeat.h \
	hnp_request_wrapper.h \
	launch.h \
	register.h 
	
libhamster_avro_la_SOURCES = \
	$(headers) \
	allocate.c \
	common.c \
	finish.c \
	heartbeat.c \
	hnp_request_wrapper.c \
	launch.c \
	register.c 