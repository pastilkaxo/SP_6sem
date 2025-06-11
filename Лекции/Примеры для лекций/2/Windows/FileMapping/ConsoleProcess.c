#include <windows.h>
#include <stdio.h>

int main()
{
  char  MappingName[] = "MappingName";
  HANDLE  hMapping;      // дескриптор объекта, отображающего файл
  int    *ptr;           // для указателя на массив
  const int    n = 10;   // размерность массива

  fputs("This is a child process.\n", stdout);
  // открываем объект отображения файла в память
  hMapping = CreateFileMappingA(
        INVALID_HANDLE_VALUE,  // файл подкачки страниц
        NULL,            // атрибуты защиты по умолчанию
        PAGE_READWRITE,  // режим доступа: чтение и запись
        0,               // старшее слово = 0
        n * sizeof(int), // младшее слово = длине массива
        MappingName);    // имя объекта отображения
  if (!hMapping)
  {
    fputs("Create file mapping failed.\n", stderr);
    return GetLastError();
  }
  // создаем вид файла
  ptr = (int*)MapViewOfFile(
      hMapping,        // дескриптор объекта отображения
      FILE_MAP_WRITE,  // режим доступа к виду
      0,0,             // отображаем файл с начала
      0);              // отображаем весь файл
  // выводим массив из вида на консоль
  fputs("Array: ", stdout);
  for (int i = 0; i < n; ++i)
    fprintf(stdout, "%d ", ptr[i]);
  fputs("\n", stdout);
  // отменяем отображение файла в память
  if (!UnmapViewOfFile(ptr))
  {
    fputs("Unmap view of file failed.\n", stderr);
    return GetLastError();
  }
  // закрываем объект отображения файла в память
  if (!CloseHandle(hMapping))
  {
    fputs("Close file failed.\n", stderr);
    return GetLastError();
  }
  // ждем команду на завершение процесса
  fputs("Input any char to exit: ", stdout);
  getchar();

  return 0;
}