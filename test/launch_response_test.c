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

typedef struct {
	bool is_successful;
	process_name_t proc_name;
} launch_response_t;

*/
static void build_launch_response(launch_response_t *launch_response_array, int array_size, avro_slice_t **slice)
{
	char filename[FILE_NAME_LEN];
	char buf[BUFFER_SIZE];
	long len = 0;
	avro_schema_t schema;
	avro_value_iface_t *iface;
	avro_value_t record;
	avro_value_t results_value, LaunchResult_value, is_successful_value, name_value, jobid_value, vpid_value;
	size_t index;
	int i;

	avro_writer_t writer;

	sprintf(filename, "%s/%s", SCHEMA_PATH, "LaunchResponseRecordAvro.avsc");
	init_schema(filename, &schema);

	iface = avro_generic_class_from_schema(schema);
	avro_generic_value_new(iface, &record);

	avro_value_get_by_name(&record, "results", &results_value, &index);

	for (i = 0; i < array_size; i++) {
		avro_value_append(&results_value, &LaunchResult_value, &index);

		avro_value_get_by_name(&LaunchResult_value, "is_successful", &is_successful_value, &index);
		avro_value_set_boolean(&is_successful_value, launch_response_array[i].is_successful);

		avro_value_get_by_name(&LaunchResult_value, "name", &name_value, &index);

		avro_value_get_by_name(&name_value, "jobid", &jobid_value, &index);
		avro_value_set_int(&jobid_value, launch_response_array[i].proc_name.jobid);

		avro_value_get_by_name(&name_value, "vpid", &vpid_value, &index);
		avro_value_set_int(&vpid_value, launch_response_array[i].proc_name.vpid);
	}

	/* create a writer with memory buffer */
	writer = avro_writer_memory(buf, sizeof(buf));
	/* write record to writer (buffer) */
	if (avro_value_write(writer, &record)) {
		fprintf(stderr, "Unable to write record to memory buffer\n");
		fprintf(stderr, "Error: %s\n", avro_strerror());
		exit(1);
	}

	avro_writer_flush(writer);
	len = avro_writer_tell(writer);

	//avro_generic_value_free(&record);
	avro_value_iface_decref(iface);
	avro_schema_decref(schema);

	*slice = xmalloc(sizeof(avro_slice_t));
	(*slice)->buffer = xmalloc(len);
	(*slice)->len = len;
	memcpy((*slice)->buffer, buf, len);
}
/*
{
	"type": "record",
	"name": "LaunchResponse",
	"fields": [
			   	{"name": "results", "type": {
			   							"type": "array",
			   							"items": {
			   								"type": "record",
			   								"name": "LaunchResult",
			   								"fields": [
			   									{"name": "is_successful", "type": "boolean"},
			   									{"name": "name", "type": {
			   														"name": "ProcessName",
			   														"type": "record",
			   														"fields": [
			   															{"name": "jobid", "type": "int"},
																		{"name": "vpid", "type": "int"}
			   														]
			   													 }
			   									}
			   								]
			   							}

								     }
			    }
	]
}

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

typedef struct {
	bool is_successful;
	process_name_t proc_name;
} launch_response_t;

 */

//extern void parse_launch_response(avro_slice_t *slice, launch_response_t **launch_response_array, int *array_size)

int main()
{
	avro_slice_t *slice;

	launch_response_t launch_response_array[3] = {
			{true, {101, 1}},
			{false, {102, 2}},
			{false, {103, 3}},
			};
	build_launch_response(launch_response_array, 3, &slice);
//============================
	launch_response_t *launch_response_array_result;
	int array_size_result = 0;
	int i;
	parse_launch_response(slice, &launch_response_array_result, &array_size_result);
	for (i = 0; i < array_size_result; i++) {
		printf("%d %d %d\n", launch_response_array_result[i].is_successful, launch_response_array_result[i].proc_name.jobid, launch_response_array_result[i].proc_name.vpid);
	}

	free_launch_response_array(launch_response_array_result);

//===========================
	free_slice(slice);
	return 0;
}
