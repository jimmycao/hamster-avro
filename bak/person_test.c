#include <stdio.h>
#include <stdlib.h>

#include <avro.h>
#include "src/common.h"
#include "src/person.h"
#include "src/constants.h"

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

int main()
{
	char *name = "xixi";
	long weight = 70;
	long height = 180;
	avro_slice_t *slice;
//	avro_slice_t wrapper_slice;
	char *db_filename = "/Users/caoj7/workspace/hamster-avro/data/person.db";

	build_person(name, weight, height, &slice);

	parse_person(slice);

//	write_slice_to_file(db_filename, slice);
	free_slice(slice);

	return 0;
}
