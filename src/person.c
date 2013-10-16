#include "common.h"
#include "constants.h"
#include "register_request.h"


extern void build_person(char *name, long weight, long height, avro_slice_t *slice)
{
	char filename[128];
	char buf[4096];
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

	slice->buffer = malloc(len);
	slice->len = len;
	memcpy(slice->buffer, buf, len);
}

/*
int avro_value_get_by_name(const avro_value_t *record, const char *field_name,
                           avro_value_t *element, size_t *index);
#include <stdint.h>
#include <stdlib.h>
#include <avro.h>

int avro_value_get_boolean(const avro_value_t *value, int *dest);
int avro_value_get_bytes(const avro_value_t *value,
                         const void **dest, size_t *size);
int avro_value_get_double(const avro_value_t *value, double *dest);
int avro_value_get_float(const avro_value_t *value, float *dest);
int avro_value_get_int(const avro_value_t *value, int32_t *dest);
int avro_value_get_long(const avro_value_t *value, int64_t *dest);
int avro_value_get_null(const avro_value_t *value);
int avro_value_get_string(const avro_value_t *value,
                          const char **dest, size_t *size);
int avro_value_get_enum(const avro_value_t *value, int *dest);
int avro_value_get_fixed(const avro_value_t *value,
                         const void **dest, size_t *size);
*/
extern void print_person(avro_slice_t *slice)
{
	char filename[128];
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
