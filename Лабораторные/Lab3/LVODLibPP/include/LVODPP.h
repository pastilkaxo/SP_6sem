#ifdef LVODPP
#else
#define LVODPP __declspec(dllexport)
#endif 

#define ARR_SIZE 1024
extern "C" LVODPP int arr[ARR_SIZE];

LVODPP int bsearch_i(int* a, int n, int x);

LVODPP int bsearch_r(int* a, int x, int i, int j);

extern "C"  LVODPP void log_event(const char* message);

