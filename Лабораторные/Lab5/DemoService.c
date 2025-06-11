#include <Windows.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <direct.h>
#include <shlwapi.h>
#include <stdlib.h>
#include <stdint.h>
#pragma comment(lib, "shlwapi.lib")
#define _CRT_SECURE_NO_WARNINGS 

#define DEFAULT_BACKUP_DIR  "C:\\Users\\Влад\\AppData\\Roaming\\DemoService\\Reserved"
#define DEFAULT_LOG_DIR     "C:\\Users\\Влад\\AppData\\Roaming\\DemoService\\Logs"
#define DEFAULT_MONITOR_DIR "C:\\Users\\Влад\\AppData\\Roaming\\DemoService\\Source"

#define SERVICE_CONTROL_TEST 137
#define SERVICE_CONTROL_RELOAD_CONFIG 138  

typedef struct {
    char monitor_directory[MAX_PATH];
    char backup_directory[MAX_PATH];
    char log_directory[MAX_PATH];
    int sync_interval;
    BOOL is_suspended;
} BackupServiceConfig;

char service_title[] = "DemoService";
SERVICE_STATUS service_state;
SERVICE_STATUS_HANDLE hServiceState;
HANDLE hJournalFile = INVALID_HANDLE_VALUE;
char journal_filepath[MAX_PATH];
BackupServiceConfig config;
char config_file_path[MAX_PATH];

VOID WINAPI BackupServiceMain(DWORD argc, LPTSTR* argv);
VOID WINAPI BackupServiceHandler(DWORD control_code);
void WriteToJournal(const char* message, BOOL is_error);
void LoadConfiguration(const char* config_file);
void SynchronizeDirectories();
void EnsureDirectoryExists(const char* directory_path, const char* dir_purpose);
DWORD WINAPI BackupThread(LPVOID param);
void ExpandEnvironmentPath(const char* source_path, char* dest_path, size_t dest_size);


void InitializeConfig(BackupServiceConfig* config) {
    // Инициализируем структуру с дефолтными значениями
    strcpy_s(config->monitor_directory, MAX_PATH, DEFAULT_MONITOR_DIR);
    strcpy_s(config->backup_directory, MAX_PATH, DEFAULT_BACKUP_DIR);
    strcpy_s(config->log_directory, MAX_PATH, DEFAULT_LOG_DIR);
    config->sync_interval = 1; // Например, дефолтное значение для интервала
    config->is_suspended = FALSE; // По умолчанию сервис не приостановлен
}

// главная функция консольного приложения
int main() {
    BackupServiceConfig config;
    InitializeConfig(&config);
    SetConsoleOutputCP(CP_UTF8);
    // инициализируем структуру сервисов
    SERVICE_TABLE_ENTRY dispatcher_table[] = {
        {service_title, BackupServiceMain}, // имя сервиса и функция сервиса 
        {NULL, NULL} // больше нет сервисов
    };

    if (!StartServiceCtrlDispatcher(dispatcher_table)) { // запуск диспетчера сервиса, который является потоком и управляет этим сервисом ( 30 сек)
        char error_msg[256];
        sprintf(error_msg, "Ошибка при запуске диспетчера сервисов. Код: %lu", GetLastError());
        WriteToJournal(error_msg, TRUE);
        return GetLastError();
    }

    return 0;
}



