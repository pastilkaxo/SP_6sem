#include "testW.h"

void test_printW(const wchar_t *msg){
    fwprintf(stdout, L"This wide msg is from static library: %s\n", msg);
}