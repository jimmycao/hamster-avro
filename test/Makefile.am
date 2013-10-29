AM_CPPFLAGS = -g -I$(top_srcdir)/common/ 
#-I/Users/caoj7/program/common-c/include

LDADD = -L$(top_builddir)/common -lhamster_common 
#-L/Users/caoj7/program/common-c/lib -lavro

bin_PROGRAMS = \
	allocate_request_test \
	allocate_response_test \
	finish_request_test \
	finish_response_test \
	heartbeat_request_test \
	heartbeat_response_test \
	launch_request_test \
	launch_response_test \
	register_request_test \
	register_response_test \
	hnp_request_wrapper_test


allocate_request_test_SOURCES = allocate_request_test.c
allocate_response_test_SOURCES = allocate_response_test.c  
finish_request_test_SOURCES = finish_request_test.c
finish_response_test_SOURCES = finish_response_test.c
heartbeat_request_test_SOURCES = heartbeat_request_test.c
heartbeat_response_test_SOURCES = heartbeat_response_test.c
launch_request_test_SOURCES = launch_request_test.c
launch_response_test_SOURCES = launch_response_test.c
register_request_test_SOURCES = register_request_test.c
register_response_test_SOURCES = register_response_test.c
hnp_request_wrapper_test_SOURCES = hnp_request_wrapper_test.c

 