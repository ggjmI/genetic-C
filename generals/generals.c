#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <fcntl.h>
#include "generals.h"

void
check_null(void *ptr, int line, char *file)
{
/* Simple function to check if a argument pointer
   is equal NULL, preventing nasty bugs. */
    if (ptr == NULL)
    {
        printf("NULL Value returned in line %d of file \"%s\"!\n",
               line, file);
        exit(1);
    }
}

void
*ec_malloc(size_t size, int line, char *file)
{
/* Error checked malloc. */
    void *ptr;
    ptr = malloc(size);
    check_null(ptr, line, file);
    return ptr;
}

void
*ec_calloc(int len, size_t size, int line, char *file)
{
/* Error checked calloc. */
    void *ptr;
    ptr = calloc(len, size);
    check_null(ptr, line, file);
    return ptr;
}

void
shuffle_arr(void *arr0, int length, size_t size)
{
/* Function to 'riffle shuffle' an array. */
    if (length <= 1)
    {
    }
    else
    {
        char *arr = arr0;
        char tmp[size];
        int i, j, n_shuffle = 7; /* 7 is magic! */
        int first_half = length/2;
        int second_half = length - first_half;
        size_t card1, card2;

        for (i = 0; i < n_shuffle; i++)
        {
            for (j = 0; j < length; j++)
            {
                card1 = (size_t) rand() % first_half;
                card2 = (size_t) rand() % second_half + first_half;
                memcpy(tmp, arr + card1 * sizeof(char) * size, size);
                memcpy(arr + card1 * sizeof(char) * size,
                       arr + card2 * sizeof(char) * size,
                       size);
                memcpy(arr + card2 * sizeof(char) * size, tmp, size);
            }
        }
    }
}

int
get_random_int()
{
/* Simple function to get 4 random bytes from
   /dev/urandom and use it to return a positive int. */

    FILE *urandom_data = fopen("/dev/urandom", "r");
    check_null(urandom_data, __LINE__, __FILE__);

    /* must consider \0 due to fread call */
    int value, *random_int = malloc(sizeof(int) + 1);
    size_t result = fread(random_int, sizeof(int),
                          1, urandom_data);
    if (result < 1)
    {
        printf("Could not read one integer"
               " from /dev/urandom!\n");
        exit(1);
    }
    value = *random_int;
    if (value < 0)
        value = -value; /* Only positive values. */

    fclose(urandom_data);
    free(random_int);
    return value;
}

float
get_random_float()
{
/* Simple function to return float value from 0.0 to 1.0
   made from 'get_random_int' divided by INT_MAX. */

    int random_int = get_random_int();
    float random_float;

    /* Double conversion to reduce bad rounding probability. */
    random_float = (float) ((double) random_int / (double) INT_MAX);
    return random_float;
}
