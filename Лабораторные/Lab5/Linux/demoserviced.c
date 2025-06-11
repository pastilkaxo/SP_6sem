#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <fcntl.h>
#include <errno.h>
#include <ini.h>

#define CONFIG_FILE "/etc/demoserviced/demoserviced.conf" 
#define DEFAULT_SOURCE_DIR "/srv/demoserviced/source" 
#define DEFAULT_RESERVED_DIR "/var/lib/demoserviced/reserved" 
#define DEFAULT_LOG_DIR "/var/log/demoserviced" 
#define DEFAULT_INTERVAL 60 
#define MAX_PATH 256
#define PID_FILE "/run/demoserviced.pid" 

static volatile sig_atomic_t running = 1;
static volatile sig_atomic_t reload_config = 0;
static char service_name[] = "demoserviced";
static FILE* log_file = NULL;
static char log_path[MAX_PATH];
static char source_dir[MAX_PATH];
static char reserved_dir[MAX_PATH];
static int copy_interval;

void LogMessage(const char* message, int is_error);
void ReadConfig();
void CopyFiles();
void CreateOrCheckDir(const char* path, const char* dir_type);
void SignalHandler(int sig);
int IniHandler(void* user, const char* section, const char* name, const char* value);

int main() {
 
    pid_t pid = fork();
    if (pid < 0) {ws
        fprintf(stderr, "Fork failed: %s\n", strerror(errno)); 
        exit(1);
    }
    if (pid > 0) {
        exit(0); 
    }
    umask(0); 
    setsid(); 
    chdir("/");
    close(STDIN_FILENO); 
    close(STDOUT_FILENO);
    close(STDERR_FILENO); 

    FILE* pid_fp = fopen(PID_FILE, "w");
    if (pid_fp) {
        fprintf(pid_fp, "%d", getpid());
        fclose(pid_fp);
    } else {
        
    }


    signal(SIGTERM, SignalHandler);
    signal(SIGHUP, SignalHandler);

   
    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    snprintf(log_path, MAX_PATH, "%s/%04d%02d%02d%02d%02d%02d-service.log",
        DEFAULT_LOG_DIR, t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
        t->tm_hour, t->tm_min, t->tm_sec);

 
    CreateOrCheckDir(DEFAULT_LOG_DIR, "log"); 
    CreateOrCheckDir(DEFAULT_RESERVED_DIR, "reserved"); 

    ReadConfig();

    
    if (access(source_dir, F_OK) != 0) {
        LogMessage("Source directory does not exist", 1); 
        exit(1);
    }

 
    char params[512];
    snprintf(params, sizeof(params), "source=%s, reserved=%s, interval=%d min",
        source_dir, reserved_dir, copy_interval);
    char msg[512];
    snprintf(msg, sizeof(msg), "Success! Service %s started with parameters %s", 
        service_name, params);
    LogMessage(msg, 0);

    while (running) {
        if (reload_config) {
            ReadConfig();
            LogMessage("Configuration reloaded", 0);
            reload_config = 0;
        }
        CopyFiles();
        sleep(copy_interval * 60);
    }

    LogMessage("Success! Service demoserviced changed state from RUNNING to STOPPED", 0);
    if (log_file) {
        fclose(log_file);
    }
    remove(PID_FILE);
    exit(0);
}

void SignalHandler(int sig) {
    if (sig == SIGTERM) {
        running = 0;
        LogMessage("Success! Service was stopped by SIGTERM", 0);
    }
    else if (sig == SIGHUP) {
        reload_config = 1;
        LogMessage("Success! Service was reloaded by SIGHUP", 0);

    }
}

void LogMessage(const char* message, int is_error) {
    if (!log_file) {
        log_file = fopen(log_path, "a");
        if (!log_file) {
            fprintf(stderr, "Failed to open log file %s: %s\n", log_path, strerror(errno)); 
            return;
        }
    }
    char timestamp[32];
    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    snprintf(timestamp, sizeof(timestamp), "[%02d:%02d:%02d] ", t->tm_hour, t->tm_min, t->tm_sec);
    char full_msg[1024];
    snprintf(full_msg, sizeof(full_msg), "%s%s%s\n", timestamp,
        is_error ? "Operation failed! " : "", message);
    fprintf(log_file, "%s", full_msg);
    fflush(log_file);
}

