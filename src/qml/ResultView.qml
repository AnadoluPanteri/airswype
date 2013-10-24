/*
 * Copyright (c) 2011 Nokia Corporation.
 */

import QtQuick 1.0
import SwypeElements 1.0

Item {
    id: container

    property bool showScore: true

    property alias score: scoreText.score

    width: 360; height: 640

    // Custom QML element, see swypeelement.h
    Swype {
        objectName: "resultSwypeView"

        anchors {
            fill: parent
            margins: 50
        }

        smooth: true
    }

    Text {
        id: scoreText
        objectName: "scoreText"

        opacity: container.showScore ? 1 : 0

        property real score: 100

        anchors {
            top: parent.top; topMargin: parent.height * 0.15
            horizontalCenter: parent.horizontalCenter
        }

        color: "#ffff30"
        text: score.toPrecision(3) + " %"
        font.pixelSize: parent.width * 0.10
        style: Text.Outline; styleColor: "#fec111"
    }
}
