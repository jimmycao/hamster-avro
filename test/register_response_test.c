#include <stdio.h>
#include <stdlib.h>

#include <avro.h>
#include "common/common.h"
#include "common/constants.h"
#include "common/register.h"

extern void build_register_response(avro_slice_t **slice)
{
	char filename[FILE_NAME_LEN];
	char buf[BUFFER_SIZE];
	long len = 0;
	avro_schema_t schema;
	avro_value_iface_t *iface;
	avro_value_t record;
	size_t index;
	avro_writer_t writer;

	sprintf(filename, "%s/%s", SCHEMA_PATH, "RegisterResponseRecordAvro.avsc");
	init_schema(filename, &schema);

	iface = avro_generic_class_from_schema(schema);
	avro_generic_value_new(iface, &record);

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

	build_register_response(&slice);
	printf("slice->len = %d\n", slice->len);
	//==============================
	int result = 999;
	result = parse_register_response(slice);
	printf("result = %d\n", result);
	//==============================

	free_slice(slice);
	return 0;
}

