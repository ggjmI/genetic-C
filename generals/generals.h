#include <stdlib.h>

#ifndef GENERALS_H
#define GENERALS_H

/* General purpose functions. */

void
check_null(void *ptr, int line, char *file);
/* Basic function to check if a ptr is NULL. */
void
*ec_malloc(size_t size, int line, char *file);
/* Error checked malloc. */
void
*ec_calloc(int len, size_t size, int line, char *file);
/* Error checked calloc. */
void
shuffle_arr(void *arr0, int length, size_t size);
/* Function to 'riffle shuffle' an array. */

#endif /* GENERALS_H */
