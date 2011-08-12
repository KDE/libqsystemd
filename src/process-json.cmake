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

# This CMake macro processes JSON-encoded data with the accompanied scripts
# into pairs of h/cpp files which are automatically added to the build.
# The signature is: qsystemd_process_json(SRCVARIABLE basename1 basename2 ...)

macro(qsystemd_process_json _srcvar)
	foreach(_basename ${ARGN})
		set(PY_FILE ${CMAKE_CURRENT_SOURCE_DIR}/${_basename}.py)
		set(JSON_FILE ${CMAKE_CURRENT_SOURCE_DIR}/${_basename}.json)
		set(OUT_BASE ${CMAKE_CURRENT_BINARY_DIR}/${_basename})
		add_custom_command(OUTPUT ${OUT_BASE}.h ${OUT_BASE}.cpp
			COMMAND ${PY_FILE} ${JSON_FILE} ${OUT_BASE}
			MAIN_DEPENDENCY ${JSON_FILE} DEPENDS ${PY_FILE}
			COMMENT "Processing ${_basename}.json"
		)
		set (${_srcvar} ${${_srcvar}} "${OUT_BASE}.cpp")
	endforeach(_basename)
endmacro(qsystemd_process_json)
