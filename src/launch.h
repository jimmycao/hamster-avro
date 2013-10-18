/*
 * launch.h
 *
 *  Created on: Oct 17, 2013
 *      Author: caoj7
 */

#ifndef LAUNCH_H_
#define LAUNCH_H_

typedef struct {
	int jobid;
	int vpid;
} process_name_t;

typedef struct {
	char *en_vars;
	char *args;
	char *host_name;
	process_name_t proc_name;
} launch_context_t;


extern void build_launch_request(launch_context_t *launch_context_array, int array_size, avro_slice_t **slice);

extern void free_launch_context_array(launch_context_t *launch_context_array, int array_size);


#endif /* LAUNCH_H_ */
