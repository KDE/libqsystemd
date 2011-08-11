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

#ifndef LIBQSYSTEMD_UNIT_P_H
#define LIBQSYSTEMD_UNIT_P_H

#include "dbusref_p.h"
#include "unit.h"

#include "dbus-interface-types_p.h"
#include "systemd_unit_interface.h"

namespace QsdPrivate
{
	struct ParsedUnitListEntry
	{
		ParsedUnitListEntry(const QsdPrivate::UnitListEntry& ule);

		QString id, description;
		QsdUnit::ActiveState activeState;
		QsdUnit::LoadState loadState;
		QsdUnit::Type type;
	};
}

struct QsdUnit::Private
{
	Private(const QsdPrivate::UnitListEntry& ule, const QsdDBusRef& siblingInterface);

	QsdFromDBusRef<org::freedesktop::systemd1::Unit> m_interface;
	QsdPrivate::ParsedUnitListEntry* m_pule;
};

#endif // LIBQSYSTEMD_UNIT_P_H
