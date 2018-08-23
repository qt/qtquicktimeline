/******************************************************************************
**
** Copyright (C) 2018 The Qt Company Ltd.
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

#include "qquicktimeline_p.h"

#include <QtCore/qmath.h>
#include <QtGui/qpainter.h>
#include <QtQuick/private/qquickitem_p.h>

QT_BEGIN_NAMESPACE

class QQuickTimelinePrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(QQuickTimeline)
public:
    QQuickTimelinePrivate()
        : startFrame(0),
          endFrame(0),
          currentFrame(0),
          enabled(false),
          componentComplete(true)
    {
    }

    qreal startFrame;
    qreal endFrame;
    qreal currentFrame;

    bool enabled:1;
    bool componentComplete:1;

protected:
    void init();
    void disable();

    static void append_keyframe(QQmlListProperty<QQuickKeyframeGroup> *list, QQuickKeyframeGroup *a);
    static int keyframe_count(QQmlListProperty<QQuickKeyframeGroup> *list);
    static QQuickKeyframeGroup* keyframe_at(QQmlListProperty<QQuickKeyframeGroup> *list, int pos);
    static void clear_keyframes(QQmlListProperty<QQuickKeyframeGroup> *list);

    static void append_animation(QQmlListProperty<QQuickTimelineAnimation> *list, QQuickTimelineAnimation *a);
    static int animation_count(QQmlListProperty<QQuickTimelineAnimation> *list);
    static QQuickTimelineAnimation* animation_at(QQmlListProperty<QQuickTimelineAnimation> *list, int pos);
    static void clear_animations(QQmlListProperty<QQuickTimelineAnimation> *list);


    QList<QQuickKeyframeGroup *> keyframes;
    QList<QQuickTimelineAnimation *> animations;
};

void QQuickTimelinePrivate::init()
{
    for (auto keyFrames : keyframes) {
        keyFrames->init();
        keyFrames->setProperty(currentFrame);
    }
}

void QQuickTimelinePrivate::disable()
{
    for (auto keyFrames : keyframes)
        keyFrames->resetDefaultValue();
}

void QQuickTimelinePrivate::append_keyframe(QQmlListProperty<QQuickKeyframeGroup> *list, QQuickKeyframeGroup *a)
{
    QQuickTimeline *q = static_cast<QQuickTimeline *>(list->object);
    q->d_func()->keyframes.append(a);
}

int QQuickTimelinePrivate::keyframe_count(QQmlListProperty<QQuickKeyframeGroup> *list)
{
    QQuickTimeline *q = static_cast<QQuickTimeline *>(list->object);
    return q->d_func()->keyframes.count();
}

QQuickKeyframeGroup* QQuickTimelinePrivate::keyframe_at(QQmlListProperty<QQuickKeyframeGroup> *list, int pos)
{
    QQuickTimeline *q = static_cast<QQuickTimeline *>(list->object);
    return q->d_func()->keyframes.at(pos);
}

void QQuickTimelinePrivate::clear_keyframes(QQmlListProperty<QQuickKeyframeGroup> *list)
{
    QQuickTimeline *q = static_cast<QQuickTimeline *>(list->object);
    while (q->d_func()->keyframes.count()) {
        QQuickKeyframeGroup *firstKeyframe = q->d_func()->keyframes.at(0);
        q->d_func()->keyframes.removeAll(firstKeyframe);
    }
}

void QQuickTimelinePrivate::append_animation(QQmlListProperty<QQuickTimelineAnimation> *list, QQuickTimelineAnimation *a)
{
    QQuickTimeline *q = static_cast<QQuickTimeline *>(list->object);
    a->setTargetObject(q);
    q->d_func()->animations.append(a);
}

int QQuickTimelinePrivate::animation_count(QQmlListProperty<QQuickTimelineAnimation> *list)
{
    QQuickTimeline *q = static_cast<QQuickTimeline *>(list->object);
    return q->d_func()->animations.count();
}

QQuickTimelineAnimation* QQuickTimelinePrivate::animation_at(QQmlListProperty<QQuickTimelineAnimation> *list, int pos)
{
    QQuickTimeline *q = static_cast<QQuickTimeline *>(list->object);
    return q->d_func()->animations.at(pos);
}

void QQuickTimelinePrivate::clear_animations(QQmlListProperty<QQuickTimelineAnimation> *list)
{
    QQuickTimeline *q = static_cast<QQuickTimeline *>(list->object);
    while (q->d_func()->animations.count()) {
        QQuickTimelineAnimation *firstAnimation = q->d_func()->animations.at(0);
        q->d_func()->animations.removeAll(firstAnimation);
    }
}

QQuickTimeline::QQuickTimeline(QObject *parent) : QObject(*(new QQuickTimelinePrivate), parent)
{

}

QQmlListProperty<QQuickKeyframeGroup> QQuickTimeline::keyframes()
{
    Q_D(QQuickTimeline);

    return QQmlListProperty<QQuickKeyframeGroup>(this, &d->keyframes, QQuickTimelinePrivate::append_keyframe,
                                                 QQuickTimelinePrivate::keyframe_count,
                                                 QQuickTimelinePrivate::keyframe_at,
                                                 QQuickTimelinePrivate::clear_keyframes);
}

QQmlListProperty<QQuickTimelineAnimation> QQuickTimeline::animations()
{
    Q_D(QQuickTimeline);

    return QQmlListProperty<QQuickTimelineAnimation>(this, &d->animations, QQuickTimelinePrivate::append_animation,
                                                     QQuickTimelinePrivate::animation_count,
                                                     QQuickTimelinePrivate::animation_at,
                                                     QQuickTimelinePrivate::clear_animations);
}

bool QQuickTimeline::enabled() const
{
    Q_D(const QQuickTimeline);
    return d->enabled;
}

void QQuickTimeline::setEnabled(bool b)
{
    Q_D(QQuickTimeline);
    if (d->enabled == b)
        return;
    d->enabled = b;

    if (d->componentComplete) {
        if (b)
            init();
        else
            reset();
    }

    emit enabledChanged();
}

qreal QQuickTimeline::startFrame() const
{
    Q_D(const QQuickTimeline);
    return d->startFrame;
}

void QQuickTimeline::setStartFrame(qreal frame)
{
    Q_D(QQuickTimeline);
    if (d->startFrame == frame)
        return;
    d->startFrame = frame;
    emit startFrameChanged();
}

qreal QQuickTimeline::endFrame() const
{
    Q_D(const QQuickTimeline);
    return d->endFrame;
}

void QQuickTimeline::setEndFrame(qreal frame)
{
    Q_D(QQuickTimeline);
    if (d->endFrame == frame)
        return;
    d->endFrame = frame;
    emit endFrameChanged();
}

qreal QQuickTimeline::currentFrame() const
{
    Q_D(const QQuickTimeline);
    return d->currentFrame;
}

void QQuickTimeline::setCurrentFrame(qreal frame)
{
    Q_D(QQuickTimeline);
    if (d->currentFrame == frame)
        return;
    d->currentFrame = frame;

    if (d->componentComplete && d->enabled)
        for (auto keyFrames : d->keyframes)
            keyFrames->setProperty(d->currentFrame);

    emit currentFrameChanged();
}

void QQuickTimeline::init()
{
    Q_D(QQuickTimeline);

    if (d->componentComplete)
        d->init();
}

void QQuickTimeline::reset()
{
    Q_D(QQuickTimeline);

    if (d->componentComplete)
        d->disable();
}

QList<QQuickTimelineAnimation *> QQuickTimeline::getAnimations() const
{
     Q_D(const QQuickTimeline);

    return d->animations;
}

void QQuickTimeline::classBegin()
{
    Q_D(QQuickTimeline);
    d->componentComplete = false;
}

void QQuickTimeline::componentComplete()
{
    Q_D(QQuickTimeline);
    d->componentComplete = true;

    if (d->enabled)
        init();
}

QT_END_NAMESPACE
