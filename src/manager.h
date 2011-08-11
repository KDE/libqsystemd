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

#ifndef LIBQSYSTEMD_MANAGER_H
#define LIBQSYSTEMD_MANAGER_H

#include <QtCore/QObject>

#include <libqsystemd_export.h>

class QsdUnit;

/**
 * @class QsdManager manager.h <qsystemd/QsdManager>
 *
 * This class represents the systemd process itself. It can be used to query
 * its state, issue commands not related to a certain unit or job, and list
 * units and jobs.
 *
 * All QsdUnit and QsdJob instances are created by this class.
 */
class QSYSTEMD_EXPORT QsdManager : public QObject
{
	private:
		class Private;
		QsdManager(Private* d);
		~QsdManager();
	public:
		///@return the only QsdManager instance
		static QsdManager* instance();

		///@return a list of all units currently available in systemd
		QList<QsdUnit*> units() const;
	Q_SIGNALS:
		//TODO: unitAdded, unitRemoved
	private:
		Private* const d;
};

#endif // LIBQSYSTEMD_MANAGER_H
