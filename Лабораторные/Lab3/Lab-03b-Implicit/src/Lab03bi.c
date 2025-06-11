#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#define SIZE 1024 

__declspec(dllimport) int bsearch_i(int*, int, int);  
__declspec(dllimport) int RecursiveFunc(int*, int, int, int); 
__declspec(dllimport) int arr[SIZE];


int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: Lab-03b-im <Function Name> [Target Number]\n");
        return 1;
    }

    const char* function_name = argv[1];
    int target_value = -1;

    if (argc == 3) {
        target_value = atoi(argv[2]);
    }
    else {
        printf("Enter the number to search: ");
        scanf_s("%d", &target_value);
    }

    if (strcmp(function_name, "iterative") == 0 || strcmp(function_name, "1") == 0) {
        int result = bsearch_i(arr, SIZE, target_value); 
        if (result >= 0) {
            printf("iterative: Number %d found at index %d\n", target_value, result);
        }
        else {
            printf("iterative: Number %d not found\n", target_value);
        }
    }
    else if (strcmp(function_name, "recursive") == 0 || strcmp(function_name, "2") == 0) {
        int result = RecursiveFunc(arr, target_value, 0, SIZE - 1);
        if (result >= 0) {
            printf("recursive: Number %d found at index %d\n", target_value, result);
        }
        else {
            printf("recursive: Number %d not found\n", target_value);
        }
    }
    else {
        printf("Function '%s' is empty!\n", function_name);
        return 1;
    }

    return 0;
}
