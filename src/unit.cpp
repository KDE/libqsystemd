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

static QsdUnit::Type typeFromUnitName(const QString& id)
{
	return QsdUnitType::fromString(id.section(QChar('.'), -1));
}

QsdPrivate::ParsedUnitListEntry::ParsedUnitListEntry(const QsdPrivate::UnitListEntry& ule)
	: id(ule.id)
	, description(ule.description)
	, activeState(QsdActiveState::fromString(ule.active_state))
	, loadState(QsdLoadState::fromString(ule.load_state))
	, type(typeFromUnitName(ule.id))
{
}

QsdUnit::Private::Private(const QsdPrivate::UnitListEntry& ule, const QsdDBusRef& siblingInterface)
	: m_interface(siblingInterface.siblingPath(ule.unit_path))
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
	return QsdActiveState::fromString(d->m_interface.activeState());
}

QsdUnit::LoadState QsdUnit::loadState() const
{
	if (d->m_pule)
	{
		return d->m_pule->loadState;
	}
	return QsdLoadState::fromString(d->m_interface.loadState());
}
