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
            info->threadinfo->status = status = THREAD_BUSY;
            writelog(LOG_DEBUG, "Start processing %s (Thread: %d)", job->file, info->threadinfo->id);

            job->status = JOB_PROCESSING;
            job->content = readfile(job);

            /**
             * Send data to Post endpoint
             */
            if(job->content != NULL) {
                if(senddata(job)) {
                    writelog(LOG_ERROR, "Resetting Job-Status for %s (Thread: %d)", job->file, info->threadinfo->id);
                    job->status = JOB_READY;
                    continue;
                } else {
                    writelog(LOG_DEFAULT, "Finished Processing %s (Thread: %d)", job->file, info->threadinfo->id);
                }
            }

            jobfinish(info->supervisor, job);

            sleep(1);
        };

        status = THREAD_IDLE;

        if(info->threadinfo->status != status) {
            info->threadinfo->status = status;
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
        writelog(LOG_ERROR, "Fopen failed for %s -> %s, deleting job. (Thread: %d)", job->fullpath, strerror(errno), job->thread);
        return NULL;
    }

    fserr = fseek(file, 0, SEEK_END);

    if(fserr) {
        writelog(LOG_ERROR, "Fseek failed for %s -> %s (Thread: %d)", job->fullpath, strerror(errno), job->thread);
    }

    offset = ftell(file);
    rewind(file);

    char * filebuf = malloc((offset+1) * sizeof(char));
    bzero(filebuf, offset+1);

    fread (filebuf, 1, offset, file);
    fclose(file);

    return filebuf;
}

int senddata(t_job * job)
{
    int status = 0;
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
        //curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);
        //curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
        res = curl_easy_perform(curl);

        if(res != CURLE_OK) {
            status = 1;
            writelog(LOG_ERROR, "Curl Failed -> %s (Thread: %d)", curl_easy_strerror(res), job->thread);
        }

        curl_easy_cleanup(curl);
        free(post);
        free(encoded);
    }

    fclose(null);

    return status;
}