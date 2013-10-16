#include <stdio.h>
#include <stdlib.h>

#include <avro.h>
#include "src/common.h"
#include "src/person.h"

int main()
{
	char *name = "xixi";
	long weight = 70;
	long height = 180;
	avro_slice_t slice;
	avro_slice_t wrapper_slice;
	char *db_filename = "/Users/caoj7/workspace/hamster-avro/data/person.db";

	build_person(name, weight, height, &slice);
	print_person(&slice);

	if (slice.buffer) {
		free(slice.buffer);
	}
	return 0;
}




