#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#if HAVE_STDBOOL_H
#  include <stdbool.h>
#else
typedef enum {false, true} bool;
#endif /* !HAVE_STDBOOL_H */

#define SCHEMA_PATH "/Users/caoj7/program/hamster-avro/schema"

#define FILE_NAME_LEN 256
#define BUFFER_SIZE   8192


#endif /* CONSTANTS_H_ */
