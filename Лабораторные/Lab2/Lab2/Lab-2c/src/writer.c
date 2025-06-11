#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>

#define LOW_OFFSET 0
#define VIEW_SIZE (64 * 1024)
#define AMOUNT_SIZE (640 * 1024)
#define N 10
#define MUTEX_NAME "2c"


void handleError(const char* message) {
    fprintf(stderr, "%s (Error Code: %lu)\n", message, GetLastError());
    exit(EXIT_FAILURE);
}



int main() {

    int* ptr;
    HANDLE mutex = CreateMutex(NULL, FALSE, MUTEX_NAME);

    if (!mutex) {
        CloseHandle(mutex);
        handleError("Error: cant create mutex");
    }

    printf("CreateMutex;\nEnter to cont.");
    getchar();

    HANDLE mapping = CreateFileMapping(
        INVALID_HANDLE_VALUE,
        NULL,
        PAGE_READWRITE,
        0,AMOUNT_SIZE,
        "Lab-02"
    );

    printf("CreateFileMapping;\nEnter to cont.");
    getchar();

    if (!mapping) {
        CloseHandle(mapping);
        handleError("Error: cant mapping in memory");
    }

    ptr = (int*)MapViewOfFile(
        mapping,
        FILE_MAP_WRITE,
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


        for (int j = 0; j < (VIEW_SIZE / sizeof(int)); j++) {
            d[j] = j + i ;
            printf("%d. ptr[j] = %d\n",i,ptr[j]);
        }

        printf("Blocked;\nEnter to cont.");


        ReleaseMutex(mutex);
        Sleep(500);
    }
    UnmapViewOfFile(ptr);

    printf("MapViewOfFile;\nEnter to cont.");
    getchar();


    CloseHandle(mapping);

    printf("Enter to exit;");
    getchar();

    return 0;
}
