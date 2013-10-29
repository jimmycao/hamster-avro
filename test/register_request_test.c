#include <stdio.h>
#include <stdlib.h>

#include <avro.h>
#include "common/common.h"
#include "common/constants.h"
#include "common/register.h"

extern int parse_register_request(avro_slice_t *slice)
{
	char filename[FILE_NAME_LEN];
	avro_schema_t schema;
	avro_value_iface_t *iface;
	avro_value_t record;
	size_t index;
	avro_reader_t reader;
	size_t size = 0;

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

	avro_value_get_size(&record, &size);

	avro_value_iface_decref(iface);
	avro_schema_decref(schema);

	//	printf("slice->len = %d\n", slice->len);
	//	printf("size = %ld\n", size);
	//
	//	if (size > 0) {
	//		return 0;
	//	} else {
	//		return -1;
	//	}
	if (size == 0) {
		return 0;
	} else {
		return -1;
	}
}

int main()
{
	avro_slice_t *slice;

	build_register_request(&slice);
	printf("slice->len = %d\n", slice->len);
	//==============================
	int result = 999;
	result = parse_register_request(slice);
	printf("result = %d\n", result);
	//==============================

	free_slice(slice);
	return 0;
}

