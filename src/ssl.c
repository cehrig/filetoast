#include <openssl/crypto.h>
#include <pthread.h>
#include "ssl.h"

pthread_mutex_t * mutex_buf = NULL;

unsigned long id_function(void)
{
    return ((unsigned long) pthread_self());
}

void locking_function(int mode, int n, const char * file, int line)
{
    if (mode & CRYPTO_LOCK) {
        pthread_mutex_lock(&mutex_buf[n]);
    } else {
        pthread_mutex_unlock(&mutex_buf[n]);
    }
}

void init_ssl_locks()
{
    mutex_buf = (pthread_mutex_t *) OPENSSL_malloc(CRYPTO_num_locks() * sizeof(pthread_mutex_t));

    int i;
    for (i = 0; i < CRYPTO_num_locks(); i++) {
        pthread_mutex_init(&mutex_buf[i], NULL);
    }

    CRYPTO_set_locking_callback(locking_function);
    CRYPTO_set_id_callback(id_function);
}