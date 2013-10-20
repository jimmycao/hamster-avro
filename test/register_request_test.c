#include <stdio.h>
#include <stdlib.h>

#include <avro.h>
#include "src/common.h"
#include "src/constants.h"
#include "src/register.h"

//static void parse_finish_request(avro_slice_t *slice, bool *succeed, char **diag)
//{
//	char filename[FILE_NAME_LEN];
//	avro_schema_t schema;
//	avro_value_iface_t *iface;
//	avro_value_t record, succeed_value, diag_value;
//	size_t index;
//	avro_reader_t reader;
//	size_t size;
//
//	sprintf(filename, "%s/%s", SCHEMA_PATH, "FinishRequestRecordAvro.avsc");
//	init_schema(filename, &schema);
//
//	iface = avro_generic_class_from_schema(schema);
//	avro_generic_value_new(iface, &record);
//
//	reader = avro_reader_memory(slice->buffer, slice->len);
//	if (avro_value_read(reader, &record)) {
//		fprintf(stderr, "Unable to read record from memory buffer\n");
//		fprintf(stderr, "Error: %s\n", avro_strerror());
//		exit(1);
//	}
//
//	avro_value_get_by_name(&record, "succeed", &succeed_value, &index);
//	avro_value_get_boolean(&succeed_value, succeed);
//
//	avro_value_get_by_name(&record, "diagnostics", &diag_value, &index);
//	avro_value_get_string(&diag_value, diag, &size);
//
//	//avro_generic_value_free(&record);
//	avro_value_iface_decref(iface);
//	avro_schema_decref(schema);
//}

int main()
{
	avro_slice_t *slice;

	build_register_request(&slice);
	//==============================
//	bool succeed_result;
//	char *diag_result;
//	parse_finish_request(slice, &succeed_result, &diag_result);
//	printf("succeed_result = %d\n", succeed_result);
//	printf("diag_result = %s\n", diag_result);
//	if (diag_result) {
//		free(diag_result);
//	}
	//==============================

	free_slice(slice);
	return 0;
}

