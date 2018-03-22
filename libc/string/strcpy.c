
#include <string.h>

char *strcpy(char *destination, const char *source)
{
    int i = 0;
    for (; source[i] != '\0'; ++i)
    {
        destination[i] = source[i];
    }
    destination[i] = NULL;
}