// точка входа сервиса
VOID WINAPI BackupServiceMain(DWORD argc, LPTSTR* argv) {
    // Диспетчер сервиса получает управляющие сигналы от менеджера 
    // сервисов по именованному каналу и передает эти запросы функции BackupServiceHandler, которая обрабатывает эти управляющие запросы

    // 1) регистрируем обработчик запросов для сервиса
    hServiceState = RegisterServiceCtrlHandler(
        service_title, // имя сервиса
        BackupServiceHandler); // функция обработчик управляющих команд

    if (!hServiceState) {
        return;
    }

    // 2) инициализируем структуру состояния сервиса
    service_state.dwServiceType = SERVICE_WIN32_OWN_PROCESS; // тип сервиса (самостоятельный процесс)
    service_state.dwCurrentState = SERVICE_START_PENDING; // стартующее состоняние сервиса (сервис стартует)
    service_state.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE | SERVICE_ACCEPT_PRESHUTDOWN | SERVICE_ACCEPT_PARAMCHANGE; // список возможных команд управления
    service_state.dwWin32ExitCode = NO_ERROR; // код возврата
    service_state.dwServiceSpecificExitCode = 0; // специф. код возврата сервиса
    service_state.dwCheckPoint = 0; // контрольная точка ( счетчик)
    service_state.dwWaitHint = 10000; // период ожидания команды ( из состояния в состояние перед новым SetServiceStatus)
    SetServiceStatus(hServiceState, &service_state); // 3) устанавливаем новое состояние сервиса

    config.is_suspended = FALSE;
    memset(config.monitor_directory, 0, MAX_PATH);
    memset(config.backup_directory, 0, MAX_PATH);
    memset(config.log_directory, 0, MAX_PATH);
    config.sync_interval = 0;

    char temp_log_dir[MAX_PATH] = "C:\\Temp";
    if (!PathFileExists(temp_log_dir)) {
        _mkdir(temp_log_dir);
    }

    time_t current_time = time(NULL);
    struct tm time_info_data;
    struct tm* time_info = &time_info_data;
    localtime_s(&time_info_data, &current_time);

    snprintf(journal_filepath, MAX_PATH, "%s\\%04d%02d%02d%02d%02d%02d-service-temp.log",
        temp_log_dir, time_info->tm_year + 1900, time_info->tm_mon + 1, time_info->tm_mday,
        time_info->tm_hour, time_info->tm_min, time_info->tm_sec);

    if (argc > 1) {
        WriteToJournal("Загрузка конфигурации из переданного файла...", FALSE);
        LoadConfiguration(argv[1]);
    }
    else {
        WriteToJournal("Нет указанного файла конфигурации. Работа сервиса невозможна.", TRUE);
        service_state.dwCurrentState = SERVICE_STOPPED;
        service_state.dwWin32ExitCode = ERROR_FILE_NOT_FOUND;
        SetServiceStatus(hServiceState, &service_state);
        return;
    }

    if (strlen(config.monitor_directory) == 0 ||
        strlen(config.backup_directory) == 0 ||
        strlen(config.log_directory) == 0 ||
        config.sync_interval <= 0) {

        WriteToJournal("Ошибка: Не все необходимые параметры указаны в конфигурационном файле", TRUE);
        service_state.dwCurrentState = SERVICE_STOPPED;
        service_state.dwWin32ExitCode = ERROR_INVALID_PARAMETER;
        SetServiceStatus(hServiceState, &service_state);
        return;
    }

    EnsureDirectoryExists(config.log_directory, "журнальный");
    EnsureDirectoryExists(config.backup_directory, "резервный");
    EnsureDirectoryExists(config.monitor_directory, "исходный");

    if (hJournalFile != INVALID_HANDLE_VALUE) {
        CloseHandle(hJournalFile);
        hJournalFile = INVALID_HANDLE_VALUE;
    }

    snprintf(journal_filepath, MAX_PATH, "%s\\%04d%02d%02d%02d%02d%02d-service.log",
        config.log_directory, time_info->tm_year + 1900, time_info->tm_mon + 1, time_info->tm_mday,
        time_info->tm_hour, time_info->tm_min, time_info->tm_sec);

    if (!PathFileExists(config.monitor_directory)) {
        WriteToJournal("Директория для мониторинга не существует! Сервис будет остановлен.", TRUE);
        service_state.dwCurrentState = SERVICE_STOPPED;
        service_state.dwWin32ExitCode = ERROR_PATH_NOT_FOUND;
        SetServiceStatus(hServiceState, &service_state);
        return;
    }

    char startup_info[512];
    snprintf(startup_info, sizeof(startup_info),
        "Успех! Сервис %s запущен с параметрами исходный=%s, резервный=%s, интервал=%d мин",
        service_title, config.monitor_directory, config.backup_directory, config.sync_interval);
    WriteToJournal(startup_info, FALSE);

    service_state.dwCurrentState = SERVICE_RUNNING;
    SetServiceStatus(hServiceState, &service_state);

    HANDLE hThread = CreateThread(NULL, 0, BackupThread, NULL, 0, NULL);
    if (hThread) {
        WaitForSingleObject(hThread, INFINITE);
        CloseHandle(hThread);
    }
}

DWORD WINAPI BackupThread(LPVOID param) {
    while (service_state.dwCurrentState == SERVICE_RUNNING) {
        if (!config.is_suspended) {
            SynchronizeDirectories();
        }
        Sleep(config.sync_interval * 60 * 1000);
    }
    return 0;
}

