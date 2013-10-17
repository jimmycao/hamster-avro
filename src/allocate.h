#ifndef ALLOCATE_H_
#define ALLOCATE_H_

typedef struct {
	char *host_name;
	int slot_num;
} node_resource_t;

extern void build_allocate_request(int resource_count, avro_slice_t **slice);
extern void parse_allocate_response(avro_slice_t *slice, node_resource_t *node_resource_array, int array_size);

#endif /* ALLOCATE_H_ */
