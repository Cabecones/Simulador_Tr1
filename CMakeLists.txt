cmake_minimum_required(VERSION 3.24)
project(Simulador)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Simulador
        CamadaFisica.cpp
        CamadaFisica.hpp
        Simulador.cpp)
