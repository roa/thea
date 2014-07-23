#ifndef THEA_LOGGER_H
#define THEA_LOGGER_H

#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <config.h>

#define LOG_BUFFER_SIZE 100

struct Logger
{
    const char *fname;
    int fd;
};

typedef struct Logger Logger;

void logger_init();
void logger_deinit();
void logger_log(const char *fmt, ...);
void logger_dump(const char *fmt, ...);

#define buffer_set_timestamp(_buf, _len)                         \
    do {                                                         \
        char *_timestr;                                          \
        struct tm *_local;                                       \
        time_t _time = time(NULL);                               \
        _local = localtime(&_time);                              \
        _timestr = asctime(_local);                              \
        _len += snprintf(_buf, LOG_BUFFER_SIZE, "%s", _timestr); \
        _buf[_len - 1] = ' ';                                    \
    } while (0)

#define logger_write(_fd, _buf, _len)          \
    do {                                       \
        if (_len > LOG_BUFFER_SIZE)            \
            _len = LOG_BUFFER_SIZE;            \
        if (write(_fd, _buf, _len) < 0)        \
            fprintf(                           \
                stderr,                        \
                "while writing to logfile %s", \
                strerror(errno)                \
            );                                 \
    } while(0)

#endif // THEA_LOGGER_H
