#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

typedef int (*bsearch_func)(int*, int, int);
typedef int (*bsearch_r_func)(int*, int, int, int);
typedef int* LArr;
#define SIZE 1024

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Usage: Lab-03b-ex <Library Name> <Function Name or Number> [Target Number]\n");
        return 1;
    }

    const char* library_name = argv[1];
    const char* function_name = argv[2];
    int target_value = -1;

 
    HMODULE hLib = LoadLibrary(library_name);
    if (!hLib) {
        printf("Failed to load library: %s\n", library_name);
        return 1;
    }

    if (argc == 4) {
        target_value = atoi(argv[3]);
    }
    else {
        printf("Enter the number to search: ");
        scanf_s("%d", &target_value);
    }

  
    LArr a = (LArr)GetProcAddress(hLib, "arr");
    if (!a) {
        printf("Array 'arr' not found in library %s!\n", library_name);
        FreeLibrary(hLib);
        return 1;
    }

    void* func_addr = NULL;
    
    if (strcmp(function_name, "iterative") == 0 || strcmp(function_name, "1") == 0) {
        func_addr = GetProcAddress(hLib,MAKEINTRESOURCE(1));
        if (!func_addr) {
            func_addr = (bsearch_r_func)GetProcAddress(hLib, "?bsearch_i@@YAHPEAHHHH@Z");
            if(!func_addr)  func_addr = GetProcAddress(hLib, "bsearch_i");
        }
    }
   
    else if (strcmp(function_name, "recursive") == 0 || strcmp(function_name, "2") == 0) {
        func_addr = GetProcAddress(hLib, MAKEINTRESOURCE(2));
        if (!func_addr) {
            func_addr = (bsearch_r_func)GetProcAddress(hLib, "?bsearch_r@@YAHPEAHHHH@Z");
            if (!func_addr)  func_addr = GetProcAddress(hLib, "RecursiveFunc");
        }
    }

    if (!func_addr) {
        printf("Function '%s' not found in library %s!\n", function_name, library_name);
        FreeLibrary(hLib);
        return 1;
    }

   
    if (!a) {
        printf("Array is not found!\n");
        FreeLibrary(hLib);
        return 1;
    }

 
    if (strcmp(function_name, "iterative") == 0 || strcmp(function_name, "1") == 0) {
        bsearch_func bsearch_iter = (bsearch_func)func_addr;
        int result = bsearch_iter((int*)a, SIZE, target_value);  
        if (result >= 0) {
            printf("iterative: Number %d found at index %d\n", target_value, result);
        }
        else {
            printf("iterative: Number %d not found\n", target_value);
        }
    }
    else if (strcmp(function_name, "recursive") == 0 || strcmp(function_name, "2") == 0 ){
        bsearch_r_func bsearch_rec = (bsearch_r_func)func_addr;
        int result = bsearch_rec((int*)a, target_value, 0, SIZE - 1);  
        if (result >= 0) {
            printf("recursive: Number %d found at index %d\n", target_value, result);
        }
        else {
            printf("recursive: Number %d not found\n", target_value);
        }
    }

    FreeLibrary(hLib);
    return 0;
}
