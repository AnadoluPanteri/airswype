/*
 * Copyright (c) 2011 Nokia Corporation.
 */

import QtQuick 1.0

AppWindow {
    id: container

    property bool freeDrawMode: false

    // This is set on Qt side
    property real scoreLimit

    // Start the game
    signal start(variant gamemode)

    // Stop measure accelerometer data
    signal stopMeasure()

    // Give next shape
    signal nextShape()

    // Stop all sounds playing
    signal stopAllSounds()

    // Play sound
    signal playSound(int index)

    // Change view state to measure accelerometer data
    function measureStarted() {
        container.state = "countdown_done"
    }

    // Change view state to show result of drawing
    function measureStopped() {
        container.state = "result"
    }

    anchors.fill: parent

    Image {
        anchors.fill: parent
        source: "qrc:/gfx/background.png"
    }

    MovingCloud {
        id: movingCloudHighest

        y: parent.height * 0.12
        minX: -width * 0.5; maxX: parent.width - width * 0.5
        velocity: 12
        source: "qrc:/gfx/bg_cloud1.png"
    }

    MovingCloud {
        id: movingCloudMiddle

        y: parent.height * 0.4
        minX: -width * 0.5; maxX: parent.width - width * 0.5
        velocity: 10
        source: "qrc:/gfx/bg_cloud2.png"
    }

    MovingCloud {
        id: movingCloudLowest

        y: parent.height * 0.7
        minX: -width * 0.5; maxX: parent.width - width * 0.5
        velocity: 8
        source: "qrc:/gfx/bg_cloud2.png"
    }

    Sun {
        id: sun

        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width; height: parent.height

        y: parent.height * 0.83
    }

    MovingCloud {
        id: bottomCloudFurthest

        anchors {
            bottom: parent.bottom; bottomMargin: height * -0.14
        }

        width: parent.width * 1.28; height: width * 0.31
        minX: parent.width - width; maxX: 0
        velocity: 6
        source: "qrc:/gfx/bottom_cloud2.png"
    }

    // Main menu
    Menu {
        id: menu

        anchors.fill: parent

        onOpacityChanged: {
            if(opacity > 0.95) {
                container.playSound(0);
            }
        }

        onPlayMenuPressAudio: {
            container.playSound(1);
        }

        // Listen menu selections
        onMenuItemSelected: {
            switch (index) {
            case 1: {
                container.freeDrawMode = false
                container.state = "target";
                break;
            }
            case 2: {
                container.freeDrawMode = true
                container.state = "countdown";
                break;
            }
            case 3: {
                container.state = "info";
                break;
            }
            case 4: {
                Qt.quit();
                break;
            }
            }
        }
    }

    /*
    * Result view
    * Shows drawing result and score
    */
    ResultView {
        id: resultView

        anchors.fill: parent
        opacity: 0
    }

    /*
    * Target view
    * Shows target of drawing
    */
    TargetView {
        id: targetView

        anchors.fill: parent
        opacity: 0
        onFinished: container.state = "countdown"
    }

    /*
    * Countdown view
    * User prepares to draw on the air
    */
    CountDownView {
        id: countDownView

        anchors.fill: parent
        opacity: 0
        onFinished: container.start(container.freeDrawMode)
        onCancelClicked: {
            container.stopMeasure();
            container.state = "result"
        }
        onPlayCountDownAudio: container.playSound(2)
    }

    /*
    * Info view
    * Shows application info and version
    */
    InfoView {
        id: infoView

        anchors.fill: parent
        opacity: 0
    }

    MovingCloud {
        id: bottomCloudNearest

        property real yMargin: height * -0.14

        anchors {
            bottom: parent.bottom
            bottomMargin: yMargin
        }

        width: parent.width * 1.28; height: width * 0.31
        minX: parent.width - width; maxX: 0
        velocity: 4
        source: "qrc:/gfx/bottom_cloud1.png"


        Behavior on yMargin {
            PropertyAnimation {
                duration: 200
                easing.type: Easing.InOutCubic
            }
        }
    }

    Button {
        id: retryButton

        anchors {
            left: parent.left
            bottom: parent.bottom
            margins: parent.width * 0.028
        }

        width: parent.width * 0.136; height: width * 1.306
        source: "qrc:/gfx/retry_button.png"
        opacity: 0
        onClicked: {
            container.state = "countdown"
        }
    }

    Button {
        id: nextButton

        anchors {
            left: parent.left
            bottom: parent.bottom
            margins: parent.width * 0.028
        }

        width: parent.width * 0.122; height: width * 1.205
        source: "qrc:/gfx/next_button.png"
        opacity: 0
        onClicked: {
            if (container.freeDrawMode)
                container.state = "countdown"
            else {
                // Gets new shape from DB
                container.nextShape();
                container.state = "target"
            }
        }
    }

    Button {
        id: backButton

        anchors {
            right: parent.right
            bottom: parent.bottom
            margins: parent.width * 0.028
        }

        width: parent.width * 0.122; height: width * 1.205
        source: "qrc:/gfx/back_button.png"
        opacity: 0
        onClicked: container.state = ""
    }

    Button {
        id: closeButton

        visible: container.exitButtonVisible

        anchors {
            top: parent.top; topMargin: 5
            right: parent.right; rightMargin: 5
        }

        source: "qrc:/gfx/close_button.png"
        onClicked: Qt.quit()
    }

    /*
    * Main view states
    */
    states: [
        State {
            name: "target"
            PropertyChanges { target: menu; opacity: 0 }
            PropertyChanges { target: targetView; opacity: 1 }
            StateChangeScript {
                name: "stopAudio"; script: container.stopAllSounds()
            }
            StateChangeScript {
                name: "targetShow";
                script: {
                    // Starts the timer of the target view
                    targetView.start();
                }
            }
        },
        State {
            name: "countdown"
            PropertyChanges { target: menu; opacity: 0 }
            PropertyChanges { target: countDownView; opacity: 1 }
            StateChangeScript {
                name: "startCountDown"; script: countDownView.start()
            }
            StateChangeScript {
                name: "stopAudio"; script: container.stopAllSounds()
            }
        },
        State {
            name: "countdown_done"
            PropertyChanges { target: menu; opacity: 0 }
            PropertyChanges { target: countDownView; opacity: 1 }
            PropertyChanges {
                target: resultView; opacity: 1; showScore: false
            }
            StateChangeScript { name: "go"; script: countDownView.showGo() }
        },
        State {
            name: "result"
            PropertyChanges { target: menu; opacity: 0 }
            PropertyChanges {
                target: targetView; opacity: container.freeDrawMode ? 0 : 1
            }
            PropertyChanges {
                target: resultView
                opacity: 1; showScore: container.freeDrawMode ? false : true
            }
            PropertyChanges { target: backButton; opacity: 1 }
            PropertyChanges {
                target: retryButton
                opacity: {
                    if(container.freeDrawMode)
                        return 1.0
                    if(resultView.score < container.scoreLimit)
                        return 1.0;
                    return 0.0;
                }
            }
            PropertyChanges {
                target: nextButton
                opacity: retryButton.opacity > 0.99 ? 0 : 1.0
            }
            PropertyChanges { target: bottomCloudNearest; yMargin: 0 }
            StateChangeScript {
                name: "scoreCheck";
                script: {
                    if(container.freeDrawMode) {
                        return;
                    }
                    if(resultView.score < container.scoreLimit) {
                        container.playSound(4);
                    }
                    else {
                        sun.peek();
                        container.playSound(3);
                    }
                }
            }
        },
        State {
            name: "info"
            PropertyChanges { target: menu; opacity: 0 }
            PropertyChanges { target: infoView; opacity: 1 }
            PropertyChanges { target: backButton; opacity: 1 }
            PropertyChanges { target: bottomCloudNearest; yMargin: 0 }
        }
    ]
}
