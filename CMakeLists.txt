project(libqsystemd)
cmake_minimum_required(VERSION 2.6)

find_package(Qt4 COMPONENTS QtCore QtDBus REQUIRED)
include(${QT_USE_FILE})
include(${CMAKE_CURRENT_SOURCE_DIR}/annotate-systemd-interface.cmake)
include_directories(${CMAKE_CURRENT_BINARY_DIR}) # find generated interfaces

set(libqsystemd_SRCS
	dbus-interface-types.cpp
	manager.cpp
	main.cpp
	unit.cpp
)
#libqsystemd_add_dbus_interface(libqsystemd_SRCS ${DBUS_PREFIX}/interfaces/org.freedesktop.systemd1.Automount.xml systemd_automount_interface)
#libqsystemd_add_dbus_interface(libqsystemd_SRCS ${DBUS_PREFIX}/interfaces/org.freedesktop.systemd1.Device.xml    systemd_device_interface)
#libqsystemd_add_dbus_interface(libqsystemd_SRCS ${DBUS_PREFIX}/interfaces/org.freedesktop.systemd1.Job.xml       systemd_job_interface)
libqsystemd_add_dbus_interface(libqsystemd_SRCS ${DBUS_PREFIX}/interfaces/org.freedesktop.systemd1.Manager.xml   systemd_manager_interface)
#libqsystemd_add_dbus_interface(libqsystemd_SRCS ${DBUS_PREFIX}/interfaces/org.freedesktop.systemd1.Mount.xml     systemd_mount_interface)
#libqsystemd_add_dbus_interface(libqsystemd_SRCS ${DBUS_PREFIX}/interfaces/org.freedesktop.systemd1.Path.xml      systemd_path_interface)
#libqsystemd_add_dbus_interface(libqsystemd_SRCS ${DBUS_PREFIX}/interfaces/org.freedesktop.systemd1.Service.xml   systemd_service_interface)
#libqsystemd_add_dbus_interface(libqsystemd_SRCS ${DBUS_PREFIX}/interfaces/org.freedesktop.systemd1.Snapshot.xml  systemd_snapshot_interface)
#libqsystemd_add_dbus_interface(libqsystemd_SRCS ${DBUS_PREFIX}/interfaces/org.freedesktop.systemd1.Socket.xml    systemd_socket_interface)
#libqsystemd_add_dbus_interface(libqsystemd_SRCS ${DBUS_PREFIX}/interfaces/org.freedesktop.systemd1.Swap.xml      systemd_swap_interface)
#libqsystemd_add_dbus_interface(libqsystemd_SRCS ${DBUS_PREFIX}/interfaces/org.freedesktop.systemd1.Target.xml    systemd_target_interface)
#libqsystemd_add_dbus_interface(libqsystemd_SRCS ${DBUS_PREFIX}/interfaces/org.freedesktop.systemd1.Timer.xml     systemd_timer_interface)
libqsystemd_add_dbus_interface(libqsystemd_SRCS ${DBUS_PREFIX}/interfaces/org.freedesktop.systemd1.Unit.xml      systemd_unit_interface)

# TODO: library
add_executable(libqsystemd ${libqsystemd_SRCS})
target_link_libraries(libqsystemd ${QT_LIBRARIES})
