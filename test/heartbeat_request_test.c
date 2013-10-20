#include <stdio.h>
#include <stdlib.h>

#include <avro.h>
#include "src/common.h"
#include "src/constants.h"
#include "src/heartbeat.h"



int main()
{
	avro_slice_t *slice;

	build_heartbeat_request(&slice);
	//==============================

	//==============================

	free_slice(slice);
	return 0;
}

