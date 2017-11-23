#include <string.h>

void *memset(void *bufptr, int value, nvmos_size_t size) {
    unsigned char *buf = (unsigned char *) bufptr;
    for (nvmos_size_t i = 0; i < size; i++)
        buf[i] = (unsigned char) value;
    return bufptr;
}
