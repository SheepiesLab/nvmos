#include <limits.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <kernel/file.h>

static bool print(const char *data, size_t length) {
    const unsigned char *bytes = (const unsigned char *) data;
    for (size_t i = 0; i < length; i++)
        if (putchar(bytes[i]) == EOF)
            return false;
    return true;
}

char int2hex(int x, bool lowercase) {
    if (x >= 0 && x <= 9) {
        return (char) (48 + x);
    } else if (x >= 10 && x <= 15) {
        if (lowercase) {
            return (char) (97 + x - 10);
        } else {
            return (char) (65 + x - 10);
        }
    }
    return 'x';
}

char int2dec(int x) {
    if (x >= 0 && x <= 9) {
        return (char) (48 + x);
    }
    return 'x';
}

int printf(const char *restrict format, ...) {
    va_list parameters;
    va_start(parameters, format);

    int written = 0;

    while (*format != '\0') {
        size_t maxrem = INT_MAX - written;

        if (format[0] != '%' || format[1] == '%') {
            if (format[0] == '%')
                format++;
            size_t amount = 1;
            while (format[amount] && format[amount] != '%')
                amount++;
            if (maxrem < amount) {
                // TODO: Set errno to EOVERFLOW.
                return -1;
            }
            if (!print(format, amount))
                return -1;
            format += amount;
            written += amount;
            continue;
        }

        const char *format_begun_at = format++;

        if (*format == 'c') {
            format++;
            char c = (char) va_arg(parameters, int /* char promotes to int */);
            if (!maxrem) {
                // TODO: Set errno to EOVERFLOW.
                return -1;
            }
            if (!print(&c, sizeof(c)))
                return -1;
            written++;
        } else if (*format == 's') {
            format++;
            const char *str = va_arg(parameters, const char*);
            size_t len = strlen(str);
            if (maxrem < len) {
                // TODO: Set errno to EOVERFLOW.
                return -1;
            }
            if (!print(str, len))
                return -1;
            written += len;
        } else if (*format == 'p') {
            format++;
            uint64_t ptr = va_arg(parameters, const uint64_t);
            size_t len = sizeof(ptr) * 2;
            if (maxrem < len) {
                // TODO: Set errno to EOVERFLOW.
                return -1;
            }
            char str[len];
            for (int i = len - 1; i >= 0; --i) {
                str[i] = int2hex(ptr % 16, false);
                ptr = ptr >> 4;
            }
            if (!print(str, len))
                return -1;
            written += len;
        } else if (*format == 'd') { //TODO: 64bit incompatible
            format++;
            uint32_t num = va_arg(parameters, const uint32_t);
            size_t len = 0;
            char str[19];
            while (num != 0) {
                str[18 - len] = int2dec(num % 10);
                num = num / 10;
                ++len;
            }
            if (maxrem < len) {
                // TODO: Set errno to EOVERFLOW.
                return -1;
            }
            if (!print((str + 19 - len), len))
                return -1;
            written += len;
        } else {
            format = format_begun_at;
            size_t len = strlen(format);
            if (maxrem < len) {
                // TODO: Set errno to EOVERFLOW.
                return -1;
            }
            if (!print(format, len))
                return -1;
            written += len;
            format += len;
        }
    }

    va_end(parameters);
    return written;
}
