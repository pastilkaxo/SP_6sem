cmake_minimum_required(VERSION 3.20)

include_directories(${CMAKE_SOURCE_DIR}/LVODLib/include) # �������� ��� ������ ������������ ������
add_library(LVODLib SHARED LVOD.c) # �������� ���������� �� ��������� ������
target_link_options(LVODLib PRIVATE "/DEF:${CMAKE_SOURCE_DIR}/LVODLib/src/LVODLib.def")