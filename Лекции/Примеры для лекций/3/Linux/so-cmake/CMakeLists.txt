cmake_minimum_required(VERSION 3.20)

project(SOEx)
add_subdirectory(MainProject)
add_subdirectory(LibProject)
SET_TARGET_PROPERTIES(LibProject PROPERTIES
   LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin
)
SET_TARGET_PROPERTIES(MainProject PROPERTIES
   RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin
)
