#ifndef PERSON_H_
#define PERSON_H_

extern void build_person(char *name, long weight, long height, avro_slice_t **slice);

extern void parse_person(avro_slice_t *slice);

#endif /* PERSON_H_ */
