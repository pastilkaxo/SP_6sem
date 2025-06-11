#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>

#define LOW_OFFSET 0
#define VIEW_SIZE (64 * 1024)
#define AMOUNT_SIZE (640 * 1024)
#define N 10
#define MUTEX_NAME "2c"
#define MAP_NAME "Lab-02"

void handleError(const char* message) {
    fprintf(stderr, "%s (Error Code: %lu)\n", message, GetLastError());
    exit(EXIT_FAILURE);
}



int main() {

    int* ptr;
    HANDLE mutex = OpenMutex(SYNCHRONIZE, FALSE, MUTEX_NAME);

    if (!mutex) {
        CloseHandle(mutex);
        handleError("Error: cant open mutex");
    }

    printf("CreateMutex;\nEnter to cont.");
    getchar();

    HANDLE mapping = OpenFileMapping(FILE_MAP_READ, FALSE, MAP_NAME);


    printf("OpenFileMapping;\nEnter to cont.");
    getchar();

    if (!mapping) {
        CloseHandle(mapping);
        handleError("Error: cant open mapping");
    }

    ptr = (int*)MapViewOfFile(
        mapping,
        FILE_MAP_READ,
        0,
        LOW_OFFSET,
        VIEW_SIZE
    );

    if (!ptr) {
        handleError("Error: can not be mapped");
    }

    for (int i = 0; i < N; i++) {
        WaitForSingleObject(mutex, INFINITE);
        int* d = ptr;
        printf("Readed:\nEnter to cont.");
        for (int j = 0; j < (VIEW_SIZE / sizeof(int)); j++) {
            printf("%d. ptr[j] = %d\n", i, d[j]);
        }


        ReleaseMutex(mutex);
        Sleep(500);
    }


    printf("MapViewOfFile Ended;\nEnter to cont.");
    getchar();

    UnmapViewOfFile(ptr);
    CloseHandle(mapping);

    printf("Enter to exit;");
    getchar();

    return 0;
}
