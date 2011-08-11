import Qt 4.7
import org.kde.plasma.core 0.1 as PlasmaCore

Item {
	id: main

	PlasmaCore.DataSource {
		id: unitsSource
		engine: "systemd-units"
		interval: 0
		Component.onCompleted: {
			connectedSources = sources
		}
	}

	Column {
		Repeater {
			model: PlasmaCore.DataModel {
				dataSource: unitsSource
			}
			delegate: Text {
				text: "Unit " + model["DataEngineSource"] + " (" + type + ")"
			}
		}
	}
}
