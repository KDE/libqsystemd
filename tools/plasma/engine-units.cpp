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

#include "engine-units.h"

#include <manager.h>
#include <unit.h>

#include <KDE/KLocalizedString>
#include <Plasma/DataContainer>

QsystemdUnitsDataEngine::QsystemdUnitsDataEngine(QObject* parent, const QVariantList& args)
	: Plasma::DataEngine(parent, args)
{
	//don't poll systemd more often than twice a second
	setMinimumPollingInterval(500);
}

static QString unitTypeToString(QsdUnit::Type type)
{
	static const QVector<QString> strings = QVector<QString>()
		<< i18nc("Type of a systemd unit", "service")
		<< i18nc("Type of a systemd unit", "socket")
		<< i18nc("Type of a systemd unit", "target")
		<< i18nc("Type of a systemd unit", "device")
		<< i18nc("Type of a systemd unit", "mount")
		<< i18nc("Type of a systemd unit", "automount")
		<< i18nc("Type of a systemd unit", "snapshot")
		<< i18nc("Type of a systemd unit", "timer")
		<< i18nc("Type of a systemd unit", "swap")
		<< i18nc("Type of a systemd unit", "path");
	return strings.value(type);
}

static QString activeStateToString(QsdUnit::ActiveState state)
{
	static const QVector<QString> strings = QVector<QString>()
		<< i18nc("Active state of a systemd unit", "active")
		<< i18nc("Active state of a systemd unit", "reloading")
		<< i18nc("Active state of a systemd unit", "inactive")
		<< i18nc("Active state of a systemd unit", "failed")
		<< i18nc("Active state of a systemd unit", "activating")
		<< i18nc("Active state of a systemd unit", "deactivating");
	return strings.value(state);
}

static QString loadStateToString(QsdUnit::LoadState state)
{
	static const QVector<QString> strings = QVector<QString>()
		<< i18nc("Load state of a systemd unit", "stub")
		<< i18nc("Load state of a systemd unit", "loaded")
		<< i18nc("Load state of a systemd unit", "error")
		<< i18nc("Load state of a systemd unit", "merged")
		<< i18nc("Load state of a systemd unit", "masked");
	return strings.value(state);
}

void QsystemdUnitsDataEngine::init()
{
	foreach (const QsdUnit* unit, QsdManager::instance()->units())
	{
		const QString id = unit->id();
		setData(id, "description", unit->description());
		setData(id, "type", unitTypeToString(unit->type()));
		setData(id, "active_state", activeStateToString(unit->activeState()));
		setData(id, "load_state", loadStateToString(unit->loadState()));
	}
}

K_EXPORT_PLASMA_DATAENGINE(systemd-units, QsystemdUnitsDataEngine)

#include "engine-units.moc"
