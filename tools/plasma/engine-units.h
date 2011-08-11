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

#ifndef QSYSTEMD_PLASMA_ENGINE_UNITS_H
#define QSYSTEMD_PLASMA_ENGINE_UNITS_H

class QsdUnit;

#include <Plasma/DataEngine>

class QsystemdUnitsDataEngine : public Plasma::DataEngine
{
	Q_OBJECT
	public:
		QsystemdUnitsDataEngine(QObject* parent, const QVariantList& args);
		virtual void init();
		//TODO: sourceRequestEvent, updateSourceEvent
};

#endif // QSYSTEMD_PLASMA_ENGINE_UNITS_H
