#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>

#define PATH TEXT("task2.txt")
#define LOW_OFFSET 0
#define VIEW_SIZE (128 * 1024 * 1024)
#define FILE_SIZE (512 * 1024 * 1024)
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

    createAndFillFile("task2.txt");

    HANDLE file = CreateFile(
        PATH,
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (file == INVALID_HANDLE_VALUE) {
        handleError("Error open file");
    }

    printf("CreateFile;\n Enter to cont.");
    getchar();

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
        "lab2B"
    );

    printf("CreateFileMapping;\nEnter to cont.");
    getchar();

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


    printf("MapViewOfFile;\nEnter to cont.");
    getchar();

    if (!mapped) {
        CloseHandle(mapping);
        CloseHandle(file);
        handleError("Error: file can not be mapped");
    }


    printf("Output;\nEnter to start.");
    getchar();

    printf("File data ( off: %d):\n%.*s\n", LOW_OFFSET, VIEW_SIZE, (char*)mapped);

    UnmapViewOfFile(mapped);

    CloseHandle(mapping);
    CloseHandle(file);

    printf("Enter to exit;");
    getchar();

    return 0;
}
