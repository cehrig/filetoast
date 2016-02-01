//
// Created by Christian Ehrig on 31/01/16.
//

#ifndef FILETOAST_THREAD_H
#define FILETOAST_THREAD_H

#include <pthread.h>

#define THREAD_UNASSIGNED -1

typedef enum
{
    THREAD_STARTUP,
    THREAD_IDLE,
    THREAD_BUSY
} THREAD_STATUS;

typedef struct threadinfo
{
    int id;
    pthread_t thread;
    THREAD_STATUS status;
} t_threadinfo;

typedef struct supervisor
{
    t_threadinfo ** threads;
    pthread_mutex_t lock;
    struct jobs * jobs;
    short dirc;
    char ** directories;
    short patc;
    char ** patterns;
} t_supervisor;

typedef struct threadarg
{
    t_supervisor * supervisor;
    t_threadinfo * threadinfo;
} t_threadarg;

t_supervisor * tsetup(short, short, char **, char **);

void globfs(t_supervisor *);
void assign(t_supervisor *);
int retthread();


void tjoin(t_supervisor *);

#endif //FILETOAST_THREAD_H
