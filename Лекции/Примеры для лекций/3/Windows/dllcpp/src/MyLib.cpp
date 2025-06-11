/***************************************************************************
Модуль: MyLib.c
***************************************************************************/
// сюда включаются стандартные заголовочные файлы Windows и библиотеки C
#include <windows.h>
// этот файл исходного кода DLL экспортирует функции и переменные
#define MYLIBAPI extern __declspec(dllexport)
// включаем экспортируемые структуры данных, идентификаторы, функции и
// переменные
#include "MyLib.hpp"
////////////////////////////////////////////////////////////////////////////
// здесь размещается исходный код этой DLL
int g_nResult;
int Add(int nLeft, int nRight) {
  g_nResult = nLeft + nRight;
  return (g_nResult);
}

BOOL WINAPI DllMain(HINSTANCE hinstDll, DWORD fdwReason, PVOID fImpLoad) {
  switch (fdwReason) {
  case DLL_PROCESS_ATTACH:
    // DLL проецируется на адресное пространство процесса
    break;
  case DLL_THREAD_ATTACH:
    // создается поток
    break;
  case DLL_THREAD_DETACH:
    // поток корректно завершается
    break;
  case DLL_PROCESS_DETACH:
    // DLL отключается от адресного пространства процесса
    break;
  }
  return (TRUE); // используется только для DLL_PROCESS_ATTACH
}
////////////////////////////// Конец файла ////////////////////////////////