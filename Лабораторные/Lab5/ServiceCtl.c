#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define SERVICE_CONTROL_TEST 137
#define SERVICE_CONTROL_RELOAD_CONFIG 138

void InstallService(const char* service_name, const char* binary_path);
void LaunchService(const char* service_name, const char* config_path);
void TerminateService(const char* service_name);
void RemoveService(const char* service_name);
void SuspendService(const char* service_name);
void ResumeService(const char* service_name);
void GetServiceStatus(const char* service_name);
void SendTestSignal(const char* service_name);
void ReloadConfig(const char* service_name);
void PrintUsage(const char* program_name);

int main(int argc, char* argv[]) {
    SetConsoleOutputCP(CP_UTF8);

    if (argc < 3) {
        PrintUsage(argv[0]);
        return 1;
    }

    const char* service_name = argv[1];
    const char* operation = argv[2];

    if (_stricmp(operation, "Create") == 0) {
        if (argc != 4) {
            printf("Использование: %s %s Create <путь_к_файлу_сервиса>\n", argv[0], service_name);
            return 1;
        }
        InstallService(service_name, argv[3]);
    }
    else if (_stricmp(operation, "Start") == 0) {
        if (argc != 4) {
            printf("Использование: %s %s Start <путь_к_конфигурации>\n", argv[0], service_name);
            return 1;
        }
        LaunchService(service_name, argv[3]);
    }
    else if (_stricmp(operation, "Stop") == 0) {
        TerminateService(service_name);
    }
    else if (_stricmp(operation, "Delete") == 0 || _stricmp(operation, "Uninstall") == 0) {
        RemoveService(service_name);
    }
    else if (_stricmp(operation, "Pause") == 0) {
        SuspendService(service_name);
    }
    else if (_stricmp(operation, "Continue") == 0) {
        ResumeService(service_name);
    }
    else if (_stricmp(operation, "Info") == 0) {
        GetServiceStatus(service_name);
    }
    else if (_stricmp(operation, "Test") == 0) {
        SendTestSignal(service_name);
    }
    else if (_stricmp(operation, "ReloadConfig") == 0) {
        ReloadConfig(service_name);
    }
    else {
        printf("Неизвестная операция: %s\n", operation);
        PrintUsage(argv[0]);
        return 1;
    }

    return 0;
}

void PrintUsage(const char* program_name) {
    printf("Программа: %s\n", program_name);
    printf("Описание: Управление сервисами в Windows.\n\n");

    printf("Доступные операции:\n");

    printf("  %s\n    - Установка сервиса\n    - Требуется путь к исполняемому файлу\n", "Create");
    printf("  %s\n    - Запуск сервиса\n    - Требуется путь к конфигурационному файлу\n", "Start");
    printf("  %s\n    - Остановка сервиса\n", "Stop");
    printf("  %s, %s\n    - Удаление сервиса\n", "Delete", "Uninstall");
    printf("  %s\n    - Приостановка сервиса\n", "Pause");
    printf("  %s\n    - Возобновление работы сервиса\n", "Continue");
    printf("  %s\n    - Получение информации о состоянии сервиса\n", "Info");
    printf("  %s\n    - Отправка тестового сигнала сервису\n", "Test");
    printf("  %s\n    - Перезагрузка конфигурации сервиса без перезапуска\n", "ReloadConfig");

    printf("\nПримечание: Для выполнения операций с сервисами необходимо запускать программу с правами администратора.\n");
}



