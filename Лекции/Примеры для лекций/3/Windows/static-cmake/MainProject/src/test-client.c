#include "testA.h"
#include "testW.h"
#include <windows.h>

int main() {
    test_printA("Hello, World!");
    test_printW(L"Hello, World!");
    system("pause");
}
