#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>

#define PATH TEXT("task1.txt")
#define LOW_OFFSET 65536
#define VIEW_SIZE 24
#define FILE_SIZE 120000  
#define FILL_TEXT "Lorem Ipsum is simply dummy text of the printing and typesetting industry.\n"


void handleError(const char* message) {
    fprintf(stderr, "%s (Error Code: %lu)\n", message, GetLastError());
    exit(EXIT_FAILURE);
}

void createAndFillFile(const char* filePath) {
    FILE* file = fopen(filePath, "w");
    if (!file) {
        handleError("Error creating file");
    }
    for (int i = 0; i < FILE_SIZE / strlen(FILL_TEXT); i++) {
        fputs(FILL_TEXT, file);
    }

    fclose(file);
}

int main() {

    createAndFillFile("task1.txt");

    HANDLE file = CreateFile(
        PATH,
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,c
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (file == INVALID_HANDLE_VALUE) {
        handleError("Error open file");
    }


    DWORD fileSize = GetFileSize(file, NULL);
    if (fileSize == INVALID_FILE_SIZE) {
        CloseHandle(file);
        handleError("Error: cant get file size");
    }

    HANDLE mapping = CreateFileMapping(
        file,
        NULL,
        PAGE_READWRITE,
        0,
        0,
        "lab2A"
    );

    if (!mapping) {
        CloseHandle(file);
        handleError("Error: cant mapping file in memory");
    }

    unsigned char* mapped = (unsigned char*)MapViewOfFile(
        mapping,
        FILE_MAP_READ,
        0,
        LOW_OFFSET,
        VIEW_SIZE
    );


    if (!mapped) {
        CloseHandle(mapping);
        CloseHandle(file);
        handleError("Error: file can not be mapped");
    }

    printf("File data ( off: %d):\n%.*s\n", LOW_OFFSET, VIEW_SIZE, (char*)mapped);
    getchar();


    UnmapViewOfFile(mapped);


    unsigned char* mapped2 = (unsigned char*)MapViewOfFile(
        mapping,
        FILE_MAP_WRITE,
        0,
        LOW_OFFSET,
        VIEW_SIZE
    );

    memset(mapped2, '\0', VIEW_SIZE);
    FlushViewOfFile(mapped2, VIEW_SIZE);

    getchar();

    UnmapViewOfFile(mapped2);
    CloseHandle(mapping);
    CloseHandle(file);

    return 0;
}
