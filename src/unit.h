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

#ifndef LIBQSYSTEMD_UNIT_H
#define LIBQSYSTEMD_UNIT_H

#include "enumerations.h"
class QsdManager;

#include <libqsystemd_export.h>

/**
 * @class QsdUnit unit.h <qsystemd/QsdUnit>
 *
 * This class represents a unit that is available in systemd. Instances are
 * created by QsdManager; any QsdUnit instance is guaranteed to correspond to
 * a unit actually known to systemd.
 *
 * When storing references to QsdUnit instances, be sure to listen to the
 * QsdManager::unitRemoved() signal. Units will be deleted when one such signal
 * is processed.
 */
//TODO: QsdUnitRef class (weak reference to a unit by manager instance + id)
class QSYSTEMD_EXPORT QsdUnit
{
	private:
		class Private;
		friend class QsdManager;

		QsdUnit(Private* d);
		~QsdUnit();
		Q_DISABLE_COPY(QsdUnit)
	public:
		typedef QsdUnitType::UnitType Type;
		typedef QsdActiveState::ActiveState ActiveState;
		typedef QsdLoadState::LoadState LoadState;

		///@return the identifier of this unit
		QString id() const;
		///@return a mostly human-readable description of this unit
		QString description() const;
		//TODO: map UnitListEntry::{following,job_id,job_type,sub_state}

		///@return the type of unit
		QsdUnit::Type type() const;
		///@return the active_state of this unit
		QsdUnit::ActiveState activeState() const;
		///@return the load_state of this unit
		QsdUnit::LoadState loadState() const;
	private:
		Private* const d;
};

#endif // LIBQSYSTEMD_UNIT_H
