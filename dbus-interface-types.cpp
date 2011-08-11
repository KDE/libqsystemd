/***************************************************************************
 *   Copyright 2011 Stefan Majewsky <majewsky@gmx.net>
 *
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public
 *   License as published by the Free Software Foundation; either
 *   version 2 of the License, or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
***************************************************************************/

#include "dbus-interface-types.h"

#include <QtDBus/QDBusArgument>
#include <QtDBus/QDBusMetaType>

QDBusArgument& operator<<(QDBusArgument& arg, const QsdPrivate::UnitListEntry& ue)
{
	arg.beginStructure();
	arg << ue.id;
	arg << ue.description;
	arg << ue.load_state;
	arg << ue.active_state;
	arg << ue.sub_state;
	arg << ue.following;
	arg << ue.unit_path;
	arg << ue.job_id;
	arg << ue.job_type;
	arg << ue.job_path;
	arg.endStructure();
	return arg;
}

const QDBusArgument& operator>>(const QDBusArgument& arg, QsdPrivate::UnitListEntry& ue)
{
	arg.beginStructure();
	arg >> ue.id;
	arg >> ue.description;
	arg >> ue.load_state;
	arg >> ue.active_state;
	arg >> ue.sub_state;
	arg >> ue.following;
	arg >> ue.unit_path;
	arg >> ue.job_id;
	arg >> ue.job_type;
	arg >> ue.job_path;
	arg.endStructure();
	return arg;
}

QDBusArgument& operator<<(QDBusArgument& arg, const QsdPrivate::JobListEntry& je)
{
	arg.beginStructure();
	arg << je.id;
	arg << je.name;
	arg << je.type;
	arg << je.state;
	arg << je.job_path;
	arg << je.unit_path;
	arg.endStructure();
	return arg;
}

const QDBusArgument& operator>>(const QDBusArgument& arg, QsdPrivate::JobListEntry& je)
{
	arg.beginStructure();
	arg >> je.id;
	arg >> je.name;
	arg >> je.type;
	arg >> je.state;
	arg >> je.job_path;
	arg >> je.unit_path;
	arg.endStructure();
	return arg;
}

void QsdPrivate::registerMetaTypes()
{
	qRegisterMetaType<QsdPrivate::UnitListEntry>("QsdPrivate::UnitListEntry");
	qRegisterMetaType<QsdPrivate::JobListEntry>("QsdPrivate::JobListEntry");
	qDBusRegisterMetaType<QsdPrivate::UnitListEntry>();
	qDBusRegisterMetaType<QsdPrivate::JobListEntry>();
	qRegisterMetaType<QList<QsdPrivate::UnitListEntry> >("QList<QsdPrivate::UnitListEntry>");
	qRegisterMetaType<QList<QsdPrivate::JobListEntry> >("QList<QsdPrivate::JobListEntry>");
	qDBusRegisterMetaType<QList<QsdPrivate::UnitListEntry> >();
	qDBusRegisterMetaType<QList<QsdPrivate::JobListEntry> >();
}
