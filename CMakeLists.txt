cmake_minimum_required(VERSION 3.25)
project(bandeira C)

set(CMAKE_C_STANDARD 11)
include_directories(C:/MinGW/include)
link_directories(C:/MinGW/lib)

add_executable(bandeira main.c linha.h linha.c ponto.c ponto.h state.h poligono.c poligono.h interface.c transformacoes.c)

target_link_libraries(bandeira opengl32 glu32 freeglut)