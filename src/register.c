#include "common.h"
#include "constants.h"
#include "register.h"

extern void build_register_request(avro_slice_t *slice)
{
	char filename[FILE_NAME_LEN];
	char buf[BUFFER_SIZE];
	long len = 0;
	avro_schema_t schema;
	avro_value_iface_t *iface;
	avro_value_t record;
	avro_writer_t writer;

	sprintf(filename, "%s/%s", SCHEMA_PATH, "RegisterRequestRecordAvro.avsc");
	init_schema(filename, &schema);

	iface = avro_generic_class_from_schema(schema);
	avro_generic_value_new(iface, &record);

	/* create a writer with memory buffer */
	writer = avro_writer_memory(buf, sizeof(buf));
	/* write record to writer (buffer) */
	if (avro_value_write(writer, &record)) {
		fprintf(stderr, "Unable to write person record to memory buffer\n");
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

extern int get_register_response(avro_slice_t *slice)
{
	char filename[FILE_NAME_LEN];
	avro_schema_t schema;
	avro_value_iface_t *iface;
	avro_value_t record;

	size_t index;
	avro_reader_t reader;

	size_t size = 0;


	sprintf(filename, "%s/%s", SCHEMA_PATH, "RegisterResponseRecordAvro.avsc");
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

	if (size > 0) {
		return 0;
	} else {
		return -1;
	}
}