// обработчик запросов
VOID WINAPI BackupServiceHandler(DWORD control_code) {
    char status_message[512];

    switch (control_code) {
    case SERVICE_CONTROL_STOP: // остановить сервис
    case SERVICE_CONTROL_SHUTDOWN: // закончить работу сервиса
        snprintf(status_message, sizeof(status_message),
            "Успех! Сервис %s сменил состояние с %s на ОСТАНОВЛЕН",
            service_title, config.is_suspended ? "ПРИОСТАНОВЛЕН" : "РАБОТАЕТ");
        WriteToJournal(status_message, FALSE);

        service_state.dwCurrentState = SERVICE_STOPPED;
        if (hJournalFile != INVALID_HANDLE_VALUE) {
            CloseHandle(hJournalFile);
        }
        break;

    case SERVICE_CONTROL_PAUSE: // приостановить сервис
        config.is_suspended = TRUE;
        service_state.dwCurrentState = SERVICE_PAUSED;
        snprintf(status_message, sizeof(status_message),
            "Успех! Сервис %s сменил состояние с РАБОТАЕТ на ПРИОСТАНОВЛЕН", service_title);
        WriteToJournal(status_message, FALSE);
        break;

    case SERVICE_CONTROL_CONTINUE: // возобновить сервис
        config.is_suspended = FALSE;
        service_state.dwCurrentState = SERVICE_RUNNING;
        snprintf(status_message, sizeof(status_message),
            "Успех! Сервис %s сменил состояние с ПРИОСТАНОВЛЕН на РАБОТАЕТ", service_title);
        WriteToJournal(status_message, FALSE);
        break;

    case SERVICE_CONTROL_PRESHUTDOWN: // выполнить действие перед выключением сервиса
        WriteToJournal("Получен сигнал PRESHUTDOWN", FALSE);
        break;

    case SERVICE_CONTROL_TEST:
        snprintf(status_message, sizeof(status_message),
            "Привет, это тестовый код из сервиса %s!", service_title);
        WriteToJournal(status_message, FALSE);
        break;

    case SERVICE_CONTROL_RELOAD_CONFIG:
        WriteToJournal("Получен сигнал перезагрузки конфигурации", FALSE);
        LoadConfiguration(NULL);




        if (hJournalFile != INVALID_HANDLE_VALUE) {
            CloseHandle(hJournalFile);
            hJournalFile = INVALID_HANDLE_VALUE;
        }


        time_t current_time = time(NULL);
        struct tm time_info_data;
        struct tm* time_info = &time_info_data;
        localtime_s(&time_info_data, &current_time);

        snprintf(journal_filepath, MAX_PATH, "%s\\%04d%02d%02d%02d%02d%02d-service.log",
            config.log_directory, time_info->tm_year + 1900, time_info->tm_mon + 1, time_info->tm_mday,
            time_info->tm_hour, time_info->tm_min, time_info->tm_sec);

        EnsureDirectoryExists(config.log_directory, "журнальный");
        EnsureDirectoryExists(config.backup_directory, "резервный");
        EnsureDirectoryExists(config.monitor_directory, "исходный");

        WriteToJournal("Журнальный файл перемещен в новую директорию", FALSE);
        break;

    default:
        service_state.dwCheckPoint++;
        break;
    }

    SetServiceStatus(hServiceState, &service_state);
}

