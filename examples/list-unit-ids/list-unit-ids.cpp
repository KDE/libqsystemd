#include <cstdio>
#include <QtCore/QTextStream>
#include <qsystemd/manager.h>
#include <qsystemd/unit.h>

int main(int argc, char** argv)
{
	QStringList unitIds;
	foreach (QsdUnit* unit, QsdManager::instance()->units())
		unitIds << unit->id();

	QTextStream stream(stdout);
	qSort(unitIds);
	foreach (const QString& unitId, unitIds)
		stream << unitId << "\n";
}
