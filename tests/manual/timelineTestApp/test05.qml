/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd
** All rights reserved.
** For any questions to The Qt Company, please use contact form at http://www.qt.io/contact-us
**
** This file is part of the Qt Enterprise Qt Quick Timeline Add-on.
**
** Licensees holding valid Qt Enterprise licenses may use this file in
** accordance with the Qt Enterprise License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company.
**
** If you have questions regarding the use of this file, please use
** contact form at http://www.qt.io/contact-us
**
****************************************************************************/

import QtQuick 2.0
import QtQuick.Timeline 1.0

Item {
    width: 640
    height: 480

    id: root

    state: "page02"

    Rectangle {
        id: rectangle
        x: 0
        y: 0
        width: 64
        height: 64
        color: "#747474"

        MouseArea {
            anchors.fill: parent
            id: leftArea
        }
    }

    Rectangle {
        id: rectangle1
        x: 576
        y: 0
        width: 64
        height: 64
        color: "#747474"

        MouseArea {
            anchors.fill: parent
            id: rightArea
        }
    }

    Item {
        id: item1
        x: -640
        y: 123
        width: 1920
        height: 480

        Rectangle {
            id: screen01
            x: 0
            y: 0
            width: 640
            height: 359
            color: "#ffffff"

            Text {
                x: 0
                y: 0
                text: qsTr("Page 01")
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 38
            }
        }


        Rectangle {
            id: screen02
            x: 640
            y: 0
            width: 640
            height: 359
            color: "#ffffff"

            Text {
                x: 0
                y: 0
                text: qsTr("Page 02")
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 38
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }



        Rectangle {
            id: screen03
            x: 1280
            y: 0
            width: 640
            height: 359
            color: "#ffffff"

            Text {
                x: 0
                y: 0
                text: qsTr("Page 03")
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 38
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }
    }

    Timeline {
        id: timeline
        endFrame: 1000
        startFrame: 0
        enabled: true

        animations: [
            TimelineAnimation {
                id: animationToPage01
                from: 500
                to: 0
                running: false
                onFinished: root.state = "page01"
            },
            TimelineAnimation {
                id: animationToPage02Left
                from: 0
                to: 500
                running: false
                onFinished: root.state = "page02"
            },
            TimelineAnimation {
                id: animationToPage02Right
                from: 1000
                to: 500
                running: false
                onFinished: root.state = "page02"
            },
            TimelineAnimation {
                id: animationToPage03
                from: 500
                to: 1000
                running: false
                onFinished: root.state = "page03"
            }

        ]

        KeyframeGroup {
            target: item1
            property: "x"

            Keyframe {
                frame: 0
                value: 0
            }

            Keyframe {
                frame: 500
                value: -640
            }

            Keyframe {
                frame: 1000
                value: -1280
            }
        }
    }

    Connections {
        target: rightArea
        enabled: root.state == "page01"
        onClicked: root.state = "toPage02FromLeft"
    }

    Connections {
        target: rightArea
        enabled: root.state == "page02"
        onClicked: root.state = "toPage03"
    }

    Connections {
        target: leftArea
        enabled: root.state == "page02"
        onClicked: root.state = "toPage01"
    }

    Connections {
        target: leftArea
        enabled: root.state == "page03"
        onClicked: root.state = "toPage02FromRight"
    }

    states: [
        State {
            name: "page01"

            PropertyChanges {
                target: timeline
                currentFrame: 0
            }
        },
        State {
            name: "page02"

            PropertyChanges {
                target: timeline
                currentFrame: 500
            }
        },
        State {
            name: "page03"

            PropertyChanges {
                target: timeline
                currentFrame: 1000
            }
        },
        State {
            name: "toPage01"
            PropertyChanges {
                target: animationToPage01
                running: true
            }
        },
        State {
            name: "toPage02FromLeft"
            PropertyChanges {
                target: animationToPage02Left
                running: true
            }
        },
        State {
            name: "toPage03"
            PropertyChanges {
                target: animationToPage03
                running: true
            }
        },
        State {
            name: "toPage02FromRight"
            PropertyChanges {
                target: animationToPage02Right
                running: true
            }
        }
    ]

}

/*##^## Designer {
    D{i:62;anchors_width:100;anchors_height:100}D{i:47;currentFrame__AT__NodeInstance:1}
}
 ##^##*/
