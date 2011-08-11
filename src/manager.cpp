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

#include "manager.h"
#include "unit.h"
#include "unit_p.h"

#include "dbus-interface-types.h"
#include "systemd_manager_interface.h"

struct QsdManager::Private
{
	Private(const QsdDBusRef& ref);

	QsdFromDBusRef<org::freedesktop::systemd1::Manager> m_interface;
	QList<QsdUnit*> m_units;
};

QsdManager::QsdManager(Private* d)
	: d(d)
{
}

QsdManager::Private::Private(const QsdDBusRef& ref)
	: m_interface(ref)
{
	QsdPrivate::registerMetaTypes();
	//list units
	const QList<QsdPrivate::UnitListEntry> entries = m_interface.ListUnits();
	foreach (const QsdPrivate::UnitListEntry& ule, entries)
	{
		m_units << new QsdUnit(new QsdUnit::Private(ule, m_interface.ref()));
	}
}

QsdManager::~QsdManager()
{
	delete d;
}

/*static*/ QsdManager* QsdManager::instance()
{
	//TODO: thread-safe implementation
	static QsdManager instance(new QsdManager::Private(QsdDBusRef(
		QDBusConnection::systemBus(),
		"org.freedesktop.systemd1",
		"/org/freedesktop/systemd1"
	)));
	return &instance;
}

QList<QsdUnit*> QsdManager::units() const
{
	return d->m_units;
}
