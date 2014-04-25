/*
 * Copyright (c) 2011-2014 Microsoft Mobile.
 */

import QtQuick 1.0

Item {
    id: container

    property int countDownFrom: 5

    signal finished()
    signal cancelClicked()
    signal playCountDownAudio()

    function start() {
        counter.value = container.countDownFrom;
        timer.interval = 1300
        timer.start();
        cancelButton.opacity = 0;
    }

    function showGo() {
        timer.interval = 500
        timer.restart();
    }

    anchors.fill: parent

    Behavior on opacity { PropertyAnimation { duration: 100 } }

    Timer {
        id: timer

        triggeredOnStart: true
        repeat: true
        onTriggered: {
            counter.value--;

            if(counter.value == 4) {
                counter.scale = 1;
            }

            if(counter.value < 4) {
                animation.restart();
            }

            if(counter.value == 1) {
                counter.scale = 1;
                timer.stop();
                container.finished();
            }
            else if(counter.value == -1) {
                timer.stop();
                cancelButton.opacity = 0.5;
            }
        }
    }

    Text {
        id: counterText

        anchors {
            top: parent.top; topMargin: parent.height * 0.15
            horizontalCenter: parent.horizontalCenter
        }

        color: "white"
        font.pixelSize: Math.min(parent.width, parent.height) * 0.1
        style: Text.Outline;
        styleColor: "#47afff"
    }

    Image {
        id: counter

        property int value: -1

        anchors.centerIn: parent
        smooth: true

        source: {
            if(value == 4) {
                return "qrc:/gfx/hold_still.png"
            }
            if(value == 3) {
                container.playCountDownAudio();
                return "qrc:/gfx/count_3.png"
            }
            else if(value == 2) {
                container.playCountDownAudio();
                return "qrc:/gfx/count_2.png"
            }
            else if(value == 1) {
                container.playCountDownAudio();
                counterText.text = "Hold still";
                return "qrc:/gfx/count_1.png"
            }
            else if(value == 0) {
                counterText.text = "";
                return "qrc:/gfx/go_cloud.png"
            }
            else {
                return ""
            }
        }
    }

    Image {
        id: cancelButton

        anchors {
            top: parent.top; topMargin: parent.height * 0.20
            horizontalCenter: parent.horizontalCenter
        }

        width: sourceSize.width * 2; height: sourceSize.height * 2
        opacity: 0
        smooth: true
        source: "qrc:/gfx/done_button.png"

        Text {
            anchors.centerIn: parent

            text: "Cancel"
            color: "white"
            style: Text.Raised
            styleColor: "black"

            font.bold: true
            font.pixelSize: Math.min(parent.width, parent.height) * 0.5
        }

        MouseArea {
            anchors.fill: parent

            scale: 2
            onPressed: parent.scale = 0.9
            onReleased: parent.scale = 1.0
            onClicked: {
                cancelButton.opacity = 0
                container.cancelClicked()
            }
        }
    }

    PropertyAnimation {
        id: animation

        from: 1.0
        to: 0.1

        target: counter
        property: "scale"
        duration: timer.interval
    }
}
