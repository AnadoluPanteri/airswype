/*
* Copyright (c) 2011 Nokia Corporation.
*/

import QtQuick 1.0
import Qt.labs.particles 1.0

Item {
    id: container

    signal menuItemSelected(int index)
    signal playMenuPressAudio()

    property variant selectionIndex

    anchors.fill: parent

    Behavior on opacity {
        PropertyAnimation {
            duration: 300
        }
    }

    Image {
        id: logo

        anchors {
            top: parent.top; topMargin: parent.height * 0.08
            horizontalCenter: parent.horizontalCenter
        }

        source: "qrc:/gfx/airswype_logo.png"
    }

    ListModel {
         id: menuModel

         ListElement {
             name: "Exit"
             index: 4
         }
         ListElement {
             name: "Free Drawing"
             index: 2
         }
         ListElement {
             name: "New Game"
             index: 1
         }
         ListElement {
             name: "Info"
             index: 3
         }
     }

    Component {
        id: menuDelegate

        Image {
            width: PathView.width
            scale: PathView.elementScale
            opacity: PathView.elementOpacity
            z: PathView.elementZ
            source: "qrc:/gfx/menu_cloud.png"
            smooth: true

            Text {
                id: text

                anchors.centerIn: parent
                color: "#218dcd";
                text: model.name
                //font.bold: true
                font.pixelSize: Math.min(parent.width, parent.height) * 0.16
            }

            Particles {
                id: explodeParticles

                width: 1; height: 1
                opacity: 0
                emissionRate: 0
                source: "qrc:/gfx/particle.png"
                lifeSpan: 500
                lifeSpanDeviation: 2000
                count: -1
                angle: 270
                angleDeviation: 65
                velocity: 100
                velocityDeviation: 100

                ParticleMotionGravity {
                    yattractor: 1000
                    xattractor: 300
                    acceleration: 50
                }
            }

            MouseArea {
                anchors.fill: parent

                onClicked: {
                    playMenuPressAudio();
                    explodeParticles.x = parent.x + parent.width / 2;
                    explodeParticles.y = parent.y + parent.height / 3;
                    explodeParticles.burst(20);
                    explodeParticles.opacity = 1;

                    selectionIndex = model.index;
                    selectionTimer.restart();
                }
            }
        }
    }

    Timer {
        id: selectionTimer

        repeat: false
        interval: 700
        onTriggered: {
            menuItemSelected(selectionIndex);
        }
    }

    PathView {
        id: pview

        property int xCap: width * 0.5
        property int yCap: container.rowHeight * 0.5

        anchors.centerIn: parent
        width: parent.width * 0.8; height: parent.height * 0.5
        pathItemCount: 4
        model: menuModel
        delegate: menuDelegate

        path:
            Path {

            // Top
            startX: pview.xCap; startY: pview.yCap
            PathAttribute { name: "elementScale"; value: 0.7 }
            PathAttribute { name: "elementZ"; value: 10 }
            PathAttribute { name: "elementOpacity"; value: 0 }

            // Mid
            PathQuad { x: pview.xCap; y: pview.height * 0.5;
                controlX: pview.xCap; controlY: pview.yCap }
            PathAttribute { name: "elementScale"; value: 1 }
            PathAttribute { name: "elementZ"; value: 100 }
            PathAttribute { name: "elementOpacity"; value: 1 }

            // Low
            PathQuad { x: pview.xCap; y: pview.height - pview.yCap;
                controlX: pview.xCap; controlY: pview.height - pview.yCap }
            PathAttribute { name: "elementScale"; value: 0.7 }
            PathAttribute { name: "elementZ"; value: 1 }
            PathAttribute { name: "elementOpacity"; value: 0 }
        }
    }
}
