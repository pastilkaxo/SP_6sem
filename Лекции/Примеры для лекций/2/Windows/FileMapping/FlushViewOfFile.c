#include <windows.h>
#include <stdio.h>


int main() {
  int a[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  char file_name[] = "C:\\Demo.bin";
  char mapping_name[] = "MappingName";
  HANDLE hFile, hMapping; // дескрипторы файла и объекта отображения
  int *ptr; // для указателя на массив

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
  //----------------------------------------------------------------------
  // открываем файл для отображения в память
  hFile = CreateFile(file_name, GENERIC_READ | GENERIC_WRITE,
                     FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING,
                     FILE_ATTRIBUTE_NORMAL, NULL);
  if (hFile == INVALID_HANDLE_VALUE) {
    fputs("Create file failed.\n", stderr);
    return GetLastError();
  }
  // открываем объект, отображающий файл в память
  hMapping =
      CreateFileMapping(hFile, // дескриптор открытого файла
                        NULL, // атрибуты защиты по умолчанию
                        PAGE_READWRITE, // режим доступа
                        0, 0, // размер объекта отображения равен размеру файла
                        mapping_name); // имя объекта отображения
  if (!hMapping) {
    fputs("Create file mapping failed.\n", stderr);
    return GetLastError();
  }
  // создаем вид файла
  ptr = (int *)MapViewOfFile(hMapping, // дескриптор объекта отображения
                             FILE_MAP_WRITE, // режим доступа к виду
                             0, 0, // отображаем файл с начала
                             0); // отображаем весь файл
  // изменяем значения элементов массива
  for (int i = 0; i < 10; ++i)
    ptr[i] += 10;
  // сбрасываем весь вид на диск
  if (!FlushViewOfFile(ptr, 0)) {
    fputs("Flush view of file failed.\n", stderr);
    return GetLastError();
  }
  //----------------------------------------------------------------------
  // создаем процесс, который будет читать данные из отображаемого
  // в память файла
  char lpszAppName[] = ".\\ConsoleProcess2.exe";

  STARTUPINFO si;
  PROCESS_INFORMATION piApp;

  ZeroMemory(&si, sizeof(STARTUPINFO));
  si.cb = sizeof(STARTUPINFO);

  // создаем новый консольный процесс
  if (!CreateProcess(lpszAppName, NULL, NULL, NULL, FALSE, CREATE_NEW_CONSOLE,
                     NULL, NULL, &si, &piApp)) {
    fputs("Create process failed.\n", stderr);
    return GetLastError();
  }
  // ждем завершения созданного прцесса
  WaitForSingleObject(piApp.hProcess, INFINITE);
  // закрываем дескрипторы этого процесса в текущем процессе
  CloseHandle(piApp.hThread);
  CloseHandle(piApp.hProcess);
  //----------------------------------------------------------------------
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
  // ждем команду на завершение процесса
  fputs("Input any char to exit: ", stdout);
  getchar();

  return 0;
}