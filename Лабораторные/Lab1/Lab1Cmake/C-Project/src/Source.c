#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <float.h>

int main() {
    double x;
    printf("Type X: ");
    if (scanf("%lf", &x) != 1) {
        printf("Bad value!\n");
        while (getchar() != '\n');
        getchar();
        return 1;
    }

    printf("x = %lf\n", x);
    double x2, x4, x5, x8, x13;

    x2 = x * x;
    x4 = x2 * x2;
    x5 = x4 * x;
    if (x5 > DBL_MAX || x5 < -DBL_MAX) {
        printf("Bad value! x5\n");
        while (getchar() != '\n');
        getchar();
        return 1;
    }
    printf("x^5 = %lf\n", x5);

    x8 = x4 * x4;
    x13 = x8 * x5;
    if (x13 > DBL_MAX || x13 < -DBL_MAX) {
        printf("Bad value! x13\n");
        while (getchar() != '\n');
        getchar();
        return 1;
    }
    printf("x^13 = %lf\n", x13);

    getchar();
    getchar();
    return 0;
}