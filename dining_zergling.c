#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/syscall.h>

// capacity of the pot
const int M = 4;

sem_t semaphore_overlord;
sem_t terran_semaphore;
sem_t hungry_zergling_semaphore;

// Global variable controlling that the cook should keep cooking
static volatile bool keep_cooking = true;

// TODO: Model the pot and synchronise the behaviour somehow!
// Hint: Take a look on the global variables of the last exercise sheet's task (but not for the whole synchronisation please)


/**
 * @brief Helper that returns the thread id of the calling thread
 *
 * @return long the thread id as a long
 */
long tid_help() {
    return syscall(SYS_gettid);
}

/**
 * @brief Message that the baby zergling has eaten a portion. Do not change.
 *
 */
void has_eaten(int i) {
    printf("Baby zergling (tid: %li) has eaten its %i-th portion.\n", tid_help(), i + 1);
}

/**
 * @brief Message that a baby zergling tries to get a portion. Do not change.
 *
 */
void try_to_get_portion() {
    printf("Baby zergling (tid: %li) tries to get a portion.\n", tid_help());
    int wait_error;
    do {
        sem_wait(&semaphore_overlord);
	wait_error = sem_trywait(&terran_semaphore);
	sem_post(&semaphore_overlord);
    } while(wait_error);
}


/**
 * @brief The start routine of a baby zergling
 *
 * @param in
 * @return void*
 */
void* baby_zergling(void* in) {
    // TODO: Synchronisation and pot access 
    for (int eaten = 0; eaten < 13; eaten++) {
        try_to_get_portion();
        has_eaten(eaten);
    }
    sem_wait(&hungry_zergling_semaphore);
    return NULL;
}


/**
 * @brief Message that the cook zergling fills the pot. Do not change.
 *
 */
void put_serving_in_the_pot() {
    int remaining_hungry_zerglings = -1;
    sem_getvalue(&hungry_zergling_semaphore, &remaining_hungry_zerglings);
    if (remaining_hungry_zerglings == 0) {
        keep_cooking = false;
    }
    sem_wait(&semaphore_overlord);
    int sem_val;
    sem_getvalue(&terran_semaphore, &sem_val);
    if (sem_val == 0) {
        for(int i = 0; i < M; ++i) {
            sem_post(&terran_semaphore);
	}
    }
    sem_post(&semaphore_overlord);
    printf("Cook fills up the pot\n");
}


/**
 * @brief Start routine for the cook zergling.
 *
 * @param in
 * @return void*
 */
void* cook_zergling(void* in) {
    // TODO: Synchronisation and pot access
    while (keep_cooking) {
        put_serving_in_the_pot();
    }
    printf("Cook finishes cooking and an ultralisk eats the whole remaining pot!\n");
    return NULL;
}

/**
 * @brief Initialises the semaphores
 *
 */
void init_sems() {
    sem_init(&semaphore_overlord, 0, 1);
    sem_init(&terran_semaphore, 0, M);
    sem_init(&hungry_zergling_semaphore, 0, 10);
}

int main(void) {
    init_sems();
    pthread_attr_t attributes;
    pthread_attr_init(&attributes);
    pthread_t zerglings[10];
    for(int i = 0; i < 10; ++i) {
        pthread_create(zerglings + i, &attributes, baby_zergling, NULL);
    }
    pthread_t cook;
    pthread_create(&cook, &attributes, cook_zergling, NULL);
    pthread_attr_destroy(&attributes);
    pthread_join(cook, NULL);
    for(int i = 0; i < 10; ++i) {
        pthread_join(zerglings[i], NULL);
    }
}
// TODO: Main Function that spawns 10 zergling processes, one cook process and joins all of them in the end. Use keep_cooking for controlling the cook
