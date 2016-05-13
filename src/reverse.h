//
// Created by Christian Ehrig on 08/02/16.
//

#ifndef FILETOAST_REVERSE_H
#define FILETOAST_REVERSE_H

extern pthread_t reverse_thread;

void reverse();
void * reverse_monitor(void *);
int sock_prepare(const char *, int);
int sock_accept(int, int *);

void * rworker(void *);
char ** readfd(int, char **, int);
char * createpath();
int writefile(char *);
char * getfullpath();
char * nanotime();
int ishex(int);
int urldecode(const char *, char *);
char * stripoh(char *);
int strchrl(char *, size_t);
long getContentLength(char *);
void closefd(int, char *, int);

#endif //FILETOAST_REVERSE_H
