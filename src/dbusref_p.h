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

#ifndef LIBQSYSTEMD_DBUSREF_P_H
#define LIBQSYSTEMD_DBUSREF_P_H

#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusObjectPath>

// This struct contains the location of a DBus interface, that is: a triple of
// bus connection, service name, and path. QsdPrivate::DBusRef does *not*
// contain the interface name, because that depends on the application.
//
// Inside qsystemd, this struct is used to identify interfaces on the DBus.
// The systemd API uses only QDBusObjectPath for that, because bus and service
// name are always the same.
namespace QsdPrivate
{
	struct DBusRef
	{
		QDBusConnection connection;
		QString service, path;

		DBusRef(const QDBusConnection& connection, const QString& service, const QString& path)
			: connection(connection), service(service), path(path)
		{}

		inline DBusRef siblingPath(const QDBusObjectPath& newPath) const
		{
			return DBusRef(connection, service, newPath.path());
		}
	};
}

// Instantiates an interface of the given type from the given QsdPrivate::DBusRef.
template<typename T> static inline T* qsdFromDBusRef(const QsdPrivate::DBusRef& ref, QObject* parent = 0)
{
	return new T(ref.service, ref.path, ref.connection, parent);
}

// Mixin class for auto-generated QDBusInterface classes that provides a
// QsdPrivate::DBusRef-based constructor.
template<typename T> class QsdFromDBusRef : public T
{
	public:
		explicit inline QsdFromDBusRef(const QsdPrivate::DBusRef& ref, QObject* parent = 0)
			: T(ref.service, ref.path, ref.connection, parent)
		{}

		inline QsdPrivate::DBusRef ref() const
		{
			return QsdPrivate::DBusRef(T::connection(), T::service(), T::path());
		}
};

#endif // LIBQSYSTEMD_DBUSREF_P_H
