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

extern void parse_launch_request(avro_slice_t *slice, launch_context_t **launch_context_array, int *array_size)
{
	char filename[FILE_NAME_LEN];
	avro_schema_t schema;
	avro_value_iface_t *iface;
	avro_reader_t reader;
	avro_value_t record;
	size_t index;
	avro_value_t launchContexts_value, launchContext_value, en_vars_value, args_value, host_name_value;
	avro_value_t name_value, jobid_value, vpid_value;
	int i;

	sprintf(filename, "%s/%s", SCHEMA_PATH, "LaunchRequestRecordAvro.avsc");
	init_schema(filename, &schema);

	iface = avro_generic_class_from_schema(schema);
	avro_generic_value_new(iface, &record);

	reader = avro_reader_memory(slice->buffer, slice->len);
	if (avro_value_read(reader, &record)) {
		fprintf(stderr, "Unable to read record from memory buffer\n");
		fprintf(stderr, "Error: %s\n", avro_strerror());
		exit(1);
	}

	avro_value_get_by_name(&record, "launchContexts", &launchContexts_value, &index);
	avro_value_get_size(&launchContexts_value, array_size);

	if (0 == *array_size) {
		*launch_context_array = NULL;
	} else {
		*launch_context_array = xmalloc(sizeof(launch_context_t) * (*array_size));
		for (i = 0; i < *array_size; i++) {
			size_t size;
			avro_value_get_by_index(&launchContexts_value, i, &launchContext_value, NULL);
			avro_value_get_by_name(&launchContext_value, "en_vars", &en_vars_value, &index);
			avro_value_get_string(&en_vars_value, &((*launch_context_array)[i].en_vars), &size);

			avro_value_get_by_name(&launchContext_value, "args", &args_value, &index);
			avro_value_get_string(&args_value, &((*launch_context_array)[i].args), &size);

			avro_value_get_by_name(&launchContext_value, "host_name", &host_name_value, &index);
			avro_value_get_string(&host_name_value, &((*launch_context_array)[i].host_name), &size);

			avro_value_get_by_name(&launchContext_value, "name", &name_value, &index);

			avro_value_get_by_name(&name_value, "jobid", &jobid_value, &index);
			avro_value_get_int(&jobid_value, &((*launch_context_array)[i].proc_name.jobid));

			avro_value_get_by_name(&name_value, "vpid", &jobid_value, &index);
			avro_value_get_int(&jobid_value, &((*launch_context_array)[i].proc_name.vpid));
		}
	}

	//avro_generic_value_free(&record);
	avro_value_iface_decref(iface);
	avro_schema_decref(schema);
}

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
 */

int main()
{
	avro_slice_t *slice;
	launch_context_t launch_context_array[3] = {
			{"var1", "args1", "host1", {101, 1}},
			{"var2", "args2", "host2", {102, 2}},
			{"var3", "args3", "host3", {103, 3}}
			};

	build_launch_request(launch_context_array, 3, &slice);
//===================================
	launch_context_t *launch_context_result_array;
	int array_size_result;
	int i;

	parse_launch_request(slice, &launch_context_result_array, &array_size_result);
	for (i = 0; i < array_size_result; i++) {
		printf("%s %s %s %d %d\n", launch_context_result_array[i].en_vars, launch_context_result_array[i].args,
				launch_context_result_array[i].host_name, launch_context_result_array[i].proc_name.jobid,
				launch_context_result_array[i].proc_name.vpid);

	}
	free_launch_context_array(launch_context_result_array, array_size_result);
//===================================
	free_slice(slice);
	return 0;
}
