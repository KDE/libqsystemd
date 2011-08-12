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

# This CMake macro processes pairs of .h.in/.cpp.in files into .h/.cpp files
# which declare and define QsdUnit and its subclasses.
# The signature is: qsystemd_process_source_file(SRCVARIABLE basename1 basename2 ...)

macro(qsystemd_process_source_file _srcvar)
	set(PY_FILE ${CMAKE_CURRENT_SOURCE_DIR}/process-source-file.py)
	foreach(_basename ${ARGN})
		set(IN_BASE ${CMAKE_CURRENT_SOURCE_DIR}/${_basename})
		set(OUT_BASE ${CMAKE_CURRENT_BINARY_DIR}/${_basename})
		set(DEPENDS ${PY_FILE})
		if(EXISTS ${IN_BASE}.cpp.in)
			set(DEPENDS ${IN_BASE}.cpp.in ${DEPENDS})
		endif(EXISTS ${IN_BASE}.cpp.in)
		add_custom_command(OUTPUT ${OUT_BASE}.h ${OUT_BASE}_p.h ${OUT_BASE}.cpp
			COMMAND ${PY_FILE} ${IN_BASE} ${OUT_BASE}
			MAIN_DEPENDENCY ${IN_BASE}.h.in DEPENDS ${DEPENDS}
			COMMENT "Processing ${_basename}.{h,cpp}.in"
		)
		set (${_srcvar} ${${_srcvar}} "${OUT_BASE}.cpp")
	endforeach(_basename)
endmacro(qsystemd_process_source_file)
