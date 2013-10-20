AM_CPPFLAGS = -I/Users/caoj7/program/common-c/include
LDADD = -L/Users/caoj7/program/common-c/lib -lavro

lib_LTLIBRARIES = libhamster_avro.la

headers = \
	common.h \
	constants.h \
	register.h \
	hnp_request_wrapper.h \
	allocate.h \
	launch.h \
	finish.h \
	heartbeat.h \
	person.h 
	
libhamster_avro_la_SOURCES = = \
	$(headers) \
	common.c \
	hnp_request_wrapper.c \
	register.c \
	allocate.c \
	launch.c \
	finish.c \
	heartbeat.c \
	person.c 
