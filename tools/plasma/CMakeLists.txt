include(KDE4Defaults)
add_definitions(${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include_directories(${KDE4_INCLUDES})

kde4_add_plugin(plasma_engine_systemd_units engine-units.cpp)
target_link_libraries(plasma_engine_systemd_units ${KDE4_KDECORE_LIBS} ${KDE4_PLASMA_LIBS} qsystemd)

install(TARGETS plasma_engine_systemd_units DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES plasma-engine-systemd-units.desktop DESTINATION ${SERVICES_INSTALL_DIR})
