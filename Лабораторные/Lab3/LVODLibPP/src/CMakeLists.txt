cmake_minimum_required(VERSION 3.20)

include_directories(${CMAKE_SOURCE_DIR}/LVODLibPP/include) # �������� ��� ������ ������������ ������
add_library(LVODLibPP SHARED LVOCPP.cpp) # �������� ���������� �� ��������� ������
