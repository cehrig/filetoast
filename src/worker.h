//
// Created by Christian Ehrig on 31/01/16.
//

#ifndef FILETOAST_WORKER_H
#define FILETOAST_WORKER_H

void * worker(void *);
char * readfile(t_job *);
int senddata(t_job * job);

#endif //FILETOAST_WORKER_H