void WriteToJournal(const char* message, BOOL is_error) { // отображать в журнале 
    if (hJournalFile == INVALID_HANDLE_VALUE) {
        hJournalFile = CreateFile(journal_filepath, FILE_APPEND_DATA, FILE_SHARE_READ,
            NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    }

    if (hJournalFile != INVALID_HANDLE_VALUE) {
        char timestamp[32];
        time_t now = time(NULL);
        struct tm t_data;
        struct tm* t = &t_data;
        localtime_s(&t_data, &now);
        snprintf(timestamp, sizeof(timestamp), "[%02d:%02d:%02d] ", t->tm_hour, t->tm_min, t->tm_sec);

        char full_message[1024];
        snprintf(full_message, sizeof(full_message), "%s%s%s\r\n", timestamp,
            is_error ? "Попытка выполнения операции сервисом провалена! " : "", message);

        DWORD bytes_written;
        WriteFile(hJournalFile, full_message, (DWORD)strlen(full_message), &bytes_written, NULL);
    }
}

void ExpandEnvironmentPath(const char* source_path, char* dest_path, size_t dest_size) {
    char temp_path[MAX_PATH];

    strcpy_s(temp_path, MAX_PATH, source_path);

    if (strstr(temp_path, "%APPDATA%") != NULL) {
        char appdata_path[MAX_PATH];
        if (ExpandEnvironmentStrings("%APPDATA%", appdata_path, MAX_PATH)) {
            char new_path[MAX_PATH];
            char* pos = strstr(temp_path, "%APPDATA%");

            size_t prefix_len = pos - temp_path;
            strncpy_s(new_path, MAX_PATH, temp_path, prefix_len);
            new_path[prefix_len] = '\0';

            strcat_s(new_path, MAX_PATH, appdata_path);
            strcat_s(new_path, MAX_PATH, pos + 9);

            strcpy_s(temp_path, MAX_PATH, new_path);
        }
    }

    strcpy_s(dest_path, dest_size, temp_path);
}

void LoadConfiguration(const char* config_file) {
    const char* file_to_use = config_file;
    if (!file_to_use) {
        if (strlen(config_file_path) == 0) {
            WriteToJournal("Ошибка: Путь к конфигурационному файлу не задан", TRUE);
            return;
        }
        file_to_use = config_file_path;
        WriteToJournal("Выполняем перезагрузку конфигурации", FALSE);
    }
    else {
        strcpy_s(config_file_path, MAX_PATH, config_file);
    }

    if (!PathFileExists(file_to_use)) {
        char error_msg[MAX_PATH + 100];
        snprintf(error_msg, sizeof(error_msg), "Конфигурационный файл %s не найден!", file_to_use);
        WriteToJournal(error_msg, TRUE);
        return;
    }

    char buffer[MAX_PATH] = { 0 };

    // Загрузка параметров из конфигурации или использование значений по умолчанию
    if (GetPrivateProfileString("Configuration", "MonitorDir", "", buffer, MAX_PATH, file_to_use) > 0) {
        strcpy_s(config.monitor_directory, MAX_PATH, buffer);
    }
    else {
        // Используем значение по умолчанию
        strcpy_s(config.monitor_directory, MAX_PATH, DEFAULT_MONITOR_DIR);
    }

    if (GetPrivateProfileString("Configuration", "BackupDir", "", buffer, MAX_PATH, file_to_use) > 0) {
        strcpy_s(config.backup_directory, MAX_PATH, buffer);
    }
    else {
        // Используем значение по умолчанию
        strcpy_s(config.backup_directory, MAX_PATH, DEFAULT_BACKUP_DIR);
    }

    if (GetPrivateProfileString("Configuration", "LogDir", "", buffer, MAX_PATH, file_to_use) > 0) {
        strcpy_s(config.log_directory, MAX_PATH, buffer);
    }
    else {
        // Используем значение по умолчанию
        strcpy_s(config.log_directory, MAX_PATH, DEFAULT_LOG_DIR);
    }

    config.sync_interval = GetPrivateProfileInt("Configuration", "SyncInterval", 0, file_to_use);
    if (config.sync_interval <= 0) {
        WriteToJournal("Ошибка: SyncInterval не указан или имеет недопустимое значение в конфигурационном файле", TRUE);
    }

    if (!config_file) {
        char config_info[512];
        snprintf(config_info, sizeof(config_info),
            "Конфигурация перезагружена: исходный=%s, резервный=%s, журнал=%s, интервал=%d мин",
            config.monitor_directory, config.backup_directory, config.log_directory, config.sync_interval);
        WriteToJournal(config_info, FALSE);
    }
}


void SynchronizeDirectories() { // копирование файлов 
    char search_pattern[MAX_PATH];
    snprintf(search_pattern, sizeof(search_pattern), "%s\\*.*", config.monitor_directory);

    WIN32_FIND_DATA file_data;
    HANDLE hFind = FindFirstFile(search_pattern, &file_data);

    if (hFind == INVALID_HANDLE_VALUE) {
        WriteToJournal("Не удалось получить доступ к директории для мониторинга", TRUE);
        return;
    }

    BOOL has_copied_files = FALSE;

    do {
        if (!(file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            char source_path[MAX_PATH], destination_path[MAX_PATH];

            snprintf(source_path, sizeof(source_path), "%s\\%s",
                config.monitor_directory, file_data.cFileName);
            snprintf(destination_path, sizeof(destination_path), "%s\\%s",
                config.backup_directory, file_data.cFileName);

            if (CopyFile(source_path, destination_path, FALSE)) {
                char copy_message[512];
                snprintf(copy_message, sizeof(copy_message),
                    "Успех! Сервис %s создал резервную копию файла %s",
                    service_title, file_data.cFileName);
                WriteToJournal(copy_message, FALSE);
                has_copied_files = TRUE;
            }
            else {
                char error_message[512];
                snprintf(error_message, sizeof(error_message),
                    "Не удалось скопировать файл %s", file_data.cFileName);
                WriteToJournal(error_message, TRUE);
            }
        }
    } while (FindNextFile(hFind, &file_data));

    FindClose(hFind);

    if (has_copied_files) {
        WriteToJournal("Успех! Сервис DemoService успешно создал резервную копию файлов!", FALSE);
    }
}

void EnsureDirectoryExists(const char* directory_path, const char* dir_purpose) {
    char message[512];

    if (PathFileExists(directory_path)) {
        snprintf(message, sizeof(message),
            "Успех! %s обнаружил %s каталог %s",
            service_title, dir_purpose, directory_path);
    }
    else {
        if (_mkdir(directory_path) == 0) {
            snprintf(message, sizeof(message),
                "Успех! %s создал %s каталог %s",
                service_title, dir_purpose, directory_path);
        }
        else {
            snprintf(message, sizeof(message),
                "Не удалось создать %s каталог %s",
                dir_purpose, directory_path);
        }
    }

    WriteToJournal(message, strstr(message, "Не удалось") != NULL);
}