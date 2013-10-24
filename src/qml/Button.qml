/*
 * Copyright (c) 2011 Nokia Corporation.
 */

import QtQuick 1.0

Image {
    id: button

    property alias enabled: mouseArea.enabled

    signal clicked()

    width: 60; height: 60
    fillMode: Image.PreserveAspectFit
    smooth: true

    Behavior on scale { PropertyAnimation { duration: 50 } }
    Behavior on opacity { PropertyAnimation { duration: 200 } }

    MouseArea {
        id: mouseArea

        anchors.fill: parent
        onClicked: button.clicked()
        onPressed: button.scale = 0.9
        onReleased: button.scale = 1.0
        scale: 1.5
    }
}
