/*
 * Copyright (c) 2011-2014 Microsoft Mobile.
 */

import QtQuick 1.0

AppWindow {
    id: container

    signal hideFinished()

    // Hides the splash screen
    function hide() {
        animation.start()
    }

    anchors.fill: parent
    z: 100

    Image {
        anchors.fill: parent
        source: "../gfx/airswype_splashscreen.png"
        fillMode: Image.Stretch
    }

    SequentialAnimation {
        id: animation

        PropertyAnimation {
            target: container
            easing.overshoot: 2.5
            properties: "scale, opacity"
            to: 0
            duration: 1000
            easing.type: Easing.InBack
        }

        ScriptAction {
            script: container.hideFinished()
        }
    }
}
