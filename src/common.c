#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include <avro.h>

static void read_file(char *filename, avro_slice_t* slice)
{
	FILE *fp;

	if (NULL == (fp = fopen(filename, "r"))) {
		fprintf(stderr, "%s :Open file failed.\n", filename);
		exit(1);
	}

	fseek(fp, 0, SEEK_END);
	slice->len = ftell(fp);
	rewind(fp);
	slice->buffer = malloc(slice->len);
	fread(slice->buffer, 1, slice->len, fp);
	fclose(fp);
}


/**
 *  init schema with a schema file
 */
extern void init_schema(char *filename, avro_schema_t *schema)
{
	avro_slice_t schema_slice;
	read_file(filename, &schema_slice);
	if (avro_schema_from_json(schema_slice.buffer, 0, schema, NULL)) {
		fprintf(stderr, "Unable to parse schema. \n");
		exit(1);
	}
	if (schema_slice.buffer) {
		free(schema_slice.buffer);
	}
}

extern void write_slice_to_file(char *filename, avro_slice_t *slice)
{
	FILE *fp;

	remove(filename);
	if (NULL == (fp = fopen(filename, "w"))) {
		fprintf(stderr, "%s : Open file failed.\n", filename);
		exit(1);
	}

	fwrite(slice->buffer, 1, slice->len, fp);
	fclose(fp);
}