void InstallService(const char* service_name, const char* binary_path) {
          
    SC_HANDLE service_manager = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
    if (!service_manager) {
        printf("Ошибка при открытии менеджера сервисов (%lu)\n", GetLastError());
        return;
    }

    char username[256] = {0};
    DWORD username_len = sizeof(username);
    if (!GetUserName(username, &username_len)) {
        printf("Ошибка при получении имени текущего пользователя (%lu)\n", GetLastError());
        CloseServiceHandle(service_manager);
        return;
    }

    char account_name[512] = {0};
    snprintf(account_name, sizeof(account_name), ".\\%s", username);
    
    SC_HANDLE service = CreateService(
        service_manager,
        service_name,
        service_name,
        SERVICE_ALL_ACCESS,
        SERVICE_WIN32_OWN_PROCESS,
        SERVICE_AUTO_START,
        SERVICE_ERROR_NORMAL,
        binary_path,
        NULL, // не нужно указывать имя пользователя
        NULL, // не нужно указывать пароль
        NULL,
        NULL, // используем LocalSystem
        NULL // пароль тоже не нужен                      
    );

    if (service) {
        printf("Сервис '%s' успешно установлен от имени пользователя %s\n", service_name, account_name);

        SERVICE_DESCRIPTION desc;
        desc.lpDescription = "Сервис резервного копирования файлов";
        ChangeServiceConfig2(service, SERVICE_CONFIG_DESCRIPTION, &desc);
        
        
        SERVICE_DELAYED_AUTO_START_INFO delayed_info;
        delayed_info.fDelayedAutostart = TRUE;
        ChangeServiceConfig2(service, SERVICE_CONFIG_DELAYED_AUTO_START_INFO, &delayed_info);
        
        CloseServiceHandle(service);
    }
    else {
        printf("Ошибка при создании сервиса (%lu)\n", GetLastError());
    }

    CloseServiceHandle(service_manager);
}

void LaunchService(const char* service_name, const char* config_path) {
   
    SC_HANDLE service_manager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
    if (!service_manager) {
        printf("Ошибка при открытии менеджера сервисов (%lu)\n", GetLastError());
        return;
    }

    SC_HANDLE service = OpenService(service_manager, service_name, SERVICE_START);
    if (!service) {
        printf("Ошибка при открытии сервиса '%s' (%lu)\n", service_name, GetLastError());
        CloseServiceHandle(service_manager);
        return;
    }

    const char* args[] = { config_path };

    if (StartService(service, 1, args)) {
        printf("Сервис '%s' успешно запущен с конфигурацией: %s\n", service_name, config_path);
    }
    else {
        DWORD error = GetLastError();
        if (error == ERROR_SERVICE_ALREADY_RUNNING) {
            printf("Сервис '%s' уже запущен\n", service_name);
        }
        else {
            printf("Ошибка при запуске сервиса (%lu)\n", error);
        }
    }

    CloseServiceHandle(service);
    CloseServiceHandle(service_manager);
}

void TerminateService(const char* service_name) {
   
    SC_HANDLE service_manager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
    if (!service_manager) {
        printf("Ошибка при открытии менеджера сервисов (%lu)\n", GetLastError());
        return;
    }

    SC_HANDLE service = OpenService(service_manager, service_name, SERVICE_STOP | SERVICE_QUERY_STATUS);
    if (!service) {
        printf("Ошибка при открытии сервиса '%s' (%lu)\n", service_name, GetLastError());
        CloseServiceHandle(service_manager);
        return;
    }

    SERVICE_STATUS status;
    if (QueryServiceStatus(service, &status) && status.dwCurrentState == SERVICE_STOPPED) {
        printf("Сервис '%s' уже остановлен\n", service_name);
    }
    else {
        printf("Отправка сигнала остановки сервису '%s'...\n", service_name);
        
        if (ControlService(service, SERVICE_CONTROL_STOP, &status)) {
            while (QueryServiceStatus(service, &status)) {
                if (status.dwCurrentState == SERVICE_STOPPED) {
                    printf("Сервис '%s' успешно остановлен\n", service_name);
                    break;
                }
                Sleep(500); 
            }
        }
        else {
            printf("Ошибка при остановке сервиса (%lu)\n", GetLastError());
        }
    }

    CloseServiceHandle(service);
    CloseServiceHandle(service_manager);
}

