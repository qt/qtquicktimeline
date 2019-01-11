/******************************************************************************
**
** Copyright (C) 2019 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the Qt Quick Timeline module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qquicktimelineanimation_p.h"

#include "qquicktimeline_p.h"

#include <private/qobject_p.h>
#include <private/qquickanimation_p_p.h>

#include <QTimer>

QT_BEGIN_NAMESPACE

/*!
    \qmltype TimelineAnimation
    \inherits QQuickNumberAnimation
    \instantiates QQuickTimelineAnimation
    \inqmlmodule QtQuick.Timeline
    \ingroup qtqmltypes

    \brief A number animation attached to a timeline.

    Specifies a number animation attached to a timeline that animates the
    properties of an object.
*/

/*!
    \qmlproperty bool TimelineAnimation::pingPong

    Whether the animation .
*/

/*!
    \qmlsignal TimelineAnimation::finished

    This signal is emitted when the timeline animation finishes.
*/

QQuickTimelineAnimation::QQuickTimelineAnimation(QObject *parent) : QQuickNumberAnimation(parent)
{
    setProperty(QLatin1String("currentFrame"));
    connect(this, &QQuickAbstractAnimation::started, this, &QQuickTimelineAnimation::handleStarted);
    connect(this, &QQuickAbstractAnimation::stopped, this, &QQuickTimelineAnimation::handleStopped);
}

void QQuickTimelineAnimation::setPingPong(bool b)
{
    if (b == m_pinpong)
        return;

    m_pinpong = b;
    emit pingPongChanged();
}

bool QQuickTimelineAnimation::pingPong() const
{
    return m_pinpong;
}

void QQuickTimelineAnimation::handleStarted()
{
    QQuickTimeline* timeline = qobject_cast<QQuickTimeline*>(parent());

    if (!timeline)
        return;

    for (QQuickTimelineAnimation *other : timeline->getAnimations()) {
        if (other != this)
            other->stop();
    }

    auto *privateObject = static_cast<QQuickPropertyAnimationPrivate*>(QObjectPrivate::get(this));

    if (m_pinpong && m_originalStart) {
        m_originalLoop = privateObject->loopCount;
        m_currentLoop = 0;
        privateObject->loopCount = 1;
        privateObject->animationInstance->setLoopCount(1);
        m_originalStart = false;
        m_reversed = false;
    }
}

static void swapStartEnd(QQuickPropertyAnimationPrivate *privateObject)
{
    std::swap(privateObject->to, privateObject->from);
}

void QQuickTimelineAnimation::handleStopped()
{
    if (!m_pinpong) {
        emit finished();
        return;
    }

    auto *privateObject = static_cast<QQuickPropertyAnimationPrivate*>(QObjectPrivate::get(this));

    if (m_reversed) {
        m_currentLoop++;
    }

    if (!(privateObject->animationInstance->currentTime() < privateObject->duration)
            && (m_currentLoop < m_originalLoop
                || m_originalLoop == -1)) {
        swapStartEnd(privateObject);

        m_reversed = !m_reversed;
        QQuickTimelineAnimation::start();

    } else {
        if (m_reversed) {
            swapStartEnd(privateObject);
        }
        m_originalStart = true;
        m_reversed = false;
        privateObject->loopCount = m_originalLoop;
        emit finished();
    }
}

QT_END_NAMESPACE
