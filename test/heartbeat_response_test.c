#include <stdio.h>
#include <stdlib.h>

#include <avro.h>
#include "common/common.h"
#include "common/constants.h"
#include "common/heartbeat.h"

/*
{
	"type": "record",
	"name": "HeartBeatResponse",
	"fields": [
		{"name": "completed_processes", "type": {
						"type": "array",
						"items": {
							"type": "record",
							"name": "ProcessStatus",
							"fields": [
								{"name": "name", "type": {
													"type": "record",
													"name": "ProcessName",
													"fields": [
														{"name": "jobid", "type": "int"},
														{"name": "vpid", "type": "int"}
													]
											 	  }
								},

								{"name": "state", "type": {
													    "type": "enum",
  									  					"name": "MsgType",
  									                    "symbols" : ["RUNNING", "COMPLETED"]
												  }
								},

								{"name": "exit_value", "type": "int"}
							]

						}
					 }
		}
	]
}
 */

static void build_heartbeat_response(completed_proc_t *completed_proc_array, int array_size, avro_slice_t **slice)
{
	char filename[FILE_NAME_LEN];
	char buf[BUFFER_SIZE];
	long len = 0;
	avro_schema_t schema;
	avro_value_iface_t *iface;
	avro_value_t record;
	avro_value_t completed_processes_value, ProcessStatus_value;
	avro_value_t name_value, ProcessName_value, jobid_value, vpid_value;
	avro_value_t state_value;
	avro_value_t exit_value_value;
	size_t index;
	int i;

	avro_writer_t writer;

	sprintf(filename, "%s/%s", SCHEMA_PATH, "HeartBeatResponseRecordAvro.avsc");
	init_schema(filename, &schema);

	iface = avro_generic_class_from_schema(schema);
	avro_generic_value_new(iface, &record);

	avro_value_get_by_name(&record, "completed_processes", &completed_processes_value, &index);

	for (i = 0; i < array_size; i++) {
		avro_value_append(&completed_processes_value, &ProcessStatus_value, &index);

		avro_value_get_by_name(&ProcessStatus_value, "name", &name_value, &index);
		avro_value_get_by_name(&name_value, "jobid", &jobid_value, &index);
		avro_value_set_int(&jobid_value, completed_proc_array[i].proc_name.jobid);
		avro_value_get_by_name(&name_value, "vpid", &vpid_value, &index);
		avro_value_set_int(&vpid_value, completed_proc_array[i].proc_name.vpid);

		avro_value_get_by_name(&ProcessStatus_value, "state", &state_value, &index);
		avro_value_set_enum(&state_value, completed_proc_array[i].proc_state);

		avro_value_get_by_name(&ProcessStatus_value, "exit_value", &exit_value_value, &index);
		avro_value_set_int(&exit_value_value, completed_proc_array[i].exit_value);
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


typedef struct {
	int jobid;
	int vpid;
} process_name_t;

typedef enum {
	RUNNING,
	COMPLETED
} process_state_enum_t;

typedef struct {
	process_name_t proc_name;
	process_state_enum_t proc_state;
	int exit_value;
} completed_proc_t;

//	extern void parse_heartbeat_response(avro_slice_t *slice, completed_proc_t **completed_proc_array, int *array_size);

 */

int main()
{
	avro_slice_t *slice;

	completed_proc_t completed_proc_array[3] = {
			{{101, 1}, RUNNING, 1001},
			{{201, 2}, COMPLETED, 2002},
			{{301, 3}, COMPLETED, 3003}
	};
	build_heartbeat_response(completed_proc_array, 3, &slice);
//============================
	completed_proc_t *completed_proc_array_result;
	int array_size_result = 0;
	int i;
	parse_heartbeat_response(slice, &completed_proc_array_result, &array_size_result);
	for (i = 0; i < array_size_result; i++) {
		printf("%d %d %d %d\n", completed_proc_array_result[i].proc_name.jobid, completed_proc_array_result[i].proc_name.vpid,
				completed_proc_array_result[i].proc_state, completed_proc_array_result[i].exit_value);
	}

	free_completed_proc_array(completed_proc_array_result);
//===========================
	free_slice(slice);
	return 0;
}
