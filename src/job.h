//
// Created by Christian Ehrig on 31/01/16.
//

#ifndef FILETOAST_JOB_H
#define FILETOAST_JOB_H

typedef enum
{
    JOB_READY,
    JOB_PROCESSING,
    JOB_FINISHED
} JOB_STATUS;

typedef struct job
{
    int thread;
    char * path;
    char * file;
    char * fullpath;
    char * content;
    JOB_STATUS status;
    struct job * next;
} t_job;

typedef struct jobs
{
    int num;
    t_job * job;
} t_jobs;

t_job * jobcreate(char *, char *);
void jobadd(t_supervisor *, t_job *);
void jobassign(t_supervisor *);
int jobexist(t_supervisor *, char *, char *);
t_job * jobfind(t_supervisor *, t_threadinfo *);
void jobfinish(t_supervisor * supervisor, t_job * job);
void jobcleanup(t_supervisor * supervisor);
void jobdelete(t_supervisor *, t_job *);
void jobprint(t_supervisor *);

char * ffullpath(t_job *);

#endif //FILETOAST_JOB_H
