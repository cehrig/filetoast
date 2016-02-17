#include <fcntl.h>
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include "log.h"
#include "main.h"
#include "reverse.h"

pthread_t reverse_thread;
pthread_mutex_t lock;

void reverse()
{
    pthread_mutex_init(&lock, NULL);
    pthread_create(&reverse_thread, NULL, reverse_monitor, (void *) NULL);
}

void * reverse_monitor(void * args)
{
    int sockfd = sock_prepare(configuration.bind, configuration.port);

    if(listen(sockfd, 100) < 0) {
        writelog(LOG_CRITICAL, "Can not listen on socket");
        exit(255);
    }

    int clientfd;

    pthread_t clthread[configuration.rmaxthreads];

    int pclientfd[configuration.rmaxthreads];
    memset(pclientfd, 0, configuration.rmaxthreads);

    int x = 0, y = 0;
    while(1)
    {
        clientfd = sock_accept(sockfd);

        pclientfd[x] = clientfd;

        pthread_create(&clthread[x], NULL, rworker, (void *) &clientfd);

        if(++x == configuration.rmaxthreads) {
            writelog(LOG_DEBUG, "start joining");
            for(y = 0; y < configuration.rmaxthreads; y++) {
                close(pclientfd[y]);
                pclientfd[y] = 0;
                pthread_join(clthread[y], NULL);
            }
            writelog(LOG_DEBUG, "finished joining");
            x = y = 0;
        }
    }

    return NULL;
}

int sock_prepare(const char * ip, int port)
{
    int sockfd, yes = 1;
    struct sockaddr_in sockaddr_in;

    sockaddr_in.sin_family = AF_INET;
    sockaddr_in.sin_port = htons(port);
    inet_aton(ip, &sockaddr_in.sin_addr);

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        writelog(LOG_CRITICAL, "Can't open socket");
        exit(255);
    }

    if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) < 0) {
        writelog(LOG_CRITICAL, "socket: Can not set socket options");
        exit(255);
    }

    if(bind(sockfd, (struct sockaddr *) &sockaddr_in, (socklen_t) sizeof(sockaddr_in)) < 0) {
        writelog(LOG_CRITICAL, "socket: Error binding address");
        exit(255);
    }

    return sockfd;
}

int sock_accept(int sockfd)
{
    int clientsocklen;
    struct sockaddr_in clientaddr_in;
    clientsocklen = sizeof(struct sockaddr_in);

    pthread_mutex_lock(&lock);
    int clientfd = accept(sockfd, (struct sockaddr *) &clientaddr_in, (socklen_t *) &clientsocklen);
    pthread_mutex_unlock(&lock);

    return clientfd;
}

void * rworker(void * args)
{
    int clientfd = *((int *) args);
    printf("starting \tthread: %ld \tclientfd: %d\n", pthread_self(), clientfd);
    char * input = NULL;
    char * cinput = NULL;

    if(!readfd(clientfd, &input)) {
        writelog(LOG_ERROR, "Error reading from socket");
    }

    cinput = input;

    /**
     * Strip HTTP and Queue overhead
     */
    if(stripoh(cinput)) {
        cinput = stripoh(cinput);
    }

    if(!cinput || !strlen(cinput)) {
        closefd(clientfd, input, 0);
        return NULL;
    }

    if(writefile(cinput)) {
        closefd(clientfd, input, 0);
        return NULL;
    }

    /**
     * closing the descriptor
     */
    closefd(clientfd, input, 1);

    printf("finished \tthread: %ld \tclientfd: %d\n", pthread_self(), clientfd);
    return NULL;
}

int writefile(char * input)
{
    char * fullpath = createpath();

    FILE * fp;
    fp = fopen(fullpath, "w+");

    fputs(input, fp);

    fclose(fp);
    free(fullpath);

    return 0;
}

char * createpath()
{
    char * fullpath = NULL;
    do {
        fullpath = getfullpath();
    } while(!access(fullpath, F_OK));

    writelog(LOG_DEBUG, "Creating file for incoming data: %s", fullpath);

    return fullpath;
}

char * getfullpath()
{
    const char * directory = configuration.directory;
    char filename[512];
    char * ntime = nanotime();

    sprintf(filename, "queue_input_t_%s%s", ntime, configuration.postfix);

    char * fullpath = malloc(strlen(directory) + strlen(filename) + 1);
    bzero(fullpath, strlen(directory) + strlen(filename) + 1);
    memcpy(fullpath, directory, strlen(directory));
    memcpy(fullpath+strlen(directory), filename, strlen(filename));

    free(ntime);

    return fullpath;
}

char * nanotime()
{
    time_t sec;
    long nsec;
    struct timespec tp;
    char * out = (char *) malloc(20 * sizeof(char));

    if(clock_gettime(CLOCK_REALTIME, &tp)) {
        return NULL;
    }

    sec = tp.tv_sec;
    nsec = tp.tv_nsec;

    sprintf(out, "%ld%ld", sec, nsec);

    return out;
}

char ** readfd(int clientfd, char ** input)
{
    int bufferlen = 10, run = 0;
    ssize_t sumb = 0, readb = 0;
    char buffer[bufferlen];

    while((readb = read(clientfd, buffer, bufferlen)) > 0)
    {
        printf("read: %d \tthread: %ld \tclientfd: %d\n", readb, pthread_self(), clientfd);

        sumb += readb;

        *input = (char *) realloc(*input, ++run * bufferlen * sizeof(char));
        memcpy((*input)+((run-1)*bufferlen), buffer, readb);

        if(readb < bufferlen) {
            break;
        }
    };

    *input = (char *) realloc(*input, sumb * sizeof(char) +1);
    memset((*input) + sumb, 0, 1);
    return input;
}

char * stripoh(char * input)
{
    char * begin = NULL;

    if((begin = strstr(input, "&P_DATA=")) != NULL) {
        return begin+8;
    }

    return NULL;
}

void closefd(int clientfd, char * input, int success)
{
    char send[1024];
    sprintf(send, "HTTP/1.1 200 OK\nContent-Type: application/json; charset=utf-8\nAccess-Control-Allow-Origin: *\nAccess-Control-Allow-Methods: POST, GET, OPTIONS\nContent-Length: 9\n\nokokok!!!");

    if(success) {
        write(clientfd, send, strlen(send));
    }

    if(input != NULL) {
        free(input);
    }

    close(clientfd);
}