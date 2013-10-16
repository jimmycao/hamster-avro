#include "common.h"
#include "constants.h"


extern void build_hnp_request_wrapper(avro_slice_t *inner_slice, int type, avro_slice_t *wrapper_slice)
{
	char filename[128];
	char buf[4096];
	long len = 0;
	avro_schema_t schema;
	avro_value_iface_t *iface;
	avro_value_t record;
	avro_value_t request_value;
	avro_value_t msg_type_value;
	size_t index;
	avro_writer_t writer;

	sprintf(filename, "%s/%s", SCHEMA_PATH, "HamsterHnpRequestRecordAvro.avsc");
	init_schema(filename, &schema);

	iface = avro_generic_class_from_schema(schema);
	avro_generic_value_new(iface, &record);

	avro_value_get_by_name(&record, "request", &request_value, &index);
	avro_value_set_bytes(&request_value, inner_slice->buffer, inner_slice->len);
	avro_value_get_by_name(&record, "msg_type_value", &msg_type_value, &index);
	avro_value_set_enum(&msg_type_value, type);

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

	wrapper_slice->buffer = malloc(len);
	wrapper_slice->len = len;
	memcpy(wrapper_slice->buffer, buf, len);
}

