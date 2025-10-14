import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

import dev.decovar.Backend 1.0

Window {
    visible: true
    width: 700
    minimumWidth: 500
    height: 300
    minimumHeight: 200

    readonly property string defaultColor: "white"

    Backend {
        id: backend
    }

    Rectangle {
        id: background
        anchors.fill: parent

        Button {
            anchors.centerIn: parent

            text: "About Qt"
            onClicked: {
                backend.showAboutQt();
            }
        }
    }
}
