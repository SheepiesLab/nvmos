#include <string.h>

nvmos_size_t strlen(const char *str) {
    nvmos_size_t len = 0;
    while (str[len])
        len++;
    return len;
}
