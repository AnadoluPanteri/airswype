/*
 * Copyright (c) 2011 Nokia Corporation.
 */

import QtQuick 1.0

Rectangle {
    id: container
    objectName: "infoView"

    property string version

    width: 360; height: 640
    color: "#30303070"

    Behavior on opacity {
        PropertyAnimation {
            duration: 500
        }
    }

    Flickable {
        id: flickable

        anchors {
            fill: parent
            topMargin: 50
            bottomMargin: 90
            leftMargin: 20
            rightMargin: 20
        }

        contentHeight: text.height

        Text {
            id: text

            width: flickable.width
            color: "white"
            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
            style: Text.Raised
            styleColor: "black"
            font.pixelSize: width * 0.04
            text: "<h2>AirSwype " + container.version + "</h2>" +
                  "<p>AirSwype is a Nokia example application where the user " +
                  "must draw the presented figure in the air with the phone " +
                  "as precisely as possible.</p>" +
                  "<p>For more information about the project see the " +
                  "<a href=\"https://projects.developer.nokia.com/airswype\">" +
                  "AirSwype project page</a></p>" +
                  "<h3>Instructions</h3>" +
                  "<p>The application has two modes: a game mode and a " +
                  "free drawing mode.</p>" +
                  "<h2>Game mode</h2>" +
                  "In game mode, try to draw the target shape shown by the " +
                  "red line in the air with the phone. The application will " +
                  "show a countdown from 3, and you must hold the phone " +
                  "still during the countdown when the text \"Hold still\" " +
                  "is shown. When the text \"Go\" is shown, you may begin " +
                  "to draw the target shape. During the drawing, try to " +
                  "hold the phone in the same orientation that you held it " +
                  "during the countdown - otherwise the phone will not " +
                  "detect the shape properly. Also, pay attention to the " +
                  "requested drawing direction of the shape. AirSwype will " +
                  "randomly flip and rotate the shapes in every new " +
                  "game.</p> " +
                  "<p>The application will stop the drawing automatically if " +
                  "the drawn shape is at least 75% match of the target " +
                  "shape. If you fail to draw the shape with enough " +
                  "precision, you have to cancel the drawing by tapping the " +
                  "Cancel button.</p>"  +
                  "<p>The shapes will get more difficult after each " +
                  "successful drawing.</p>" +
                  "<h2>Free draw mode</h2>" +
                  "<p>In free draw mode, you can practice the drawing in the " +
                  "air and study how the phone detects the movement.</p>" +
                  "<h3>Sounds</h3>" +
                  "<p>All the sounds used in the application have been " +
                  "downloaded from The Freesound Project.</p>" +
                  "<p>The following samples were used in the application:</p>" +
                  "<p><strong>PhreaKsAccount:</strong></p>" +
                  "<p>46448__PhreaKsAccount__cheer1,<br>" +
                  "46450__PhreaKsAccount__cheer2,<br>" +
                  "46451__PhreaKsAccount__cheer3,<br>" +
                  "46452__PhreaKsAccount__cheer4,<br>" +
                  "46455__PhreaKsAccount__cheer7,<br>" +
                  "46456__PhreaKsAccount__cheer8,<br>" +
                  "46457__PhreaKsAccount__cheer9,<br>" +
                  "46463__PhreaKsAccount__jibberish1,<br>" +
                  "46465__PhreaKsAccount__jibberish2,<br>" +
                  "46466__PhreaKsAccount__jibberish3,<br>" +
                  "46467__PhreaKsAccount__jibberish4,<br>" +
                  "46468__PhreaKsAccount__jibberish5,<br>" +
                  "46469__PhreaKsAccount__jibberish6,<br>" +
                  "46470__PhreaKsAccount__jibberish7,<br>" +
                  "46471__PhreaKsAccount__jibberish8,<br>" +
                  "46472__PhreaKsAccount__jibberish9,</p>" +
                  "<p><strong>offthesky:</strong></p>" +
                  "<p>35722__offthesky__wind_chimes,</p>" +
                  "<p><strong>qubodup</strong></p>" +
                  "<p>60000__qubodup__swosh_13,</p>" +
                  "<p><strong>Robinhood76</strong></p>" +
                  "<p>92909__Robinhood76__01526_swoosh_2.<br><br><br><br></p>"

            onLinkActivated: Qt.openUrlExternally(link)
        }
    }
}
