/****************************************************************************
**
** Copyright (C) 2020 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Quick Timeline Add-on.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick 2.0
// Note: KeyframeGroup keyframeSource requires 1.1 version
import QtQuick.Timeline 1.1

Item {
    id: rootItem
    property vector3d v3: Qt.vector3d(0,0,0)

    Rectangle {
        id: rectangle1
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 10
        width: 120
        height: 120
        color: "red"
        radius: width / 2
    }
    Rectangle {
        id: rectangle2
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: 10
        width: 120
        height: 120
        color: "blue"
        radius: width / 2
    }
    Rectangle {
        id: rectangle3
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        width: 120
        height: 120
        color: "black"
        radius: width / 2
    }
    Text {
        font.pixelSize: 20
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 10
        text: "value: " + (rootItem.v3.x).toFixed(2)
              + ", " + (rootItem.v3.y).toFixed(2)
              + ", " + (rootItem.v3.z).toFixed(2)
        color: "black"
    }

    Timeline {
        id: timeline1
        enabled: true
        startFrame: 0
        endFrame: 1000

        animations: [
            TimelineAnimation {
                running: true
                duration: 2000
                from: 0
                to: 2000
                loops: Animation.Infinite
            }
        ]
        KeyframeGroup {
            target: rectangle1
            property: "opacity"
            keyframeSource: "animate_real.cbor"
        }
    }

    Timeline {
        id: timeline2
        enabled: true
        startFrame: 0
        endFrame: 2000

        animations: [
            TimelineAnimation {
                running: true
                duration: 2000
                from: 0
                to: 2000
                loops: Animation.Infinite
            }
        ]
        KeyframeGroup {
            target: rectangle2
            property: "visible"
            keyframeSource: "animate_bool.cbor"
        }
    }

    Timeline {
        id: timeline3
        enabled: true
        startFrame: 0
        endFrame: 2000

        animations: [
            TimelineAnimation {
                running: true
                duration: 2000
                from: 0
                to: 2000
                loops: Animation.Infinite
            }
        ]
        KeyframeGroup {
            target: rectangle3
            property: "color"
            keyframeSource: "animate_color.cbor"
        }
    }

    Timeline {
        id: timeline4
        enabled: true
        startFrame: 0
        endFrame: 2000

        animations: [
            TimelineAnimation {
                running: true
                duration: 10000
                from: 0
                to: 2000
                loops: Animation.Infinite
            }
        ]
        KeyframeGroup {
            target: rootItem
            property: "v3"
            keyframeSource: "animate_vector3d.cbor"
        }
    }
}
