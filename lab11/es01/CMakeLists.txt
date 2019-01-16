cmake_minimum_required(VERSION 3.12)
project(es01 C)

set(CMAKE_C_STANDARD 99)

add_executable(es01 main.c data.h classe_titoli.h collezione_titoli.h quotazione_giornaliera.h collezione_giornaliera.h quotazione_giornaliera.c collezione_giornaliera.c classe_titoli.c collezione_titoli.c utility.h data.c)