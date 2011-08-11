#include "dbus-interface-types.h"
#include "systemd_manager_interface.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>

int main(int argc, char** argv)
{
	QCoreApplication app(argc, argv);
	QsdPrivate::registerMetaTypes();

	org::freedesktop::systemd1::Manager manager(
		"org.freedesktop.systemd1",
		"/org/freedesktop/systemd1",
		QDBusConnection::systemBus()
	);
	QList<QsdPrivate::UnitListEntry> unitEntries = manager.ListUnits();

	foreach (const QsdPrivate::UnitListEntry& unitEntry, unitEntries)
	{
		qDebug() << 1 << unitEntry.id;
		qDebug() << 2 << unitEntry.description;
		qDebug() << 3 << unitEntry.load_state;
		qDebug() << 4 << unitEntry.active_state;
		qDebug() << 5 << unitEntry.sub_state;
		qDebug() << 6 << unitEntry.following;
		qDebug() << 7 << unitEntry.unit_path.path();
		qDebug() << 8 << unitEntry.job_id;
		qDebug() << 9 << unitEntry.job_type;
		qDebug() << 10 << unitEntry.job_path.path();
	}
}
