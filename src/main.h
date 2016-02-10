//
// Created by Christian Ehrig on 25/01/16.
//

#ifndef XML2QUEUE_MAIN_H
#define XML2QUEUE_MAIN_H

typedef struct config
{
    int enabled_filetoast;
    const char * posturl;
    const char * poststring;
    const char * directories;
    const char * patterns;
    int maxthreads;
    int enabled_astofile;
    const char * bind;
    int port;
    int rmaxthreads;

} t_config;

void prepconfig();

extern t_config configuration;

#endif //XML2QUEUE_MAIN_H
