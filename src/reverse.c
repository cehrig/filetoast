#include <fcntl.h>
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include "log.h"
#include "main.h"
#include "reverse.h"

pthread_t reverse_thread;

void reverse()
{
    pthread_create(&reverse_thread, NULL, reverse_monitor, (void *) NULL);
}

void * reverse_monitor(void * args)
{
    int sockfd = sock_prepare(configuration.bind, configuration.port);
    int clientfd;

    pthread_t clthread[configuration.rmaxthreads];

    int pclientfd[configuration.rmaxthreads];
    memset(pclientfd, 0, configuration.rmaxthreads);

    int x = 0, y = 0;
    while(1)
    {
        clientfd = sock_listen(sockfd);
        pclientfd[x] = clientfd;

        pthread_create(&clthread[x], NULL, rworker, (void *) &clientfd);

        if(++x == configuration.rmaxthreads) {
            for(y = 0; y < configuration.rmaxthreads; y++) {
                pclientfd[y] = 0;
                pthread_join(clthread[y], NULL);
            }

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

int sock_listen(int sockfd)
{
    int clientsocklen;
    struct sockaddr_in clientaddr_in;
    clientsocklen = sizeof(struct sockaddr_in);

    if(listen(sockfd, 100) < 0) {
        writelog(LOG_CRITICAL, "Can not listen on socket");
        exit(255);
    }

    int clientfd = accept(sockfd, (struct sockaddr *) &clientaddr_in, (socklen_t *) &clientsocklen);

    return clientfd;
}

void * rworker(void * args)
{
    int clientfd = *((int *) args);

    char * input;
    readfd(clientfd, input);

    /**
     * closing the descriptor
     */
    closefd(clientfd);

    return NULL;
}

int readfd(int clientfd, char * input)
{
    int bufferlen = 512;
    int sumb = 0, readb = 0, run = 0;
    char buffer[bufferlen];

    do {
        readb = read(clientfd, buffer, bufferlen);
        writelog(LOG_DEBUG, "FD: %d - %d", clientfd, readb);
        sumb += readb;


    } while(readb > 0);
    writelog(LOG_DEBUG, "FINISHED READ ON FD %d", clientfd);
    return 0;
}

void closefd(int clientfd)
{
    close(clientfd);
}