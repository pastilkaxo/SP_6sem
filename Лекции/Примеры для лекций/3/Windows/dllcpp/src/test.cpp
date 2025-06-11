/***************************************************************************
Модуль: MyExeFile1.cpp
***************************************************************************/
// сюда включаются стандартные заголовочные файлы Windows и библиотеки C
#include <windows.h>
// включаем экспортируемые структуры данных, идентификаторы, функции и
// переменные
#include "MyLib.hpp"
////////////////////////////////////////////////////////////////////////////
int WINAPI WinMain(HINSTANCE hinstExe, HINSTANCE hInst, LPTSTR pszCmdLine, int _) {
  int nLeft = 10, nRight = 25;
  TCHAR sz[100];
  wsprintf(sz, TEXT("%d + %d = %d"), nLeft, nRight, Add(nLeft, nRight));
  MessageBox(NULL, sz, TEXT("Calculation"), MB_OK);
  wsprintf(sz, TEXT("The result from the last Add is: %d"), g_nResult);
  MessageBox(NULL, sz, TEXT("Last Result"), MB_OK);
  return (0);
}
////////////////////////////// Конец файла /////////////////////////////////