int IniHandler(void* user, const char* section, const char* name, const char* value) {
    if (strcmp(section, "Settings") == 0) {
        if (strcmp(name, "SourceDir") == 0) {
            strncpy(source_dir, value, MAX_PATH - 1);
            source_dir[MAX_PATH - 1] = '\0';
        }
        else if (strcmp(name, "ReservedDir") == 0) {
            strncpy(reserved_dir, value, MAX_PATH - 1);
            reserved_dir[MAX_PATH - 1] = '\0';
        }
        else if (strcmp(name, "Interval") == 0) {
            copy_interval = atoi(value);
        }
    }
    return 1;
}

void ReadConfig() {
   
    strncpy(source_dir, DEFAULT_SOURCE_DIR, MAX_PATH); 
    strncpy(reserved_dir, DEFAULT_RESERVED_DIR, MAX_PATH);
    copy_interval = DEFAULT_INTERVAL;


    if (ini_parse(CONFIG_FILE, IniHandler, NULL) < 0) {
        LogMessage("Failed to load configuration file", 1); 
    }
}

void CopyFiles() {
    DIR* dir = opendir(source_dir);
    if (!dir) {
        LogMessage("Failed to access source directory", 1); 
        return;
    }

    struct dirent* entry;
    while ((entry = readdir(dir))) {
        if (entry->d_type != DT_REG) {
            continue; 
        }
        char src_file[MAX_PATH], dst_file[MAX_PATH];
        snprintf(src_file, sizeof(src_file), "%s/%s", source_dir, entry->d_name);
        snprintf(dst_file, sizeof(dst_file), "%s/%s", reserved_dir, entry->d_name);

        int src_fd = open(src_file, O_RDONLY);
        if (src_fd < 0) {
            char msg[512];
            snprintf(msg, sizeof(msg), "Operation failed! Failed to open source file %s", entry->d_name); 
            LogMessage(msg, 1);
            continue;
        }

        int dst_fd = open(dst_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (dst_fd < 0) {
            char msg[512];
            snprintf(msg, sizeof(msg), "Operation failed! Failed to open destination file %s", entry->d_name); 
            LogMessage(msg, 1);
            close(src_fd);
            continue;
        }

        char buffer[4096];
        ssize_t bytes_read;
        while ((bytes_read = read(src_fd, buffer, sizeof(buffer))) > 0) {
            if (write(dst_fd, buffer, bytes_read) != bytes_read) {
                char msg[512];
                snprintf(msg, sizeof(msg), "Operation failed! Failed to write to file %s", entry->d_name); 
                LogMessage(msg, 1);
                close(src_fd);
                close(dst_fd);
                break;
            }
        }
        if (bytes_read < 0) {
            char msg[512];
            snprintf(msg, sizeof(msg), "Operation failed! Failed to read from file %s", entry->d_name); 
            LogMessage(msg, 1);
        }
        else {
            char msg[512];
            snprintf(msg, sizeof(msg), "Success! Service %s created backup of file %s", service_name, entry->d_name); 
            LogMessage(msg, 0);
        }
        close(src_fd);
        close(dst_fd);
    }
    closedir(dir);
    LogMessage("Success! Service demoserviced completed backup operation", 0); 
}

void CreateOrCheckDir(const char* path, const char* dir_type) {
    char msg[512];
    if (access(path, F_OK) == 0) {
        snprintf(msg, sizeof(msg), "Success! Service %s found %s directory %s", service_name, dir_type, path); 
    }
    else {
        if (mkdir(path, 0755) == 0) { 
            snprintf(msg, sizeof(msg), "Success! Service %s created %s directory %s", service_name, dir_type, path); 
        }
        else {
            snprintf(msg, sizeof(msg), "Operation failed! Failed to create %s directory %s", dir_type, path); 
            LogMessage(msg, 1);
            return;
        }
    }
    LogMessage(msg, 0);
}