#include <windows.h>
#include <stdio.h>

int main()
{
  int  a[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  char  file_name[] = "Demo.bin";  
  HANDLE  hFile, hMapping;   // дескрипторы файла и объекта отображения
  int    *ptr;               // для указателя на массив

  // открываем файл для вывода
  hFile = CreateFileA(file_name, GENERIC_WRITE, FILE_SHARE_WRITE, NULL,
                      CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
  if (hFile == INVALID_HANDLE_VALUE) {
    fputs("File constructor failed.\n", stderr);
    return GetLastError();
  }
  // выводим исходный массив в файл и на консоль
  fputs("Initial array: ", stdout);
  for (int i = 0; i < 10; ++i) {
    WriteFile(hFile, &a[i], sizeof(int), NULL, NULL);
    fprintf(stdout, "%d ", a[i]);
  }
  fputs("\n", stdout);
  // закрываем выходной файл
  CloseHandle(hFile);
// ----------------------------------------------------------------------
  // открываем файл для отображения в память
  hFile = CreateFile(file_name, GENERIC_READ | GENERIC_WRITE,
    0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  if (hFile == INVALID_HANDLE_VALUE)
  {
    fputs("Create file failed.\n", stderr);
    return GetLastError();
  }
  // открываем объект, отображающий файл в память
  hMapping = CreateFileMapping(
        hFile,     // дескриптор открытого файла
        NULL,      // атрибуты защиты по умолчанию
        PAGE_READWRITE,  // режим доступа
        0, 0,      // размер объекта отображения равен размеру файла
        NULL);     // имя не используем
  if (!hMapping)
  {
    fputs("Create file mapping failed.\n", stderr);
    return GetLastError();
  }
  // создаем вид файла
  ptr = (int*)MapViewOfFile(
      hMapping,    // дескриптор объекта отображения
      FILE_MAP_WRITE,    // режим доступа к виду
      0, 0,        // отображаем файл с начала
      0);          // отображаем весь файл
  // изменяем значения элементов массива
  for (int i = 0; i < 10; ++i)
    ptr[i] += 10;
  // отменяем отображение файла в память
  if (!UnmapViewOfFile(ptr)) {
    fputs("Unmap view of file failed.\n", stderr);
    return GetLastError();
  }
  // закрываем объкт отображения файла в память
  if (!CloseHandle(hMapping)) {
    fputs("Close mapping failed.\n", stderr);
    return GetLastError();
  }
  // закрываем файл
  if (!CloseHandle(hFile)) {
    fputs("Close file failed.\n", stderr);
    return GetLastError();
  }
  // ----------------------------------------------------------------------
  // открываем файл для ввода
  hFile = CreateFileA(file_name, GENERIC_READ, FILE_SHARE_READ, NULL,
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

  return 0;
}