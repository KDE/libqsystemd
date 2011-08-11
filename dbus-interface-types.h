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

#ifndef LIBQSYSTEMD_DBUSINTERFACETYPES_H
#define LIBQSYSTEMD_DBUSINTERFACETYPES_H

#include <QtCore/QMetaType>
class QDBusArgument;
#include <QtDBus/QDBusObjectPath>

namespace QsdPrivate {

struct UnitListEntry
{
	QString id;
	QString description;
	QString load_state;
	QString active_state;
	QString sub_state;
	QString following;
	QDBusObjectPath unit_path;
	uint job_id;
	QString job_type;
	QDBusObjectPath job_path;
};

struct JobListEntry
{
	uint id;
	QString name;
	QString type;
	QString state;
	QDBusObjectPath job_path;
	QDBusObjectPath unit_path;
};

struct JobSpec
{
	uint job_id;
	QDBusObjectPath job_path;
};

void registerMetaTypes();

} //namespace QsdPrivate

QDBusArgument& operator<<(QDBusArgument& arg, const QsdPrivate::UnitListEntry& ue);
const QDBusArgument& operator>>(const QDBusArgument& arg, QsdPrivate::UnitListEntry& ue);
QDBusArgument& operator<<(QDBusArgument& arg, const QsdPrivate::JobListEntry& je);
const QDBusArgument& operator>>(const QDBusArgument& arg, QsdPrivate::JobListEntry& je);
QDBusArgument& operator<<(QDBusArgument& arg, const QsdPrivate::JobSpec& js);
const QDBusArgument& operator>>(const QDBusArgument& arg, QsdPrivate::JobSpec& js);

Q_DECLARE_METATYPE(QsdPrivate::UnitListEntry)
Q_DECLARE_METATYPE(QsdPrivate::JobListEntry)
Q_DECLARE_METATYPE(QsdPrivate::JobSpec)
Q_DECLARE_METATYPE(QList<QsdPrivate::UnitListEntry>)
Q_DECLARE_METATYPE(QList<QsdPrivate::JobListEntry>)

#endif // LIBQSYSTEMD_DBUSINTERFACETYPES_H
