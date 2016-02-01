#include <curl/curl.h>
#include <stdlib.h>
#include <unistd.h>
#include "main.h"
#include "supervisor.h"
#include "job.h"
#include "log.h"
#include "worker.h"

void * worker(void * arg)
{
    t_threadarg * info = (t_threadarg *) arg;
    int status = info->threadinfo->status;

    writelog(LOG_DEFAULT, "Starting Worker-Thread (Thread: %d)", info->threadinfo->id);

    t_job * job;
    while (1) {

        while((job = jobfind(info->supervisor, info->threadinfo)) != NULL) {
            info->threadinfo->status = THREAD_BUSY;

            job->status = JOB_PROCESSING;
            job->content = readfile(job);

            /**
             * Send data to Post endpoint
             */
            if(job->content != NULL) {
                senddata(job);
            }
            writelog(LOG_DEFAULT, "Finished Processing %s", job->file);

            jobfinish(info->supervisor, job);

            sleep(1);
        };

        status = THREAD_IDLE;

        if((info->threadinfo->status = THREAD_IDLE) != status) {
            status = info->threadinfo->status;
            writelog(LOG_DEFAULT, "Waiting for Jobs (Thread: %d)", info->threadinfo->id);
        }

        sleep(1);
    }

    return 0;
}

char * readfile(t_job * job)
{
    long offset;
    int fserr;

    FILE * file = fopen(job->fullpath, "r");

    if(file == NULL) {
        writelog(LOG_ERROR, "fopen failed for %s -> %s in thread %d, deleting job.", job->fullpath, strerror(errno), job->thread);
        return NULL;
    }

    fserr = fseek(file, 0, SEEK_END);

    if(fserr) {
        writelog(LOG_ERROR, "fseek failed for %s -> %s", job->fullpath, strerror(errno));
    }

    offset = ftell(file);
    rewind(file);

    char * filebuf = malloc((offset+1) * sizeof(char));
    bzero(filebuf, offset+1);

    fread (filebuf, 1, offset, file);
    fclose(file);

    return filebuf;
}

void senddata(t_job * job)
{
    CURL * curl;
    CURLcode res;

    FILE * null = fopen("/dev/null", "w");

    curl = curl_easy_init();
    if(curl) {
        char * encoded = curl_easy_escape(curl, job->content, strlen(job->content));

        char * post = (char *) malloc(strlen(encoded) + strlen(configuration.poststring));
        sprintf(post, configuration.poststring, encoded);

        curl_easy_setopt(curl, CURLOPT_URL, configuration.posturl);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, null);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post);

        res = curl_easy_perform(curl);

        if(res != CURLE_OK) {
            writelog(LOG_ERROR, "Curl Failed -> %s", curl_easy_strerror(res));
        }

        curl_easy_cleanup(curl);
        free(post);
        free(encoded);
    }

    fclose(null);
}