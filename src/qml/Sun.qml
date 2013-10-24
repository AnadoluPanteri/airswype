/*
 * Copyright (c) 2011 Nokia Corporation.
 */

import QtQuick 1.0

Item {
    id: container

    property real shineVelocity
    property real peekHeight: 100

    // Let the sun shine
    function peek() {
        peekAnimation.start()
    }

    width: 100; height: 62

    Component.onCompleted: {
        sun.initialY = y;
    }

    Image {
        id: shine

        anchors.centerIn: sun

        width: height; height: parent.height * 2
        source: "qrc:/gfx/background_shine.png"
        smooth: true
        opacity: 0.9

        PropertyAnimation on rotation {
            running: true
            loops: Animation.Infinite
            from: 0; to: 360
            duration: 100000
        }
    }

    Image {
        id: sun

        property real initialY

        anchors.horizontalCenter: parent.horizontalCenter

        source: "qrc:/gfx/sun.png"
        smooth: true

        SequentialAnimation {
            id: peekAnimation

            PropertyAnimation {
                target: container
                property: "y"
                to: sun.initialY - container.peekHeight
                duration: 1000
                easing.type: Easing.InOutQuart
            }
            PauseAnimation { duration: 1000 }

            PropertyAnimation {
                target: container
                property: "y"
                to: sun.initialY
                duration: 1000
                easing.type: Easing.InOutQuart
            }
        }
    }
}
