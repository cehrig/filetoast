//
// Created by Christian Ehrig on 29/01/16.
//

#ifndef XML2QUEUE_LOG_H
#define XML2QUEUE_LOG_H

#include <errno.h>
#include <stdio.h>
#include <string.h>

#define LOGFILE "/var/log/filetoast.log"

typedef enum
{
    LOG_DEFAULT,
    LOG_DEBUG,
    LOG_ERROR,
    LOG_CRITICAL
} LOG_LEVEL;

FILE * openlog(int);
void writelog(LOG_LEVEL level, const char *, ...);

#endif //XML2QUEUE_LOG_H
