#include <windows.h>
#include <stdio.h>

int main() {
  char service_name[] = "DemoService"; // им¤ сервиса
  SC_HANDLE hServiceControlManager, hService;
  SERVICE_STATUS service_status;

  // св¤зываемс¤ с менеджером сервисов
  hServiceControlManager =
      OpenSCManager(NULL, // локальна¤ машина
                    NULL, // активна¤ база данных сервисов
                    SC_MANAGER_CONNECT // соединение с менеджером сервисов
      );
  if (hServiceControlManager == NULL) {
    fprintf(stdout,
            "Open service control manager failed.\n"
            "The last error code: %lu\n"
            "Press any key to continue.\n",
            GetLastError());
    getchar();

    return 0;
  }

  fprintf(stdout, "Service control manager is opened.\n"
                  "Press any key to continue.\n");
  getchar();

  // открываем сервис
  hService =
      OpenService(hServiceControlManager, // дескриптор менеджера сервисов
                  service_name,      // им¤ сервиса
                  SERVICE_ALL_ACCESS // любой доступ к сервису
      );
  if (hService == NULL) {
    fprintf(stdout,
            "Open service failed.\n"
            "The last error code: %lu\n"
            "Press any key to continue.\n",
            GetLastError());
    getchar();

    // закрываем дескриптор менеджера сервисов
    CloseServiceHandle(hServiceControlManager);

    return 0;
  }

  fprintf(stdout, "Service is opened.\n"
                  "Press any key to continue.\n");
  getchar();

  // останавливаем сервис
  if (!ControlService(hService, // дескриптор сервиса
                      SERVICE_CONTROL_STOP, // управл¤юща¤ команда
                      &service_status // адрес структуры состо¤ни¤ сервиса
                      )) {
    fprintf(stdout,
            "Control service failed.\n"
            "The last error code: %lu\n"
            "Press any key to continue.\n",
            GetLastError());
    getchar();
    // закрываем дескрипторы
    CloseServiceHandle(hServiceControlManager);
    CloseServiceHandle(hService);

    return 0;
  }
  fprintf(stdout, "The service is stopped.\n"
                  "Press any key to continue.\n");
  getchar();

  // закрываем дескрипторы
  CloseServiceHandle(hServiceControlManager);
  CloseServiceHandle(hService);

  return 0;
}