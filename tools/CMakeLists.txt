find_package(KDE4) # optional
if (KDE4_FOUND)
	add_subdirectory(plasma)
endif (KDE4_FOUND)
