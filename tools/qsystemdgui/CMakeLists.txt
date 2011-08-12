set(qsystemdgui_SRCS
	main.cpp
)

add_executable(qsystemdgui ${qsystemdgui_SRCS})
target_link_libraries(qsystemdgui ${QT_LIBRARIES} ${QT_QTGUI_LIBRARY} qsystemd)

install(TARGETS qsystemdgui DESTINATION bin)
