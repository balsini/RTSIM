cmake_minimum_required(VERSION 2.8)
project(RTSIM)

include(ExternalProject)

ExternalProject_Add (
    METASIM
    PREFIX METASIM
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/METASIM
    INSTALL_COMMAND ""
)

ExternalProject_Add (
    RTLIB
    DEPENDS METASIM
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/RTLIB
    INSTALL_COMMAND ""
)
