cmake_minimum_required(VERSION 3.20)

include_directories(${CMAKE_SOURCE_DIR}/LibLvod/include) # �������� ��� ������ ������������ ������
add_library(LibLvod SHARED libhd.c) # �������� ���������� �� ��������� ������
