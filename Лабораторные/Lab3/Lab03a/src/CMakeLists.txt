cmake_minimum_required(VERSION 3.20)

include_directories(${CMAKE_SOURCE_DIR}/LVOLib/include) # �������� ��� ������ ������������ ������
add_executable(Lab03a client.c)
target_link_libraries(Lab03a LVOLib)