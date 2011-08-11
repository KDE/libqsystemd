include(${CMAKE_CURRENT_SOURCE_DIR}/annotate-systemd-interface.cmake)
include_directories(${CMAKE_CURRENT_BINARY_DIR}) # find generated interfaces
include_directories(${CMAKE_CURRENT_SOURCE_DIR}) # for <libqsystemd_export.h>

set(qsystemd_SRCS
	dbus-interface-types.cpp
	manager.cpp
	unit.cpp
)
#qsystemd_add_dbus_interface(qsystemd_SRCS ${DBUS_PREFIX}/interfaces/org.freedesktop.systemd1.Automount.xml systemd_automount_interface)
#qsystemd_add_dbus_interface(qsystemd_SRCS ${DBUS_PREFIX}/interfaces/org.freedesktop.systemd1.Device.xml    systemd_device_interface)
#qsystemd_add_dbus_interface(qsystemd_SRCS ${DBUS_PREFIX}/interfaces/org.freedesktop.systemd1.Job.xml       systemd_job_interface)
qsystemd_add_dbus_interface(qsystemd_SRCS ${DBUS_PREFIX}/interfaces/org.freedesktop.systemd1.Manager.xml   systemd_manager_interface)
#qsystemd_add_dbus_interface(qsystemd_SRCS ${DBUS_PREFIX}/interfaces/org.freedesktop.systemd1.Mount.xml     systemd_mount_interface)
#qsystemd_add_dbus_interface(qsystemd_SRCS ${DBUS_PREFIX}/interfaces/org.freedesktop.systemd1.Path.xml      systemd_path_interface)
#qsystemd_add_dbus_interface(qsystemd_SRCS ${DBUS_PREFIX}/interfaces/org.freedesktop.systemd1.Service.xml   systemd_service_interface)
#qsystemd_add_dbus_interface(qsystemd_SRCS ${DBUS_PREFIX}/interfaces/org.freedesktop.systemd1.Snapshot.xml  systemd_snapshot_interface)
#qsystemd_add_dbus_interface(qsystemd_SRCS ${DBUS_PREFIX}/interfaces/org.freedesktop.systemd1.Socket.xml    systemd_socket_interface)
#qsystemd_add_dbus_interface(qsystemd_SRCS ${DBUS_PREFIX}/interfaces/org.freedesktop.systemd1.Swap.xml      systemd_swap_interface)
#qsystemd_add_dbus_interface(qsystemd_SRCS ${DBUS_PREFIX}/interfaces/org.freedesktop.systemd1.Target.xml    systemd_target_interface)
#qsystemd_add_dbus_interface(qsystemd_SRCS ${DBUS_PREFIX}/interfaces/org.freedesktop.systemd1.Timer.xml     systemd_timer_interface)
qsystemd_add_dbus_interface(qsystemd_SRCS ${DBUS_PREFIX}/interfaces/org.freedesktop.systemd1.Unit.xml      systemd_unit_interface)

add_library(qsystemd SHARED ${qsystemd_SRCS})
target_link_libraries(qsystemd ${QT_LIBRARIES})
target_link_libraries(qsystemd LINK_INTERFACE_LIBRARIES ${QT_QTCORE_LIBRARY})

set_target_properties(qsystemd PROPERTIES
	VERSION ${QSYSTEMD_VERSION} SOVERSION ${QSYSTEMD_SOVERSION}
	DEFINE_SYMBOL MAKE_QSYSTEMD_LIB
)

#################### install ####################

install(TARGETS qsystemd EXPORT QSystemdLibraryDepends DESTINATION lib${LIB_SUFFIX})

install(FILES
	libqsystemd_export.h
	manager.h
	unit.h
DESTINATION include/qsystemd)

# the following adapted from http://www.vtk.org/Wiki/CMake/Tutorials/How_to_create_a_ProjectConfig.cmake_file

# add all targets to the build-tree export set
export(TARGETS qsystemd FILE ${PROJECT_BINARY_DIR}/QSystemdLibraryDepends.cmake)
# export the package for use from the build tree (this registers the build-tree
# with a global CMake registry)
export(PACKAGE QSystemd)

# create a QSystemdConfig.cmake file for use from the build tree
set(QSYSTEMD_INCLUDE_DIRS "${BUILD_TREE_INCLUDE_DIRS}")
set(QSYSTEMD_LIB_DIR "${PROJECT_BINARY_DIR}/qsystemd")
set(QSYSTEMD_CMAKE_DIR "${PROJECT_BINARY_DIR}")
configure_file(QSystemdConfig.cmake.in "${PROJECT_BINARY_DIR}/QSystemdConfig.cmake" @ONLY)
configure_file(QSystemdConfigVersion.cmake.in "${PROJECT_BINARY_DIR}/QSystemdConfigVersion.cmake" @ONLY)

# install the export set for use with the installed tree
install(EXPORT QSystemdLibraryDepends DESTINATION share/qsystemd/cmake COMPONENT devel)

# install a QSystemdConfig.cmake file for use from the installed tree
set(QSYSTEMD_INCLUDE_DIRS ${CMAKE_INSTALL_PREFIX}/include)
set(QSYSTEMD_LIB_DIR ${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX})
set(QSYSTEMD_CMAKE_DIR ${CMAKE_INSTALL_PREFIX}/share/qsystemd/cmake)
configure_file(QSystemdConfig.cmake.in "${PROJECT_BINARY_DIR}/InstallFiles/QSystemdConfig.cmake" @ONLY)
configure_file(QSystemdConfigVersion.cmake.in "${PROJECT_BINARY_DIR}/InstallFiles/QSystemdConfigVersion.cmake" @ONLY)
install(FILES
	"${PROJECT_BINARY_DIR}/InstallFiles/QSystemdConfig.cmake"
	"${PROJECT_BINARY_DIR}/InstallFiles/QSystemdConfigVersion.cmake"
DESTINATION ${QSYSTEMD_CMAKE_DIR})
