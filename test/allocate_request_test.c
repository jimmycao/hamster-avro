#include <stdio.h>
#include <stdlib.h>

#include <avro.h>
#include "common/base/common.h"
#include "common/base/constants.h"
#include "common/base/allocate.h"



static void parse_allocate_request(avro_slice_t *slice, int *resource_request_count)
{
	char filename[FILE_NAME_LEN];
	avro_schema_t schema;
	avro_value_iface_t *iface;
	avro_value_t record, field_value;
	size_t index;
	avro_reader_t reader;

	char* avro_schema_path = getenv("AVRO_SCHEMA_PATH");
	if (!avro_schema_path) {
		fprintf(stderr, "Can not get AVRO_SCHEMA_PATH from env");
		exit(1);
	}
	sprintf(filename, "%s/%s", avro_schema_path, "RegisterRequestRecordAvro.avsc");
	init_schema(filename, &schema);


	iface = avro_generic_class_from_schema(schema);
	avro_generic_value_new(iface, &record);

	reader = avro_reader_memory(slice->buffer, slice->len);
	if (avro_value_read(reader, &record)) {
		fprintf(stderr, "Unable to read record from memory buffer\n");
		fprintf(stderr, "Error: %s\n", avro_strerror());
		exit(1);
	}

	avro_value_get_by_name(&record, "resource_count", &field_value, &index);
	avro_value_get_int(&field_value, resource_request_count);

	//avro_generic_value_free(&record);
	avro_value_iface_decref(iface);
	avro_schema_decref(schema);
}



int main()
{
	int resource_count = 20;
	int resource_request_count = 0;
	avro_slice_t *slice;

	build_allocate_request(resource_count, &slice);
	parse_allocate_request(slice, &resource_request_count);
	printf("resource_request_count = %d\n", resource_request_count);

	free_slice(slice);
	return 0;
}
