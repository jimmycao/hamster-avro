#include <stdio.h>
#include <stdlib.h>

#include <avro.h>
#include "src/common.h"
#include "src/constants.h"
#include "src/allocate.h"

/*
{
	"type": "record",
	"name": "AllocateResponse",
	"fields": [
		{"name": "node_resources",  "type": {
												"type": "array",
												"items": {
															"type": "record",
															"name": "NodeResource",
															"fields": [
															           	{"name": "host_name", "type": "string"},
																	   	{"name": "slot", "type": "int"}
														              ]

														 }

											}
		}
	]
}
 */
static void build_allocate_response(node_resource_t *node_resource_array, int array_size, avro_slice_t **slice)
{
	char filename[FILE_NAME_LEN];
	char buf[BUFFER_SIZE];
	long len = 0;
	avro_schema_t schema;
	avro_value_iface_t *iface;
	avro_value_t record;
	avro_value_t node_resources_value, NodeResource_value, host_name_value, slot_value;
	size_t index;
	int i;

	avro_writer_t writer;

	sprintf(filename, "%s/%s", SCHEMA_PATH, "AllocateResponseRecordAvro.avsc");
	init_schema(filename, &schema);

	iface = avro_generic_class_from_schema(schema);
	avro_generic_value_new(iface, &record);

	avro_value_get_by_name(&record, "node_resources", &node_resources_value, &index);


	for (i = 0; i < array_size; i++) {
		avro_value_append(&node_resources_value, &NodeResource_value, &index);
		avro_value_get_by_name(&NodeResource_value, "host_name", &host_name_value, &index);
		avro_value_set_string(&host_name_value, node_resource_array[i].host_name);
		avro_value_get_by_name(&NodeResource_value, "slot", &slot_value, &index);
		avro_value_set_int(&slot_value, node_resource_array[i].slot_num);
	}

//	for (i = 0; i < array_size; i++) {
//		avro_value_get_by_name(&node_resources_value, "NodeResource", &NodeResource_value, &index);
//		printf("i = %d, index = %d\n", i, index);
//		avro_value_get_by_name(&NodeResource_value, "host_name", &host_name_value, &index);
//		avro_value_set_string(&host_name_value, node_resource_array[i].host_name);
//		avro_value_get_by_name(&NodeResource_value, "slot", &slot_value, &index);
//		avro_value_set_int(&slot_value, node_resource_array[i].slot_num);
//		avro_value_append(&node_resources_value, &NodeResource_value, &index);
//	}


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

int main()
{
	avro_slice_t *slice;

	node_resource_t node_resource_array[5] = {"node1", 1, "node2", 2, "node3", 3, "node4", 4, "node5", 5};
	int array_size = 5;
	build_allocate_response(node_resource_array, array_size, &slice);


	free_slice(slice);
	return 0;
}
