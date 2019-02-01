cmake_minimum_required(VERSION 3.12)
project(esame C)

set(CMAKE_C_STANDARD 99)

add_executable(esame main.c graph.c st.h graph.h utility.h utility.c st.c)