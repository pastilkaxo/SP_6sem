#ifndef LVODLIB
#define LVODLIB	
#define ARR_SIZE 1024
extern int arr[ARR_SIZE];
int bsearch_i(int* a, int n, int x);
int bsearch_r(int* a, int x, int i, int j);
void log_event(const char* message);
#endif