void RemoveService(const char* service_name) {
   
    SC_HANDLE service_manager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
    if (!service_manager) {
        printf("Ошибка при открытии менеджера сервисов (%lu)\n", GetLastError());
        return;
    }

    SC_HANDLE service = OpenService(service_manager, service_name, DELETE | SERVICE_STOP | SERVICE_QUERY_STATUS);
    if (!service) {
        printf("Ошибка при открытии сервиса '%s' (%lu)\n", service_name, GetLastError());
        CloseServiceHandle(service_manager);
        return;
    }

    SERVICE_STATUS status;
    if (QueryServiceStatus(service, &status) && status.dwCurrentState != SERVICE_STOPPED) {
        
        printf("Остановка сервиса '%s' перед удалением...\n", service_name);
        if (ControlService(service, SERVICE_CONTROL_STOP, &status)) {   
            while (QueryServiceStatus(service, &status) && status.dwCurrentState != SERVICE_STOPPED) {
                Sleep(500);
            }
        }
    }

    if (DeleteService(service)) {
        printf("Сервис '%s' успешно удален\n", service_name);
    }
    else {
        printf("Ошибка при удалении сервиса (%lu)\n", GetLastError());
    }

    CloseServiceHandle(service);
    CloseServiceHandle(service_manager);
}

void SuspendService(const char* service_name) {
   
    SC_HANDLE service_manager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
    if (!service_manager) {
        printf("Ошибка при открытии менеджера сервисов (%lu)\n", GetLastError());
        return;
    }

    SC_HANDLE service = OpenService(service_manager, service_name, SERVICE_PAUSE_CONTINUE | SERVICE_QUERY_STATUS);
    if (!service) {
        printf("Ошибка при открытии сервиса '%s' (%lu)\n", service_name, GetLastError());
        CloseServiceHandle(service_manager);
        return;
    }

    SERVICE_STATUS status;
    if (QueryServiceStatus(service, &status)) {
        if (status.dwCurrentState == SERVICE_RUNNING) {
            if (ControlService(service, SERVICE_CONTROL_PAUSE, &status)) {
                printf("Сервис '%s' приостановлен\n", service_name);
            }
            else {
                printf("Ошибка при приостановке сервиса (%lu)\n", GetLastError());
            }
        }
        else if (status.dwCurrentState == SERVICE_PAUSED) {
            printf("Сервис '%s' уже приостановлен\n", service_name);
        }
        else {
            printf("Сервис '%s' не запущен (текущее состояние: %lu)\n", service_name, status.dwCurrentState);
        }
    }
    else {
        printf("Ошибка при получении состояния сервиса (%lu)\n", GetLastError());
    }

    CloseServiceHandle(service);
    CloseServiceHandle(service_manager);
}

void ResumeService(const char* service_name) {
   
    SC_HANDLE service_manager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
    if (!service_manager) {
        printf("Ошибка при открытии менеджера сервисов (%lu)\n", GetLastError());
        return;
    }

    SC_HANDLE service = OpenService(service_manager, service_name, SERVICE_PAUSE_CONTINUE | SERVICE_QUERY_STATUS);
    if (!service) {
        printf("Ошибка при открытии сервиса '%s' (%lu)\n", service_name, GetLastError());
        CloseServiceHandle(service_manager);
        return;
    }

    SERVICE_STATUS status;
    if (QueryServiceStatus(service, &status)) {
        if (status.dwCurrentState == SERVICE_PAUSED) {
            if (ControlService(service, SERVICE_CONTROL_CONTINUE, &status)) {
                printf("Работа сервиса '%s' возобновлена\n", service_name);
            }
            else {
                printf("Ошибка при возобновлении работы сервиса (%lu)\n", GetLastError());
            }
        }
        else if (status.dwCurrentState == SERVICE_RUNNING) {
            printf("Сервис '%s' уже работает\n", service_name);
        }
        else {
            printf("Сервис '%s' не в приостановленном состоянии (текущее состояние: %lu)\n", 
                  service_name, status.dwCurrentState);
        }
    }
    else {
        printf("Ошибка при получении состояния сервиса (%lu)\n", GetLastError());
    }

    CloseServiceHandle(service);
    CloseServiceHandle(service_manager);
}

