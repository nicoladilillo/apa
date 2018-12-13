cmake_minimum_required(VERSION 3.12)
project(es03 C)

set(CMAKE_C_STANDARD 99)

add_executable(es03 main.c pg.h inv.h pg.c inv.c pgList.h invArray.h equipArray.h utility.h utility.c pgList.c equipArray.c invArray.c)