/*
 * Copyright (c) 2011 Nokia Corporation.
 */

import QtQuick 1.0
import SwypeElements 1.0

Item {
    id: container

    signal finished()

    function start() {
        timer.restart();
    }

    width: 360; height: 640

    Timer {
        id: timer

        interval: targetSwype.timeToShow
        onTriggered: container.finished();
    }

    // Custom QML element, see swypeelement.h
    Swype {
        id: targetSwype
        objectName: "targetSwypeView"

        property real timeToShow: 1500

        anchors {
            fill: parent
            margins: 50
        }

        opacity: 0.5
        smooth: true
    }
}
