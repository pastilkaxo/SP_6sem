/***************************************************************************
Модуль: MyExeFile1.cpp
***************************************************************************/
// сюда включаются стандартные заголовочные файлы Windows и библиотеки C
#include <windows.h>
// включаем экспортируемые структуры данных, идентификаторы, функции и
// переменные
////////////////////////////////////////////////////////////////////////////
int WINAPI WinMain(HINSTANCE hinstExe, HINSTANCE hInst, LPTSTR pszCmdLine, int _) {
  int nLeft = 10, nRight = 25;
  TCHAR sz[100];
  int (*Add)(int, int); 
  HMODULE hLib = LoadLibrary(TEXT("MyLib.dll"));
  Add = (int (*)(int, int)) GetProcAddress(hLib, "Add");
  wsprintf(sz, TEXT("%d + %d = %d"), nLeft, nRight, Add(nLeft, nRight));
  MessageBox(NULL, sz, TEXT("Calculation"), MB_OK);
  int* g_nResult = (int*) GetProcAddress(hLib, "g_nResult");
  wsprintf(sz, TEXT("The result from the last Add is: %d"), *g_nResult);
  MessageBox(NULL, sz, TEXT("Last Result"), MB_OK);
  FreeLibrary(hLib);
  return (0);
}
////////////////////////////// Конец файла /////////////////////////////////