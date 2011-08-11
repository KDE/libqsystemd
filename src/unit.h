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

class QsdManager;

#include <QtCore/QStringList>

#include <libqsystemd_export.h>

namespace QsdUnitType
{
	/**
	 * @enum QsdUnitType::UnitType
	 *
	 * The recommended typedef for this enum is QsdUnit::Type.
	 */
	enum UnitType
	{
		Service,
		Socket,
		Target,
		Device,
		Mount,
		Automount,
		Snapshot,
		Timer,
		Swap,
		Path,
		Invalid = -1
	};
}

namespace QsdActiveState
{
	/**
	 * @enum QsdActiveState::ActiveState
	 *
	 * The recommended typedef for this enum is QsdUnit::ActiveState.
	 */
	enum ActiveState
	{
		Active,
		Reloading,
		Inactive,
		Failed,
		Activating,
		Deactivating,
		Invalid = -1
	};
}

namespace QsdLoadState
{
	/**
	 * @enum QsdLoadState::LoadState
	 *
	 * The recommended typedef for this enum is QsdUnit::LoadState.
	 */
	enum LoadState
	{
		Stub,
		Loaded,
		Error,
		Merged,
		Masked,
		Invalid = -1
	};
}

/**
 * @class QsdUnit unit.h <QSystemd/QsdUnit>
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
