cmake_minimum_required(VERSION 3.10) 
project(ServiceLab)
set(CMAKE_C_COMPILER clang) 
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

if(WIN32) 
	add_executable(DemoService DemoService.c) 
	target_link_libraries(DemoService shlwapi) 
	add_executable(ServiceCtl ServiceCtl.c) 
	set_target_properties(DemoService ServiceCtl PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin" ) 
endif()

if(UNIX AND NOT APPLE) 
	find_library(INIH_LIBRARY NAMES ini inih) 
	if(NOT INIH_LIBRARY) 
		message(FATAL_ERROR "libinih not found. Please install libinih-dev.") 
	endif()
	add_executable(demoserviced demoserviced.c)
	target_link_libraries(demoserviced ${INIH_LIBRARY})
	set_target_properties(demoserviced PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
	)
endif()

if(WIN32) 
	set(CMAKE_GENERATOR "Visual Studio 16 2019") 
endif()