#include <glob.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include "main.h"
#include "supervisor.h"
#include "job.h"
#include "log.h"
#include "worker.h"

t_supervisor * tsetup(short lend, short lenp, char ** monitors, char ** patterns)
{
    /**
     * Setting Up worker threads
     */
    int i;
    t_supervisor * supervisor = (t_supervisor *) malloc(sizeof(t_supervisor));
    supervisor->threads = malloc(configuration.maxthreads * sizeof(t_threadinfo *));
    supervisor->dirc = lend;
    supervisor->patc = lenp;
    supervisor->directories = monitors;
    supervisor->patterns = patterns;
    supervisor->jobs = malloc(sizeof(t_jobs));
    supervisor->jobs->num = 0;
    supervisor->jobs->job = NULL;
    pthread_mutex_init(&supervisor->lock, NULL);

    for(i = 0; i < configuration.maxthreads; i++) {
        t_threadinfo * thread = malloc(sizeof(t_threadinfo));
        thread->id = i;
        thread->status = THREAD_STARTUP;

        supervisor->threads[i] = thread;

        t_threadarg * threadarg = (t_threadarg *) malloc(sizeof(t_threadarg));
        threadarg->supervisor = supervisor;
        threadarg->threadinfo = thread;

        pthread_create(&thread->thread, NULL, worker, (void *) threadarg);
    }

    return supervisor;
}

int retthread()
{
    static int x = 0;
    x %= configuration.maxthreads;

    return x++;
}

void globfs(t_supervisor * supervisor)
{
    glob_t globp;
    int x, y, z, e;

    for(x = 0; x < supervisor->dirc; x++) {
        char * dir = *(supervisor->directories+x);
        chdir(dir);

        for(y = 0; y < supervisor->patc; y++) {
            e = glob(*(supervisor->patterns+y), 0, NULL, &globp);

            if(e) {
                if(e != GLOB_NOMATCH) {
                    writelog(LOG_CRITICAL, "Can not glob in %s with error code %d -> %s", dir, e, strerror(errno));
                    exit(255);
                }
            }

            for(z = 0; z < globp.gl_pathc; z++) {
                if(!jobexist(supervisor, dir, globp.gl_pathv[z])) {
                    writelog(LOG_DEBUG, "New Job for file %s", globp.gl_pathv[z]);
                    jobadd(supervisor, jobcreate(dir, globp.gl_pathv[z]));
                }
            }
            globfree(&globp);
        }
    }
}

void tjoin(t_supervisor * supervisor)
{
    int i;
    for(i = 0; i < configuration.maxthreads; i++) {
        pthread_join(supervisor->threads[i]->thread, NULL);
    }
}