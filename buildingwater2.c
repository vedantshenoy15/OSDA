///Users/vedantshenoy/VScode/buildingwater.c
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

sem_t h_semaphore;  // Semaphore for hydrogen atoms
sem_t o_semaphore;  // Semaphore for oxygen atoms
int h_count = 0;    // Counter for available hydrogen atoms
int o_count = 0;    // Counter for available oxygen atoms

void* hydrogen(void* arg) {
    sem_wait(&h_semaphore);  // Wait for a hydrogen atom
    h_count++;
    // Additional steps for preparing a hydrogen atom
    printf("Hydrogen atom is ready.\n");
    // ...

    // Check if both hydrogen and oxygen atoms are available
    if (h_count >= 2 && o_count >= 1) {
        sem_post(&h_semaphore);  // Release two hydrogen atoms
        sem_post(&h_semaphore);
        h_count -= 2;
        sem_post(&o_semaphore);  // Release one oxygen atom
        o_count--;
        printf("Water molecule (H2O) is formed.\n");
    }

    pthread_exit(NULL);
}

void* oxygen(void* arg) {
    sem_wait(&o_semaphore);  // Wait for an oxygen atom
    o_count++;
    // Additional steps for preparing an oxygen atom
    printf("Oxygen atom is ready.\n");
    // ...

    // Check if both hydrogen and oxygen atoms are available
    if (h_count >= 2 && o_count >= 1) {
        sem_post(&h_semaphore);  // Release two hydrogen atoms
        sem_post(&h_semaphore);
        h_count -= 2;
        sem_post(&o_semaphore);  // Release one oxygen atom
        o_count--;
        printf("Water molecule (H2O) is formed.\n");
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t h_threads[10];  // Hydrogen threads
    pthread_t o_threads[5];   // Oxygen threads

    // Initialize semaphores
    sem_init(&h_semaphore, 0, 2);  // Maximum count of 2 for hydrogen atoms
    sem_init(&o_semaphore, 0, 1);  // Maximum count of 1 for oxygen atoms

    // Create and start hydrogen threads
    for (int i = 0; i < 10; i++) {
        pthread_create(&h_threads[i], NULL, hydrogen, NULL);
    }

    // Create and start oxygen threads
    for (int i = 0; i < 5; i++) {
        pthread_create(&o_threads[i], NULL, oxygen, NULL);
    }

    // Wait for all threads to finish
    for (int i = 0; i < 10; i++) {
        pthread_join(h_threads[i], NULL);
    }
    for (int i = 0; i < 5; i++) {
        pthread_join(o_threads[i], NULL);
    }

    // Destroy semaphores
    sem_destroy(&h_semaphore);
    sem_destroy(&o_semaphore);

    return 0;
}
