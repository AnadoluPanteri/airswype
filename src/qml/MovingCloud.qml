/*
 * Copyright (c) 2011 Nokia Corporation.
 */

import QtQuick 1.0

Image {
    id: container

    property real minX
    property real maxX
    property real velocity: 12

    smooth: true

    Behavior on x {
        SmoothedAnimation {
            velocity: container.velocity
        }
    }

    Timer {
        running: true
        repeat: true
        triggeredOnStart: true

        interval: 7500
        onTriggered: {
            container.x = Math.random() * (maxX - minX) + minX
        }
    }
}
