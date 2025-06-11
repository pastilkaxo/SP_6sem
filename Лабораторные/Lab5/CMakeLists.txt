cmake_minimum_required(VERSION 3.10)
project(ServiceLab VERSION 1.0.0 LANGUAGES C)

# Установка компиляторов
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

# Установка общего каталога для всех выходных файлов
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

# Настройка генераторов для разных ОС
if(WIN32)
    # Для Windows используем Visual Studio
    set(CMAKE_GENERATOR "Visual Studio 17 2022")
elseif(UNIX AND NOT APPLE)
    # Для Linux используем Unix Makefiles
    set(CMAKE_GENERATOR "Unix Makefiles")
elseif(APPLE)
    # Для macOS используем XCode
    set(CMAKE_GENERATOR "Xcode")
endif()

# Настройка параметров компиляции
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")

# Компоненты проекта для Windows
if(WIN32)
    # Windows-сервис
    add_executable(DemoService DemoService.c)
    target_link_libraries(DemoService shlwapi advapi32)
    
    # Утилита управления сервисом
    add_executable(ServiceCtl ServiceCtl.c)
    target_link_libraries(ServiceCtl advapi32)
    
    # Копирование конфигурационного файла
    configure_file(config.ini ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/config.ini COPYONLY)
endif()

# Компоненты проекта для Linux
if(UNIX AND NOT APPLE)
    # Добавляем исходники Linux-демона
    add_executable(demoserviced Linux/demoserviced.c)
    
    # Проверка наличия библиотеки inih
    find_library(INIH_LIBRARY NAMES ini inih)
    if(NOT INIH_LIBRARY)
        message(FATAL_ERROR "libinih не найдена. Установите libinih-dev (sudo apt-get install libinih-dev)")
    endif()
    
    # Линковка с библиотеками
    target_link_libraries(demoserviced ${INIH_LIBRARY})
    
    # Копирование файла юнита systemd
    configure_file(Linux/demoserviced.service ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/demoserviced.service COPYONLY)
endif()

# Компоненты проекта для macOS
if(APPLE)
    # macOS-демон (используется тот же исходник, что и для Linux)
    add_executable(demoserviced Linux/demoserviced.c)
    
    # macOS-специфичные настройки (если необходимы)
    target_compile_definitions(demoserviced PRIVATE MACOS=1)
    
    # Линковка с необходимыми библиотеками
    find_library(INIH_LIBRARY NAMES ini inih)
    if(INIH_LIBRARY)
        target_link_libraries(demoserviced ${INIH_LIBRARY})
    endif()
endif()

# Вывод конфигурации сборки
message(STATUS "Проект ${PROJECT_NAME} настроен для ${CMAKE_SYSTEM_NAME}")
message(STATUS "Используемый генератор: ${CMAKE_GENERATOR}")
message(STATUS "Компилятор C: ${CMAKE_C_COMPILER}")
message(STATUS "Флаги компиляции: ${CMAKE_C_FLAGS}")
message(STATUS "Выходной каталог: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")