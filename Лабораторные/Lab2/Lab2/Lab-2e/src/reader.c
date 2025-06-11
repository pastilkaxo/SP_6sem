#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <unistd.h>

#define LOW_OFFSET 0
#define VIEW_SIZE (64*1024)
#define N 10
#define SEMF_NAME "sem2e"

void handleError(const char* message) {
    perror(message);
    exit(EXIT_FAILURE);
}

int main() {
    int* ptr;

    // Открытие семафора
    sem_t* sem = sem_open(SEMF_NAME, 0);
    if (sem == SEM_FAILED)
        handleError("Error: can't open semaphore");

    // Открытие разделяемой памяти
    int fd = shm_open("lab2e", O_RDONLY, 0666);
    if (fd == -1)
        handleError("Error: can't open shared memory");

    printf("Shared memory opened. Press Enter to start reading.\n");
    getchar();
    ptr = (int*)mmap(NULL, VIEW_SIZE, PROT_READ, MAP_SHARED, fd, LOW_OFFSET);
    if (ptr == MAP_FAILED) {
        handleError("Error: can not be mapped");
    }
    for (int i = 0; i < N; i++) {
        sem_wait(sem);



        printf("Reading data (Iteration %d):\n", i);
        for (int j = 0; j < (VIEW_SIZE / sizeof(int)); j++) {
            printf("%d. ptr[%d] = %d\n", i, j, ptr[j]);
        }

        
        printf("Data read complete (Iteration %d). Press Enter to continue.\n", i);
    }
    munmap(ptr, VIEW_SIZE);
    close(fd);
    sem_close(sem);

    printf("Reading finished.\n");
    return 0;
}
