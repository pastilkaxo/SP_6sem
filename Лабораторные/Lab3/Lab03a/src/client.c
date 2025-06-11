#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LVO.h"



int main(int argc, char* argv[]) {
    int x;

    if (argc == 1) {
        printf("Usage: Lab-03a <function_name> [target_number]\n");
        return 1;
    }

    char* function_name = argv[1];

    if (argc == 3) {
        x = atoi(argv[2]);
        printf("X:%d\n", x);
    }
    else {
        printf("Enter the number to search: ");
        scanf("%d", &x);
        printf("X:%d\n", x);
    }


    int i = -1;

    if (strcmp(function_name, "iterative") == 0) {
        i = bsearch_i(arr, ARR_SIZE, x);  
    }
    else if (strcmp(function_name, "recursive") == 0) {
        i = bsearch_r(arr, x, 0, ARR_SIZE - 1); 
    }
    else {
        printf("Function empty!\n");
        return 1;
    }


    if (i >= 0) {
        printf("%d is at index %d.\n", x, i);
    }
    else {
        printf("%d is not found.\n", x);
    }

    return 0;
}
