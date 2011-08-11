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

#include "unit.h"
#include "unit_p.h"

QsdUnit::ActiveState activeStateFromString(const QString& str)
{
	static const QVector<QString> activeStateStrings = QVector<QString>()
		<< QLatin1String("active") << QLatin1String("reloading")
		<< QLatin1String("inactive") << QLatin1String("failed")
		<< QLatin1String("activating") << QLatin1String("deactivating");
	const int activeState = activeStateStrings.indexOf(str.toLower());
	return static_cast<QsdUnit::ActiveState>(activeState);
}

QsdUnit::LoadState loadStateFromString(const QString& str)
{
	static const QVector<QString> loadStateStrings = QVector<QString>()
		<< QLatin1String("stub") << QLatin1String("loaded")
		<< QLatin1String("error") << QLatin1String("merged")
		<< QLatin1String("masked");
	const int loadState = loadStateStrings.indexOf(str.toLower());
	return static_cast<QsdUnit::LoadState>(loadState);
}

static QsdUnit::Type typeFromUnitName(const QString& id)
{
	static const QVector<QString> typeSuffixes = QVector<QString>()
		<< QLatin1String(".service") << QLatin1String(".socket")
		<< QLatin1String(".target") << QLatin1String(".device")
		<< QLatin1String(".mount") << QLatin1String(".automount")
		<< QLatin1String(".snapshot") << QLatin1String(".timer")
		<< QLatin1String(".swap") << QLatin1String(".path");
	for (int i = 0; i < typeSuffixes.size(); ++i)
	{
		if (id.endsWith(typeSuffixes[i]))
		{
			return static_cast<QsdUnit::Type>(i);
		}
	}
	return QsdUnitType::Invalid;
}

QsdPrivate::ParsedUnitListEntry::ParsedUnitListEntry(const QsdPrivate::UnitListEntry& ule)
	: id(ule.id)
	, description(ule.description)
	, activeState(activeStateFromString(ule.active_state))
	, loadState(loadStateFromString(ule.load_state))
	, type(typeFromUnitName(ule.id))
{
}

QsdUnit::Private::Private(const QsdPrivate::UnitListEntry& ule)
	: m_interface("org.freedesktop.systemd1", ule.unit_path.path(), QDBusConnection::systemBus())
	, m_pule(new QsdPrivate::ParsedUnitListEntry(ule))
{
}

QsdUnit::QsdUnit(QsdUnit::Private* d)
	: d(d)
{
}

QsdUnit::~QsdUnit()
{
	delete d->m_pule;
	delete d;
}

QString QsdUnit::id() const
{
	if (d->m_pule)
	{
		return d->m_pule->id;
	}
	return d->m_interface.id();
}

QString QsdUnit::description() const
{
	if (d->m_pule)
	{
		return d->m_pule->description;
	}
	return d->m_interface.description();
}

QsdUnit::Type QsdUnit::type() const
{
	if (d->m_pule)
	{
		return d->m_pule->type;
	}
	return typeFromUnitName(d->m_interface.id());
}

QsdUnit::ActiveState QsdUnit::activeState() const
{
	if (d->m_pule)
	{
		return d->m_pule->activeState;
	}
	return activeStateFromString(d->m_interface.activeState());
}

QsdUnit::LoadState QsdUnit::loadState() const
{
	if (d->m_pule)
	{
		return d->m_pule->loadState;
	}
	return loadStateFromString(d->m_interface.loadState());
}