void GetServiceStatus(const char* service_name) {
    SC_HANDLE service_manager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
    if (!service_manager) {
        printf("Ошибка при открытии менеджера сервисов (%lu)\n", GetLastError());
        return;
    }

    SC_HANDLE service = OpenService(service_manager, service_name, 
                                  SERVICE_QUERY_STATUS | SERVICE_QUERY_CONFIG);
    if (!service) {
        printf("Ошибка при открытии сервиса '%s' (%lu)\n", service_name, GetLastError());
        CloseServiceHandle(service_manager);
        return;
    }

    SERVICE_STATUS status;
    if (QueryServiceStatus(service, &status)) {
        printf("Информация о сервисе '%s':\n", service_name);
        
        printf("Состояние: ");
        switch (status.dwCurrentState) {
            case SERVICE_STOPPED:
                printf("Остановлен\n");
                break;
            case SERVICE_START_PENDING:
                printf("Запускается\n");
                break;
            case SERVICE_STOP_PENDING:
                printf("Останавливается\n");
                break;
            case SERVICE_RUNNING:
                printf("Работает\n");
                break;
            case SERVICE_CONTINUE_PENDING:
                printf("Возобновляется\n");
                break;
            case SERVICE_PAUSE_PENDING:
                printf("Приостанавливается\n");
                break;
            case SERVICE_PAUSED:
                printf("Приостановлен\n");
                break;
            default:
                printf("Неизвестно (%lu)\n", status.dwCurrentState);
                break;
        }
        
        DWORD bytes_needed = 0;
        QueryServiceConfig(service, NULL, 0, &bytes_needed);
        
        if (bytes_needed > 0) {
            LPQUERY_SERVICE_CONFIG config = (LPQUERY_SERVICE_CONFIG)malloc(bytes_needed);
            if (config && QueryServiceConfig(service, config, bytes_needed, &bytes_needed)) {
                printf("Путь к исполняемому файлу: %s\n", config->lpBinaryPathName);
                printf("Тип запуска: ");
                switch (config->dwStartType) {
                    case SERVICE_AUTO_START:
                        printf("Автоматически\n");
                        break;
                    case SERVICE_DEMAND_START:
                        printf("Вручную\n");
                        break;
                    case SERVICE_DISABLED:
                        printf("Отключен\n");
                        break;
                    default:
                        printf("Другой (%lu)\n", config->dwStartType);
                        break;
                }
                printf("Учетная запись: %s\n", config->lpServiceStartName);
            }
            
            if (config) free(config);
        }
    }
    else {
        printf("Ошибка при получении состояния сервиса (%lu)\n", GetLastError());
    }

    CloseServiceHandle(service);
    CloseServiceHandle(service_manager);
}

void SendTestSignal(const char* service_name) {
    SC_HANDLE service_manager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
    if (!service_manager) {
        printf("Ошибка при открытии менеджера сервисов (%lu)\n", GetLastError());
        return;
    }

    SC_HANDLE service = OpenService(service_manager, service_name, SERVICE_USER_DEFINED_CONTROL);
    if (!service) {
        printf("Ошибка при открытии сервиса '%s' (%lu)\n", service_name, GetLastError());
        CloseServiceHandle(service_manager);
        return;
    }

    SERVICE_STATUS status;
    if (ControlService(service, SERVICE_CONTROL_TEST, &status)) {
        printf("Тестовый сигнал успешно отправлен сервису '%s'\n", service_name);
    }
    else {
        printf("Ошибка при отправке тестового сигнала (%lu)\n", GetLastError());
    }

    CloseServiceHandle(service);
    CloseServiceHandle(service_manager);
}

void ReloadConfig(const char* service_name) {
    SC_HANDLE service_manager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
    if (!service_manager) {
        printf("Ошибка при открытии менеджера сервисов (%lu)\n", GetLastError());
        return;
    }

    SC_HANDLE service = OpenService(service_manager, service_name, SERVICE_USER_DEFINED_CONTROL);
    if (!service) {
        printf("Ошибка при открытии сервиса '%s' (%lu)\n", service_name, GetLastError());
        CloseServiceHandle(service_manager);
        return;
    }

    SERVICE_STATUS status;
    if (ControlService(service, SERVICE_CONTROL_RELOAD_CONFIG, &status)) {
        printf("Сигнал перезагрузки конфигурации успешно отправлен сервису '%s'\n", service_name);
    }
    else {
        printf("Ошибка при отправке сигнала перезагрузки конфигурации (%lu)\n", GetLastError());
    }

    CloseServiceHandle(service);
    CloseServiceHandle(service_manager);
}