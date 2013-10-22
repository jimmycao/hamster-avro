#include <stdio.h>
#include <stdlib.h>

#include <avro.h>
#include "src/common.h"
#include "src/constants.h"
#include "src/launch.h"

//typedef struct {
//	char **en_vars_array;  /* ended with NULL */
//	char *args;
//	char *host_name;
//	process_name_t proc_name;
//} launch_context_t;

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
		 		  					{"name": "en_vars", "type": "array", "items": "string"},
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
extern void parse_launch_request(avro_slice_t *slice, launch_context_t **launch_context_array, int *array_size)
{
	char filename[FILE_NAME_LEN];
	avro_schema_t schema;
	avro_value_iface_t *iface;
	avro_reader_t reader;
	avro_value_t record;
	size_t index;
	avro_value_t launchContexts_value, launchContext_value, en_vars_array_value, en_vars_item_value, args_value, host_name_value;
	avro_value_t name_value, jobid_value, vpid_value;
	int i, j;

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
			int en_vars_array_size = 0;

			avro_value_get_by_index(&launchContexts_value, i, &launchContext_value, NULL);

			avro_value_get_by_name(&launchContext_value, "en_vars", &en_vars_array_value, &index);
			avro_value_get_size(&en_vars_array_value, &en_vars_array_size);
			/* malloc 1 more char * for the ending NULL */
			(*launch_context_array)[i].en_vars_array = (char*) xmalloc(sizeof(char*) * (en_vars_array_size+1));
			for (j = 0; j < en_vars_array_size; j++) {
				avro_value_get_by_index(&en_vars_array_value, j, &en_vars_item_value, NULL);
				avro_value_get_string(&en_vars_item_value, &((*launch_context_array)[i].en_vars_array[j]), &size);
			}
			(*launch_context_array)[i].en_vars_array[en_vars_array_size] = NULL;

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


int main()
{
	avro_slice_t *slice = NULL;
	launch_context_t *launch_context_array;

	launch_context_array = xmalloc(sizeof(launch_context_t) * 3);

	launch_context_array[0].en_vars_array = (char*)xmalloc(sizeof(char*) * 3);
	launch_context_array[0].en_vars_array[0] = strdup("var11");
	launch_context_array[0].en_vars_array[1] = strdup("var12");
	launch_context_array[0].en_vars_array[2] = NULL;
	launch_context_array[0].args = strdup("args11,args12");
	launch_context_array[0].host_name = strdup("host1");
	launch_context_array[0].proc_name.jobid = 101;
	launch_context_array[0].proc_name.vpid = 1;

	launch_context_array[1].en_vars_array = (char*) xmalloc(sizeof(char*) * 3);
	launch_context_array[1].en_vars_array[0] = strdup("var21");
	launch_context_array[1].en_vars_array[1] = strdup("var22");
	launch_context_array[1].en_vars_array[2] = NULL;
	launch_context_array[1].args = strdup("args21,args22");
	launch_context_array[1].host_name = strdup("host2");
	launch_context_array[1].proc_name.jobid = 201;
	launch_context_array[1].proc_name.vpid = 2;

	launch_context_array[2].en_vars_array = (char*) xmalloc(sizeof(char*) * 3);
	launch_context_array[2].en_vars_array[0] = strdup("var31");
	launch_context_array[2].en_vars_array[1] = strdup("var32");
	launch_context_array[2].en_vars_array[2] = NULL;
	launch_context_array[2].args = strdup("args31,args32");
	launch_context_array[2].host_name = strdup("host2");
	launch_context_array[2].proc_name.jobid = 201;
	launch_context_array[2].proc_name.vpid = 2;


	build_launch_request(launch_context_array, 3, &slice);
	free_launch_context_array(launch_context_array, 3);
//===================================
	launch_context_t *launch_context_result_array;
	int array_size_result;
	int i;

	parse_launch_request(slice, &launch_context_result_array, &array_size_result);
	for (i = 0; i < array_size_result; i++) {
		while (*(launch_context_result_array[i].en_vars_array)) {
			puts(*(launch_context_result_array[i].en_vars_array));
			launch_context_result_array[i].en_vars_array++;
		}
		printf("%s %s %d %d\n", launch_context_result_array[i].args, launch_context_result_array[i].host_name,
				launch_context_result_array[i].proc_name.jobid,
				launch_context_result_array[i].proc_name.vpid);

		putchar('\n');

	}
	free_launch_context_array(launch_context_result_array, array_size_result);
//===================================
	free_slice(slice);
	return 0;
}
