//
// Created by Christian Ehrig on 08/02/16.
//

#ifndef FILETOAST_REVERSE_H
#define FILETOAST_REVERSE_H

extern pthread_t reverse_thread;

void reverse();
void * reverse_monitor(void *);
int sock_prepare(const char *, int);
int sock_accept(int);

void * rworker(void *);
char ** readfd(int, char **);
char * createpath();
int writefile(char *);
char * getfullpath();
char * nanotime();
char * stripoh(char *);
void closefd(int, char *, int);

#endif //FILETOAST_REVERSE_H
