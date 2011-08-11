#include "manager.h"
#include "unit.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>

int main(int argc, char** argv)
{
	QCoreApplication app(argc, argv);

	foreach (const QsdUnit* unit, QsdManager::instance()->units())
	{
		qDebug() << unit->id() << unit->loadState() << unit->activeState() << unit->description();
	}
}
