#include <windows.h>
#include <stdio.h>


int main() {
  char file_name[] = "C:\\Demo.bin";
  int a[10];

  // открываем файл для ввода
  HANDLE hFile = CreateFileA(file_name, GENERIC_READ, FILE_SHARE_READ, NULL,
                             OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  if (hFile == INVALID_HANDLE_VALUE) {
    fputs("File constructor failed.\n", stderr);
    return GetLastError();
  }
  // вводим финальный массив из файла и выводим на консоль
  fputs("Final array: ", stdout);
  for (int i = 0; i < 10; ++i) {
    ReadFile(hFile, &a[i], sizeof(int), NULL, NULL);
    fprintf(stdout, "%d ", a[i]);
  }
  fputs("\n", stdout);
  // закрываем входной файл
  CloseHandle(hFile);

  // ждем команду на завершение процесса
  fputs("Input any char to exit: ", stdout);
  
  return 0;
}