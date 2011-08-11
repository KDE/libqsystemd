project(example-list-unit-ids)
cmake_minimum_required(VERSION 2.6)

find_package(qsystemd REQUIRED)
include_directories(${QSYSTEMD_INCLUDE_DIRS})
link_directories(${QSYSTEMD_LIBRARY_DIRS})

add_executable(list-unit-ids list-unit-ids.cpp)
target_link_libraries(list-unit-ids ${QSYSTEMD_LIBRARIES})
install(TARGETS list-unit-ids DESTINATION bin)
