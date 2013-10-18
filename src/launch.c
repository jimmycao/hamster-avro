#include "common.h"
#include "constants.h"
#include "launch.h"

/*
{
	"type": "record",
	"name": "LaunchRequest",
	"fields": [
		{
		 	"name": "launchContexts",
		 	"type": {
		          		"type": "array",
		 		  		"items": {
		 		  				"type": "record",
		 		  				"name": "LaunchContext",
		 		  				"fields": [
		 		  					{"name": "en_vars", "type" : "string"},
		 		  					{"name": "args", "type": "string"},
		 		  					{"name": "host_name", "type": "string"},
		 		  					{"name": "name", "type": {
		 		  										"type": "record",
		 		  										"name": "ProcessName",
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



 */

extern void build_launch_request(launch_context_t *launch_context_array, int array_size, avro_slice_t **slice)
{
	char filename[FILE_NAME_LEN];
	char buf[BUFFER_SIZE];
	long len = 0;
	avro_schema_t schema;
	avro_value_iface_t *iface;
	avro_value_t record;
	avro_value_t launchContexts_value, launchContext_value, en_vars_value, args_value, host_name_value;
	avro_value_t name_value, ProcessName_value, jobid_value, vpid_value;
	size_t index;
	avro_writer_t writer;
	int i;

	sprintf(filename, "%s/%s", SCHEMA_PATH, "LaunchRequestRecordAvro.avsc");
	init_schema(filename, &schema);

	iface = avro_generic_class_from_schema(schema);
	avro_generic_value_new(iface, &record);

	avro_value_get_by_name(&record, "launchContexts", &launchContexts_value, &index);

	for (i = 0; i < array_size; i++) {
		avro_value_append(&launchContexts_value, &launchContext_value, &index);

		avro_value_get_by_name(&launchContext_value, "en_vars", &en_vars_value, &index);
		avro_value_set_string(&en_vars_value, launch_context_array[i].en_vars);

		avro_value_get_by_name(&launchContext_value, "args", &args_value, &index);
		avro_value_set_string(&args_value, launch_context_array[i].args);

		avro_value_get_by_name(&launchContext_value, "host_name", &host_name_value, &index);
		avro_value_set_string(&host_name_value, launch_context_array[i].host_name);

		avro_value_get_by_name(&launchContext_value, "name", &name_value, &index);
		avro_value_get_by_name(&name_value, "ProcessName", &ProcessName_value, &index);

		avro_value_get_by_name(&ProcessName_value, "jobid", &jobid_value, &index);
		avro_value_set_int(&jobid_value, launch_context_array[i].proc_name.jobid);

		avro_value_get_by_name(&ProcessName_value, "vpid", &vpid_value, &index);
		avro_value_set_int(&vpid_value, launch_context_array[i].proc_name.vpid);
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
