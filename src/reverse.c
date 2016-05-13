#include <fcntl.h>
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include <regex.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include "log.h"
#include "main.h"
#include "reverse.h"

pthread_t reverse_thread;
pthread_mutex_t lock;
regex_t * mode = NULL, * length = NULL;

/*
 * Setting up the network socket and compiling regular expression
 */
void reverse()
{
    mode = malloc(sizeof(regex_t));
    if(regcomp(mode, "^POST", 0)) {
        writelog(LOG_ERROR, "Could not compile regular expression.");
    }

    length = malloc(sizeof(regex_t));
    if (regcomp(length, ".*Content-Length: \\([0-9]\\+\\).*", 0)) {
        writelog(LOG_ERROR, "Could not compile regular expression.");
    }

    pthread_mutex_init(&lock, NULL);
    pthread_create(&reverse_thread, NULL, reverse_monitor, (void *) NULL);
}

void * reverse_monitor(void * args)
{
    int sockfd = sock_prepare(configuration.bind, configuration.port);

    pthread_t clthread[configuration.rmaxthreads];

    int pclientfd[configuration.rmaxthreads];
    memset(pclientfd, 0, configuration.rmaxthreads);

    int x = 0, y = 0;
    while(1)
    {
        sock_accept(sockfd, &pclientfd[x]);
        writelog(LOG_ERROR, "ACCEPTED");
        pthread_create(&clthread[x], NULL, rworker, (void *) &pclientfd[x]);

        if(++x == configuration.rmaxthreads) {
            for(y = 0; y < configuration.rmaxthreads; y++) {
                pthread_join(clthread[y], NULL);
                pclientfd[y] = 0;
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

    if(listen(sockfd, 100) < 0) {
        writelog(LOG_CRITICAL, "Can not listen on socket");
        exit(255);
    }

    return sockfd;
}

int sock_accept(int sockfd, int * pclientfd)
{
    int clientsocklen, clientfd;
    struct sockaddr_in clientaddr_in;
    clientsocklen = sizeof(struct sockaddr_in);

    if((clientfd = accept(sockfd, (struct sockaddr *) &clientaddr_in, (socklen_t *) &clientsocklen)) <= 0) {
        writelog(LOG_CRITICAL, "Accept returned negative FD");
        exit(255);
    }

    *pclientfd = clientfd;

    return clientfd;
}

void * rworker(void * args)
{
    int clientfd = *((int *) args);
    char * input = NULL, * cinput = NULL, * winput = NULL;

    /**
     * Check for correct POST request
     */
    if(!readfd(clientfd, &input, 0)
       || regexec(mode, input, 0, NULL, 0)) {

        writelog(LOG_ERROR, "Error reading from socket");
        closefd(clientfd, input, 1);
        return NULL;
    }

    winput = input;

    long contentLength = getContentLength(input);
    char * backlog = strstr(input, "\r\n\r\n")+4;
    int count = 0, z = 0, backlogLength = strlen(backlog);
    size_t pbytes = contentLength-backlogLength;

    while((pbytes && (ioctl(clientfd, FIONREAD, &count)), count == 0) && z++ < 100000) { }

    if(count) {
        readfd(clientfd, &cinput, pbytes);

        char * payload = (char *) malloc((strlen(input)+strlen(cinput)+1) * sizeof(char));
        memset(payload, 0, strlen(input)+strlen(cinput)+1);
        memcpy(payload, input, strlen(input));
        memcpy(payload+strlen(input), cinput, strlen(cinput));

        free(cinput);
        free(input);

        winput = payload;
    }

    /**
     * Strip HTTP and Queue overhead
     */
    if(stripoh(winput)) {
        winput = stripoh(winput);
    }

    if(!winput || !strlen(winput)) {
        closefd(clientfd, winput, 0);
        return NULL;
    }

    /**
     * allocating local storage for decoded content
     */
    char * decoded = (char *) malloc((strlen(winput) + 1) * sizeof(char));

    if(configuration.decode) {
        bzero(decoded, strlen(winput) + 1);
        urldecode(winput, decoded);

        winput = decoded;
    }

    if(writefile(winput)) {
        closefd(clientfd, winput, 0);
        return NULL;
    }

    /**
     * closing the descriptor
     */
    closefd(clientfd, winput, 1);

    return NULL;
}

int writefile(char * input)
{
    char * fullpath = createpath();

    writelog(LOG_DEBUG, "Creating file for incoming data: %s (strlen: %d)", fullpath, strlen(input));

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

    sprintf(out, "%ld%09ld", sec, nsec);

    return out;
}

char ** readfd(int clientfd, char ** input, int len)
{
    size_t bytes = 0, bufSize = 200;
    ssize_t ibytes = 0;
    char * buf = NULL;
    int x = 0;

    do {

        buf = (char *) malloc(bufSize * sizeof(char));
        if(buf == NULL) {
            writelog(LOG_ERROR, "Error allocating memory for socket read buffer (size: %d)", bufSize);
        }
        memset(buf, 0, bufSize);
        ibytes = read(clientfd, buf, bufSize);

        if(ibytes < 1) {
            free(buf);
            break;
        }

        *input = (char *) realloc(*input, (bytes+ibytes) * sizeof(char));
        memcpy((*input)+bytes, buf, ibytes);

        bytes += ibytes;
        free(buf);

        if(len > 0 && bytes >= len) {
            break;
        }

    } while(ibytes > 0 && !strchrl(*input, bytes));

    char * cinput = malloc(bytes*sizeof(char)+1);
    memset(cinput, 0, bytes*sizeof(char)+1);
    memcpy(cinput, *input, bytes);

    free(*input);
    *input = cinput;

    return input;
}

char * stripoh(char * input)
{
    char * begin = NULL;

    if((begin = strstr(input, "P_DATA=")) != NULL) {
        return begin+7;
    }

    return NULL;
}

int ishex(int x)
{
    return	(x >= '0' && x <= '9')	||
              (x >= 'a' && x <= 'f')	||
              (x >= 'A' && x <= 'F');
}

int urldecode(const char *s, char *dec)
{
    char *o;
    const char *end = s + strlen(s);
    int c;

    for (o = dec; s <= end; o++) {
        c = *s++;
        if (c == '+') c = ' ';
        else if (c == '%' && (	!ishex(*s++)	||
                                  !ishex(*s++)	||
                                  !sscanf(s - 2, "%2x", &c)))
            return -1;

        if (dec) *o = c;
    }

    return o - dec;
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

int strchrl(char * string, size_t length)
{
    int httpend = 0;

    int x;
    for(x = 0; x < length; x++) {
        if(*(string+x) == 13 || *(string+x) == 10) {
            if(++httpend == 4) {
                return 1;
            }
        } else {
            httpend = 0;
        }
    }

    return 0;
}

long getContentLength(char * buf)
{
    int reti;
    long contentLength = 0;
    size_t nmatch = 2;
    regmatch_t pmatch[2];
    char * endptr;

    reti = regexec(length, buf, nmatch, pmatch, 0);
    if(reti == REG_NOMATCH) {
        return 0;
    }

    contentLength = strtol(buf+(pmatch[1].rm_so), &endptr, 10);

    return contentLength;
}