//
// Created by Christian Ehrig on 08/02/16.
//

#ifndef FILETOAST_REVERSE_H
#define FILETOAST_REVERSE_H

extern pthread_t reverse_thread;

void reverse();
void * reverse_monitor(void *);
int sock_prepare(const char *, int);
int sock_listen(int);

void * rworker(void *);
int readfd(int, char *);
void closefd(int);

#endif //FILETOAST_REVERSE_H
