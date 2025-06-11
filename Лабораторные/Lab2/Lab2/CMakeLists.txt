# CMakeList.txt: файл проекта CMake верхнего уровня; проведите глобальную конфигурацию
# и включите сюда подпроекты.
#
cmake_minimum_required (VERSION 3.8)

# Включение горячей перезагрузки для компиляторов MSVC, если поддерживается.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("Lab2")
set(EXECUTABLE_OUTPUT_PATH "./exec/")


if(WIN32)
	message("System: Windows")
	add_subdirectory ("Lab-2a")
	add_subdirectory("Lab-2b")
	add_subdirectory("Lab-2c")
elseif(UNIX)
	message("System: Linux")
	add_subdirectory("Lab-2d")
	add_subdirectory("Lab-2e")
endif()
