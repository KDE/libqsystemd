# find uninstalled libqsystemd headers
include_directories(${PROJECT_SOURCE_DIR}/src ${PROJECT_BINARY_DIR}/src)

find_package(KDE4) # optional

if (KDE4_FOUND)
	add_subdirectory(plasma)
endif (KDE4_FOUND)
add_subdirectory(qsystemdgui)
