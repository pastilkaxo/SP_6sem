#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <string.h>

int bsearch_i(int* a, int n, int x);
int bsearch_r(int* a, int x, int i, int j);

int main(int argc, char* argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Не указана загружаемая библиотека или вызываемая функция!\n");
        return 1;
    }

    const char* lib_name = argv[1];
    const char* func_name = argv[2];

    void* handle = dlopen(lib_name, RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "Загружаемая библиотека не найдена: %s\n", dlerror());
        return 1;
    }

    int x;
    if (argc == 3) {
        printf("Введите искомое число: ");
        if (scanf("%d", &x) != 1) {
            fprintf(stderr, "Ошибка ввода числа!\n");
            dlclose(handle);
            return 1;
        }
    }
    else {
        x = atoi(argv[3]);
    }


    int* lib_array = dlsym(handle, "arr");
    if (!lib_array) {
        fprintf(stderr, "Не удалось найти массив arr в библиотеке %s: %s\n", lib_name, dlerror());
        dlclose(handle);
        return 1;
    }

    int array_size = 1024;


    int result = -1;


    if (strcmp(func_name, "bsearch_i") == 0) {

        int (*search_function_i)(int*, int, int) = (int (*)(int*, int, int))dlsym(handle, func_name);
        if (!search_function_i) {
            fprintf(stderr, "Искомая функция %s не найдена в библиотеке %s: %s\n", func_name, lib_name, dlerror());
            dlclose(handle);
            return 1;
        }
        result = search_function_i(lib_array, array_size, x);
    }
    else if (strcmp(func_name, "bsearch_r") == 0) {
        int (*search_function_r)(int*, int, int, int) = (int (*)(int*, int, int, int))dlsym(handle, func_name);
        if (!search_function_r) {
            fprintf(stderr, "Искомая функция %s не найдена в библиотеке %s: %s\n", func_name, lib_name, dlerror());
            dlclose(handle);
            return 1;
        }
        result = search_function_r(lib_array, x, 0, array_size - 1);
    }
    else {
        fprintf(stderr, "Неправильное имя функции: %s\n", func_name);
        dlclose(handle);
        return 1;
    }

    if (result == -1) {
        printf("%s: Заданное число не найдено!\n", func_name);
    }
    else {
        printf("%s: Число %d найдено на позиции %d!\n", func_name, x, result);
    }

    dlclose(handle);

    return 0;
}
