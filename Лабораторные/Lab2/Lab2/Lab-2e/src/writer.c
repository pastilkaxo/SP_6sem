#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <unistd.h>

#define LOW_OFFSET 0
#define VIEW_SIZE (64*1024)
#define AMOUNT_SIZE (640 *1024)
#define N 10
#define SEMF_NAME "sem2e"

void handleError(const char* message) {
    perror(message);
    exit(EXIT_FAILURE);
}

int main() {
    int* ptr;

    // Создание семафора в закрытом состоянии
    sem_t* sem = sem_open(SEMF_NAME, O_CREAT, 0666, 0);
    if (sem == SEM_FAILED)
        handleError("Error: can't create semaphore");

    // Создание разделяемой памяти
    int fd = shm_open("lab2e", O_CREAT | O_RDWR, 0666);
    if (fd == -1)
        handleError("Error: can't create shared memory");

    // Устанавливаем размер разделяемой памяти
    if (ftruncate(fd, AMOUNT_SIZE) == -1)
        handleError("Error: can't set shared memory size");

    printf("Shared memory created. Press Enter to start writing.\n");
    getchar();
    ptr = (int*)mmap(NULL, VIEW_SIZE, PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED)
        handleError("Error: can not be mapped");
    for (int i = 0; i < N; i++) {
        // Захватываем семафор перед записью

        for (int j = 0; j < (VIEW_SIZE / sizeof(int)); j++) {
            ptr[j] = j + i;
            printf("%d. ptr[%d] = %d\n", i, j, ptr[j]);
        }

        // Разблокируем семафор после записи
        sem_post(sem);
        printf("Data written (Iteration %d). Press Enter to continue.\n", i);
        getchar();
    }
    munmap(ptr, VIEW_SIZE);
    close(fd);
    sem_close(sem);
    sem_unlink(SEMF_NAME);

    printf("Writing finished.\n");
    return 0;
}
