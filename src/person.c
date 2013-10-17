#include "common.h"
#include "constants.h"

extern void build_person(char *name, long weight, long height, avro_slice_t **slice)
{
	char filename[FILE_NAME_LEN];
	char buf[BUFFER_SIZE];
	long len = 0;
	avro_schema_t schema;
	avro_value_iface_t *iface;
	avro_value_t record;
	avro_value_t name_value;
	avro_value_t weight_value;
	avro_value_t height_value;
	size_t index;
	avro_writer_t writer;

	sprintf(filename, "%s/%s", SCHEMA_PATH, "person.schema");
	init_schema(filename, &schema);

	iface = avro_generic_class_from_schema(schema);
	avro_generic_value_new(iface, &record);

	avro_value_get_by_name(&record, "name", &name_value, &index);
	avro_value_set_string(&name_value, name);

	avro_value_get_by_name(&record, "weight", &weight_value, &index);
	avro_value_set_long(&weight_value, weight);

	avro_value_get_by_name(&record, "height", &height_value, &index);
	avro_value_set_long(&height_value, height);


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


extern void parse_person(avro_slice_t *slice)
{
	char filename[FILE_NAME_LEN];
	avro_schema_t schema;
	avro_value_iface_t *iface;
	avro_value_t record;
	avro_value_t name_value;
	avro_value_t weight_value;
	avro_value_t height_value;
	size_t index;
	avro_reader_t reader;
	char *name;
	size_t size = 0;
	long weight;
	long height;

	sprintf(filename, "%s/%s", SCHEMA_PATH, "person.schema");
	init_schema(filename, &schema);

	iface = avro_generic_class_from_schema(schema);
	avro_generic_value_new(iface, &record);

	reader = avro_reader_memory(slice->buffer, slice->len);
	if (avro_value_read(reader, &record)) {
		fprintf(stderr, "Unable to read record from memory buffer\n");
		fprintf(stderr, "Error: %s\n", avro_strerror());
		exit(1);
	}

	avro_value_get_by_name(&record, "name", &name_value, &index);
	avro_value_get_string(&name_value, &name, &size);

	avro_value_get_by_name(&record, "height", &height_value, &index);
	avro_value_get_long(&height_value, &height);

	avro_value_get_by_name(&record, "weight", &weight_value, &index);
	avro_value_get_long(&weight_value, &weight);

	//avro_generic_value_free(&record);
	avro_value_iface_decref(iface);
	avro_schema_decref(schema);

	printf("name = %s, height = %ld, weight = %ld\n", name, height, weight);
	if (name) {
		free(name);
	}
}
