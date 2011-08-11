/***************************************************************************
 *   Copyright 2011 Stefan Majewsky <majewsky@gmx.net>                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License          *
 *   version 2 as published by the Free Software Foundation                *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU Library General Public License for more details.                  *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/

#ifndef LIBQSYSTEMD_EXPORT_H
#define LIBQSYSTEMD_EXPORT_H

#include <kdemacros.h>

//export macro for libqsystemd

#ifndef QSYSTEMD_EXPORT
#	if defined(MAKE_QSYSTEMD_LIB)
#		define QSYSTEMD_EXPORT Q_DECL_EXPORT //building the library
#	else
#		define QSYSTEMD_EXPORT Q_DECL_IMPORT //using the library
#	endif
#endif

#endif // LIBQSYSTEMD_EXPORT_H
