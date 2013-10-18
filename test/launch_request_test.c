#include <stdio.h>
#include <stdlib.h>

#include <avro.h>
#include "src/common.h"
#include "src/constants.h"
#include "src/launch.h"

/*
typedef struct {
	int jobid;
	int vpid;
} process_name_t;

typedef struct {
	char *en_vars;
	char *args;
	char *host_name;
	process_name_t proc_name;
} launch_context_t;
 */

//static void parse_allocate_request(avro_slice_t *slice, int *resource_request_count)
//{
//	char filename[FILE_NAME_LEN];
//	avro_schema_t schema;
//	avro_value_iface_t *iface;
//	avro_value_t record, field_value;
//	size_t index;
//	avro_reader_t reader;
//
//	sprintf(filename, "%s/%s", SCHEMA_PATH, "AllocateRequestRecordAvro.avsc");
//	init_schema(filename, &schema);
//
//	iface = avro_generic_class_from_schema(schema);
//	avro_generic_value_new(iface, &record);
//
//	reader = avro_reader_memory(slice->buffer, slice->len);
//	if (avro_value_read(reader, &record)) {
//		fprintf(stderr, "Unable to read record from memory buffer\n");
//		fprintf(stderr, "Error: %s\n", avro_strerror());
//		exit(1);
//	}
//
//	avro_value_get_by_name(&record, "resource_count", &field_value, &index);
//	avro_value_get_int(&field_value, resource_request_count);
//
//	//avro_generic_value_free(&record);
//	avro_value_iface_decref(iface);
//	avro_schema_decref(schema);
//}

//extern void build_launch_request(launch_context_t *launch_context_array, int array_size, avro_slice_t **slice);


int main()
{
	avro_slice_t *slice;
	launch_context_t launch_context_array[3] = {
			{"var1", "args1", "host1", {101, 1}},
			{"var2", "args2", "host2", {102, 2}},
			{"var3", "args3", "host3", {103, 3}}
			};

	build_launch_request(build_launch_request, 3, &slice);

	free_slice(slice);
	return 0;
}
