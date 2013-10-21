#include <stdio.h>
#include <stdlib.h>

#include <avro.h>
#include "src/common.h"
#include "src/constants.h"
#include "src/hnp_request_wrapper.h"

/*
{
	"type": "record",
	"name": "HamsterHnpRequest",
	"fields": [
		{"name": "request",  "type": "bytes"},
		{"name": "msg_type", "type": {"type": "enum",
  									  "name": "MsgType",
  									  "symbols" : ["ALLOCATE", "LAUNCH", "REGISTER", "FINISH", "HEARTBEAT"]
								      }
		}
	]
}
 */

extern void parse_hnp_request_wrapper(avro_slice_t *wrapper_slice, avro_slice_t **inner_slice, msg_type_enum_t *msg_type)
{
	char filename[FILE_NAME_LEN];
	avro_schema_t schema;
	avro_value_iface_t *iface;
	avro_reader_t reader;
	avro_value_t record;
	size_t index;
	avro_value_t request_value, msg_type_value;
	size_t size;

	sprintf(filename, "%s/%s", SCHEMA_PATH, "HamsterHnpRequestRecordAvro.avsc");
	init_schema(filename, &schema);

	iface = avro_generic_class_from_schema(schema);
	avro_generic_value_new(iface, &record);

	reader = avro_reader_memory(wrapper_slice->buffer, wrapper_slice->len);
	if (avro_value_read(reader, &record)) {
		fprintf(stderr, "Unable to read record from memory buffer\n");
		fprintf(stderr, "Error: %s\n", avro_strerror());
		exit(1);
	}

	*inner_slice = xmalloc(sizeof(avro_slice_t));
	avro_value_get_by_name(&record, "request", &request_value, &index);
	avro_value_get_bytes(&request_value, &((*inner_slice)->buffer), &size);
	(*inner_slice)->len = size;
	avro_value_get_by_name(&record, "msg_type", &msg_type_value, &index);
	avro_value_get_enum(&msg_type_value, msg_type);

	//avro_generic_value_free(&record);
	avro_value_iface_decref(iface);
	avro_schema_decref(schema);
}

int main()
{
	int len = strlen("pivotal");
	avro_slice_t inner_slice = {"pivotal", len};

	avro_slice_t *wrapper_slice;
	msg_type_enum_t msg_type = REGISTER;
	build_hnp_request_wrapper(&inner_slice, msg_type, &wrapper_slice);
	//========================
	avro_slice_t *inner_slice_result;
	msg_type_enum_t msg_type_result;
	parse_hnp_request_wrapper(wrapper_slice, &inner_slice_result, &msg_type_result);
	printf("inner_slice_result->buffer = %s\n", (char*)inner_slice_result->buffer);
	printf("inner_slice_result->len = %d\n", inner_slice_result->len);
	printf("msg_type_result = %d\n", msg_type_result);
	free_slice(inner_slice_result);
	//========================
	free_slice(wrapper_slice);

	return 0;
}


