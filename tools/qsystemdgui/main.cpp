#include <manager.h>
#include <unit.h>

#include <QtGui/QApplication>
#include <QtGui/QTableWidget>

static QString unitTypeToString(QsdUnit::Type type)
{
	static const QVector<QString> strings = QVector<QString>()
		<< QLatin1String("service")
		<< QLatin1String("socket")
		<< QLatin1String("target")
		<< QLatin1String("device")
		<< QLatin1String("mount")
		<< QLatin1String("automount")
		<< QLatin1String("snapshot")
		<< QLatin1String("timer")
		<< QLatin1String("swap")
		<< QLatin1String("path");
	return strings.value(type);
}

static QString activeStateToString(QsdUnit::ActiveState state)
{
	static const QVector<QString> strings = QVector<QString>()
		<< QLatin1String("active")
		<< QLatin1String("reloading")
		<< QLatin1String("inactive")
		<< QLatin1String("failed")
		<< QLatin1String("activating")
		<< QLatin1String("deactivating");
	return strings.value(state);
}

static QString loadStateToString(QsdUnit::LoadState state)
{
	static const QVector<QString> strings = QVector<QString>()
		<< QLatin1String("stub")
		<< QLatin1String("loaded")
		<< QLatin1String("error")
		<< QLatin1String("merged")
		<< QLatin1String("masked");
	return strings.value(state);
}

int main(int argc, char** argv)
{
	QApplication app(argc, argv);

	const QList<QsdUnit*> units = QsdManager::instance()->units();

	QTableWidget table;
	table.setColumnCount(4);
	table.setRowCount(units.size());
	table.setHorizontalHeaderLabels(QStringList()
		<< QLatin1String("Unit")
		<< QLatin1String("Type")
		<< QLatin1String("Load state")
		<< QLatin1String("Active state")
	);

	for (int i = 0; i < units.size(); ++i)
	{
		const QsdUnit* unit = units[i];
		table.setItem(i, 0, new QTableWidgetItem(unit->id()));
		table.setItem(i, 1, new QTableWidgetItem(unitTypeToString(unit->type())));
		table.setItem(i, 2, new QTableWidgetItem(loadStateToString(unit->loadState())));
		table.setItem(i, 3, new QTableWidgetItem(activeStateToString(unit->activeState())));
	}

	table.show();
	return app.exec();
}
