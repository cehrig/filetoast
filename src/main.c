/**
 * MT - File To Queue Adapter
 */
#include <libconfig.h>
#include <signal.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include "reverse.h"
#include "supervisor.h"
#include "job.h"
#include "log.h"
#include "main.h"
#include "ssl.h"

t_config configuration;

static void signal_handle(int x) {
    writelog(LOG_CRITICAL, "Catched Signal %d", x, strerror(errno));
    exit(255);
}

int main(int argc, char ** argv)
{
    /**
     * If running with init Script pipe stdout to logfile
     */
    int daemon = 0;

    if(argc == 2 && !strcmp(argv[1], "daemon")) {
        daemon = 1;
    }
    openlog(daemon);

    signal(SIGTERM | SIGPIPE | SIGSEGV | SIGKILL | SIGQUIT | SIGINT | SIGHUP, signal_handle);
    init_ssl_locks();

    /**
     * Reading configuration file
     */
    prepconfig();

    if(configuration.enabled_filetoast) {

        /**
         * Allocating memory for monitored dirs
         */
        char * dirs = malloc((strlen(configuration.directories)+1) * sizeof(char));
        bzero(dirs, strlen(configuration.directories)+1);
        memcpy(dirs, configuration.directories, strlen(configuration.directories));

        /**
         * Allocating memory for monitored patterns
         */
        char * patts = malloc((strlen(configuration.patterns)+1) * sizeof(char));
        bzero(patts, strlen(configuration.patterns)+1);
        memcpy(patts, configuration.patterns, strlen(configuration.patterns));

        /**
         * Defining delimiter for monitored directories
         */
        char delimiter[] = ";";

        /**
         * Creating Pointer Vector for all Directories
         */
        char ** monitors = NULL;

        /**
         * Splitting Monitored directories
         */
        char * tok;
        tok = strtok(dirs, delimiter);

        short x = 0; int s = 0;
        while(tok != NULL) {
            char * cur = tok;
            if((s = mkdir(cur, 0755))) {
                if(errno == EEXIST) {
                    writelog(LOG_DEFAULT, "Directory found %s", tok);
                    monitors = realloc(monitors, ++x * sizeof(char *));
                    *(monitors+(x-1)) = tok;
                } else {
                    writelog(LOG_ERROR, "Directory %s could not be opened -> %s", tok, strerror(errno));
                }

            }

            tok = strtok(NULL, delimiter);
        }

        if(!x) {
            printf("No directory found");
            return 1;
        }

        /**
         * Creating Pointer Vector for all file patterns
         */
        char ** patterns = NULL;

        /**
         * Splitting monitored file patterns
         */
        char * tokp;
        tokp = strtok(patts, delimiter);

        short y = 0;
        while(tokp != NULL) {
            patterns = realloc(patterns, ++y * sizeof(char *));
            *(patterns+(y-1)) = tokp;

            tokp = strtok(NULL, delimiter);
        }

        /**
        * Setting up filetoast threads
        */
        t_supervisor * supervisor = tsetup(x, y, monitors, patterns);

        /**
         * Setting up filetoast monitor
         */
        filetoast(supervisor);
    }

    if(configuration.enabled_astofile) {
        /**
         * Setting up reverse data-flow (POST to File)
         */
        reverse();
    }

    /**
     * Joining monitoring threads to prevent stopping program execution
     */
    if(configuration.enabled_astofile) {
        pthread_join(reverse_thread, NULL);
    }

    if(configuration.enabled_filetoast) {
        pthread_join(filetoast_thread, NULL);
    }

    writelog(LOG_ERROR, "Filetoast quits");

    return 0;
}

void prepconfig()
{
    configuration.enabled_filetoast = 0;
    configuration.enabled_astofile = 0;

    /**
     * Read configuration file
     */
    config_t cfg, * cf;

    cf = &cfg;
    config_init(cf);

    if (!config_read_file(cf, "/etc/filetoast.conf")) {
        writelog(LOG_CRITICAL, "Can not locate configuration in /etc/filetoast.conf");
        exit(255);
    }

    if (config_lookup(cf, "filetoast")) {
        if (!config_lookup_string(cf, "filetoast.posturl", &configuration.posturl)) {
            writelog(LOG_CRITICAL, "Could not find 'posturl' in configuration file.");
            exit(255);
        }

        if (!config_lookup_string(cf, "filetoast.poststring", &configuration.poststring)) {
            writelog(LOG_CRITICAL, "Could not find 'poststring' in configuration file.");
            exit(255);
        }

        if (!config_lookup_string(cf, "filetoast.directories", &configuration.directories)) {
            writelog(LOG_CRITICAL, "Could not find 'directories' in configuration file.");
            exit(255);
        }

        if (!config_lookup_string(cf, "filetoast.patterns", &configuration.patterns)) {
            writelog(LOG_CRITICAL, "Could not find 'patterns' in configuration file.");
            exit(255);
        }

        if (!config_lookup_int(cf, "filetoast.maxthreads", &configuration.maxthreads)) {
            writelog(LOG_CRITICAL, "Could not find 'maxthreads' in configuration file.");
            exit(255);
        }

        configuration.enabled_filetoast = 1;
    } else {
        writelog(LOG_DEBUG, "Skipping Filetoast due to missing configuration");
    }

    if (config_lookup(cf, "astofile")) {
        if (!config_lookup_string(cf, "astofile.bind", &configuration.bind)) {
            writelog(LOG_CRITICAL, "Could not find 'bind' in configuration file.");
            exit(255);
        }

        if (!config_lookup_int(cf, "astofile.port", &configuration.port)) {
            writelog(LOG_CRITICAL, "Could not find 'bind' in configuration file.");
            exit(255);
        }

        if (!config_lookup_int(cf, "astofile.maxthreads", &configuration.rmaxthreads)) {
            writelog(LOG_CRITICAL, "Could not find 'maxthreads' in (reverse) configuration file.");
            exit(255);
        }
        configuration.enabled_astofile = 1;
    } else {
        writelog(LOG_DEBUG, "Skipping Reverse Filetoast due to missing configuration");
    }

}