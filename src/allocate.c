#include "common.h"
#include "constants.h"
#include "allocate.h"


extern void build_allocate_request(int resource_count, avro_slice_t *slice)
{
	char filename[FILE_NAME_LEN];
	char buf[BUFFER_SIZE];
	long len = 0;
	avro_schema_t schema;
	avro_value_iface_t *iface;
	avro_value_t record;
	avro_value_t resource_count_value;
	size_t index;
	avro_writer_t writer;

	sprintf(filename, "%s/%s", SCHEMA_PATH, "AllocateRequestRecordAvro.avsc");
	init_schema(filename, &schema);

	iface = avro_generic_class_from_schema(schema);
	avro_generic_value_new(iface, &record);

	avro_value_get_by_name(&record, "resource_count", &resource_count_value, &index);
	avro_value_set_int(&resource_count_value, resource_count);


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

	slice->buffer = malloc(len);
	slice->len = len;
	memcpy(slice->buffer, buf, len);
}
