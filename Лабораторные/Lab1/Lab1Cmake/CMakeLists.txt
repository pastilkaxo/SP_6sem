# CMakeList.txt: файл проекта CMake верхнего уровня; проведите глобальную конфигурацию
# и включите сюда подпроекты.
#
cmake_minimum_required (VERSION 3.8)

# Включение горячей перезагрузки для компиляторов MSVC, если поддерживается.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  # уст. знач. переменной
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project (Lab1Cmake) # название проекта
set(EXECUTABLE_OUTPUT_PATH  "./exec/") #куда сохраняется исполняемые файлы


if(WIN32)
	message("Hello, I am Windows!")
elseif(UNIX)
	message("Hello, I am Linux!")
elseif(APPLE)
	message("Hello, I am macOS!")
else()
	message("I am Tyler Derden")
endif()
	


# Включите подпроекты.
add_subdirectory (C-Project) #добавляет в сборку другой каталог

include(CheckLanguage) # + модули
check_language(CSharp) # доступность комп-ра


if(CMAKE_CSharp_COMPILER)
	enable_language(CSharp) # вкл поддер-ки языка
	message(STATUS "Yes c# support")
	add_subdirectory(CSharp-Project) 
else()
	message(STATUS "No c# support")   # сообщ. в консоль
endif()