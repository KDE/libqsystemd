project(libqsystemd)
cmake_minimum_required(VERSION 2.6)

set(QSYSTEMD_MAJOR_VERSION 0)
set(QSYSTEMD_MINOR_VERSION 1)
set(QSYSTEMD_PATCH_VERSION 0)
set(QSYSTEMD_SOVERSION 0)
set(QSYSTEMD_VERSION ${QSYSTEMD_MAJOR_VERSION}.${QSYSTEMD_MINOR_VERSION}.${QSYSTEMD_PATCH_VERSION})

find_package(Qt4 COMPONENTS QtCore QtDBus REQUIRED)
include(${QT_USE_FILE})

add_subdirectory(src)
add_subdirectory(tools)
