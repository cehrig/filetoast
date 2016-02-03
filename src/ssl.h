//
// Created by Christian Ehrig on 03/02/16.
//

#ifndef FILETOAST_SSL_H
#define FILETOAST_SSL_H

unsigned long id_function(void);
void locking_function(int, int, const char *, int);
void init_ssl_locks();

#endif //FILETOAST_SSL_H
