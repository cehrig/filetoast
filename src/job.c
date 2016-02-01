#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include "supervisor.h"
#include "job.h"
#include "log.h"

t_job * jobcreate(char * path, char * file)
{
    char * filename = (char *) malloc((strlen(file)+1) * sizeof(char));
    bzero(filename, strlen(file)+1);
    memcpy(filename, file, strlen(file));

    t_job * new = malloc(sizeof(t_job));
    new->thread = THREAD_UNASSIGNED;
    new->path = path;
    new->file = filename;
    new->fullpath = ffullpath(new);
    new->status = JOB_READY;
    new->next = NULL;

    return new;
}

void jobadd(t_supervisor * supervisor, t_job * new)
{
    pthread_mutex_lock(&supervisor->lock);

    t_jobs * jobs = supervisor->jobs;

    if(!jobs->job) {
        supervisor->jobs->job = new;
    } else {
        t_job * job = supervisor->jobs->job;
        while(job->next != NULL) {
            job = job->next;
        }

        job->next = new;
    }

    supervisor->jobs->num++;

    pthread_mutex_unlock(&supervisor->lock);
}

int jobexist(t_supervisor * supervisor, char * path, char * file)
{
    pthread_mutex_lock(&supervisor->lock);

    t_job * job = supervisor->jobs->job;

    while(job != NULL) {

        if(!strcmp(job->file, file) && !strcmp(job->path, path)) {
            pthread_mutex_unlock(&supervisor->lock);
            return 1;
        }

        job = job->next;
    }

    pthread_mutex_unlock(&supervisor->lock);

    return 0;
}

void jobassign(t_supervisor * supervisor)
{
    pthread_mutex_lock(&supervisor->lock);
    t_job * job = supervisor->jobs->job;

    while(job) {
        if(job->thread == THREAD_UNASSIGNED) {
            job->thread = retthread();
        }
        job = job->next;
    }
    pthread_mutex_unlock(&supervisor->lock);
}

t_job * jobfind(t_supervisor * supervisor, t_threadinfo * info)
{
    pthread_mutex_lock(&supervisor->lock);
    t_job * job = supervisor->jobs->job;

    while(job != NULL) {

        if(job->thread == info->id
           && job->status == JOB_READY) {

            pthread_mutex_unlock(&supervisor->lock);
            return job;

        }

        job = job->next;
    }

    pthread_mutex_unlock(&supervisor->lock);
    return NULL;
}

void jobfinish(t_supervisor * supervisor, t_job * job)
{
    pthread_mutex_lock(&supervisor->lock);

    job->status = JOB_FINISHED;

    pthread_mutex_unlock(&supervisor->lock);
}

void jobcleanup(t_supervisor * supervisor)
{
    pthread_mutex_lock(&supervisor->lock);
    t_job * job = supervisor->jobs->job;

    while(job != NULL) {

        t_job * tmp = job;
        job = job->next;

        if(tmp->status == JOB_FINISHED) {
            jobdelete(supervisor, tmp);
            free(tmp);
        }
    }

    pthread_mutex_unlock(&supervisor->lock);
}

void jobdelete(t_supervisor * supervisor, t_job * job)
{
    t_job * curr = supervisor->jobs->job;
    t_job * prev = curr;

    while(curr != job) {
        prev = curr;
        curr = curr->next;
    }

    if(curr == prev) {
        supervisor->jobs->job = curr->next;
    } else {
        prev->next = curr->next;
    }

    if(unlink(job->fullpath)) {
        writelog(LOG_CRITICAL, "Can not unlink file %s -> %s (Thread: %d)", job->fullpath, strerror(errno), job->thread);
    }
    writelog(LOG_DEFAULT, "Unlinked file %s -> %s (Thread: %d)", job->fullpath, strerror(errno), job->thread);

    free(curr->file);
    free(curr->fullpath);

    if(curr->content != NULL) {
        free(curr->content);
    }
}

void jobprint(t_supervisor * supervisor)
{
    t_job * job = supervisor->jobs->job;

    while(job != NULL) {
        writelog(LOG_DEFAULT, "-> %s", job->file);
        job = job->next;
    }
}

char * ffullpath(t_job * job)
{
    char lastchar = *(job->path+(strlen(job->path)-1));
    char * fullpath;

    if(lastchar != '/') {
        fullpath = malloc(strlen(job->path) + strlen(job->file) + 2);
        bzero(fullpath, strlen(job->path) + strlen(job->file) + 2);
        memcpy(fullpath, job->path, strlen(job->path));
        memcpy(fullpath+strlen(job->path), "/", 1);
        memcpy(fullpath+strlen(job->path)+1, job->file, strlen(job->file));
    } else {
        fullpath = malloc(strlen(job->path) + strlen(job->file) + 1);
        bzero(fullpath, strlen(job->path) + strlen(job->file) + 1);
        memcpy(fullpath, job->path, strlen(job->path));
        memcpy(fullpath+strlen(job->path), job->file, strlen(job->file));
    }

    return fullpath;
}