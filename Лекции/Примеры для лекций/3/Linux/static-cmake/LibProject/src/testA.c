#include "testA.h"

void test_printA(const char *msg){
    fprintf(stdout, "This msg is from static library: %s\n", msg);
}