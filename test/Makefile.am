AM_CPPFLAGS = -g -I$(top_srcdir)/src/ -I/Users/caoj7/program/common-c/include

LDADD = -L$(top_builddir)/src -lhamster_avro -L/Users/caoj7/program/common-c/lib -lavro

bin_PROGRAMS = \
	person_test \
	allocate_request_test \
	allocate_response_test \
	launch_request_test \
	launch_response_test


person_test_SOURCES = person_test.c
allocate_request_test_SOURCES = allocate_request_test.c
allocate_response_test_SOURCES = allocate_response_test.c  
launch_request_test_SOURCES = launch_request_test.c
launch_response_test_SOURCES = launch_response_test.c 