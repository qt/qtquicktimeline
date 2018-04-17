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

    QList<QQuickKeyframeGroup *> keyframes;
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
