#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
// 1) 875 threads. Firefox ou VSCode.
// 2) NUMERO TOTAL DE THREADS -> 33625 = 875 do sistema + 32750 do programa

#define NUM_THREADS 32750

void *thread_func(void *arg) {
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    int rc;

    for (int i = 0; i < NUM_THREADS; i++) {
        rc = pthread_create(&threads[i], NULL, thread_func, NULL);
        if (rc) {
            fprintf(stderr, "Error creating thread: %d\n", rc);
            exit(EXIT_FAILURE);
        }
    }
    pthread_exit(NULL);
}
