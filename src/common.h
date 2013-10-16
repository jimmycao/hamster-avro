#ifndef COMMON_H_
#define COMMON_H_

#include <avro.h>

typedef struct avro_slice {
	void *buffer;
	int len;
} avro_slice_t;


extern void init_schema(char *filename, avro_schema_t *schema);


extern void write_slice_to_file(char *filename, avro_slice_t *slice);



#endif /* COMMON_H_ */
