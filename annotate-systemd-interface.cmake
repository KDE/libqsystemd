# Copyright 2011 Stefan Majewsky <majewsky@gmx.net>
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public
# License as published by the Free Software Foundation; either
# version 2 of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.

# This CMake macro works like qt4_add_dbus_interface, but uses
# annotate-systemd-interface.py to add type annotations, which are needed by
# qdbusxml2cpp to support non-trivial DBus data types.

set(DBUS_PREFIX /usr/share/dbus-1) # TODO: determine dynamically

macro(libqsystemd_add_dbus_interface _sources  _infilepre _basename)
	get_filename_component(_infile ${_infilepre} ABSOLUTE)
	# add type annotations
	get_filename_component(_FILENAME ${_infile} NAME)
	set(_OUTFILE ${CMAKE_CURRENT_BINARY_DIR}/annotated--${_FILENAME})
	add_custom_command(OUTPUT ${_OUTFILE}
		COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/annotate-systemd-interface.py ${_infile} ${_OUTFILE}
		MAIN_DEPENDENCY ${_infile}
		DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/annotate-systemd-interface.py
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		COMMENT "Adding annotations on ${_FILENAME}"
	)
	# The following part is adapted from qt4_add_dbus_interface as found in
	# Qt4Macros.cmake shipped with CMake 2.8.3. qt4_add_dbus_interface cannot
	# be used directly because it lacks support for qdbusxml2cpp's "-i" option.
	set(_HEADER ${CMAKE_CURRENT_BINARY_DIR}/${_basename}.h)
	set(_IMPL   ${CMAKE_CURRENT_BINARY_DIR}/${_basename}.cpp)
	set(_MOC    ${CMAKE_CURRENT_BINARY_DIR}/${_basename}.moc)
	set(_PARAMS -m -i ${CMAKE_CURRENT_SOURCE_DIR}/dbus-interface-types.h)
	add_custom_command(OUTPUT ${_IMPL} ${_HEADER}
		COMMAND ${QT_DBUSXML2CPP_EXECUTABLE} ${_PARAMS} -p ${_basename} ${_OUTFILE}
		DEPENDS ${_OUTFILE}
	)
	set_source_files_properties(${_IMPL} PROPERTIES SKIP_AUTOMOC TRUE)
	qt4_generate_moc(${_HEADER} ${_MOC})
	set(${_sources} ${${_sources}} ${_IMPL} ${_HEADER} ${_MOC})
	macro_add_file_dependencies(${_IMPL} ${_MOC})
endmacro(libqsystemd_add_dbus_interface)
