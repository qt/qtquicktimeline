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

#include "qquickkeyframemutator_p.h"

#include <QtCore/qmath.h>
#include <QtGui/qpainter.h>
#include <QtQuick/private/qquickitem_p.h>

QT_BEGIN_NAMESPACE

class QQuickKeyframeMutatorPrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(QQuickKeyframeMutator)
public:
    QQuickKeyframeMutatorPrivate()
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

    static void append_keyframe(QQmlListProperty<QQuickKeyframes> *list, QQuickKeyframes *a);
    static int keyframe_count(QQmlListProperty<QQuickKeyframes> *list);
    static QQuickKeyframes* keyframe_at(QQmlListProperty<QQuickKeyframes> *list, int pos);
    static void clear_keyframes(QQmlListProperty<QQuickKeyframes> *list);

    QList<QQuickKeyframes *> keyframes;
};

void QQuickKeyframeMutatorPrivate::init()
{
    for (auto keyFrames : keyframes) {
        keyFrames->init();
        keyFrames->setProperty(currentFrame);
    }
}

void QQuickKeyframeMutatorPrivate::disable()
{
    for (auto keyFrames : keyframes)
        keyFrames->resetDefaultValue();
}

void QQuickKeyframeMutatorPrivate::append_keyframe(QQmlListProperty<QQuickKeyframes> *list, QQuickKeyframes *a)
{
    QQuickKeyframeMutator *q = static_cast<QQuickKeyframeMutator *>(list->object);
    q->d_func()->keyframes.append(a);
}

int QQuickKeyframeMutatorPrivate::keyframe_count(QQmlListProperty<QQuickKeyframes> *list)
{
    QQuickKeyframeMutator *q = static_cast<QQuickKeyframeMutator *>(list->object);
    return q->d_func()->keyframes.count();
}

QQuickKeyframes* QQuickKeyframeMutatorPrivate::keyframe_at(QQmlListProperty<QQuickKeyframes> *list, int pos)
{
    QQuickKeyframeMutator *q = static_cast<QQuickKeyframeMutator *>(list->object);
    return q->d_func()->keyframes.at(pos);
}

void QQuickKeyframeMutatorPrivate::clear_keyframes(QQmlListProperty<QQuickKeyframes> *list)
{
    QQuickKeyframeMutator *q = static_cast<QQuickKeyframeMutator *>(list->object);
    while (q->d_func()->keyframes.count()) {
        QQuickKeyframes *firstKeyframe = q->d_func()->keyframes.at(0);
        q->d_func()->keyframes.removeAll(firstKeyframe);
    }
}

QQuickKeyframeMutator::QQuickKeyframeMutator(QObject *parent) : QObject(*(new QQuickKeyframeMutatorPrivate), parent)
{

}

QQmlListProperty<QQuickKeyframes> QQuickKeyframeMutator::keyframes()
{
    Q_D(QQuickKeyframeMutator);

    return QQmlListProperty<QQuickKeyframes>(this, &d->keyframes, QQuickKeyframeMutatorPrivate::append_keyframe,
                                             QQuickKeyframeMutatorPrivate::keyframe_count,
                                             QQuickKeyframeMutatorPrivate::keyframe_at,
                                             QQuickKeyframeMutatorPrivate::clear_keyframes);
}

bool QQuickKeyframeMutator::enabled() const
{
    Q_D(const QQuickKeyframeMutator);
    return d->enabled;
}

void QQuickKeyframeMutator::setEnabled(bool b)
{
    Q_D(QQuickKeyframeMutator);
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

qreal QQuickKeyframeMutator::startFrame() const
{
    Q_D(const QQuickKeyframeMutator);
    return d->startFrame;
}

void QQuickKeyframeMutator::setStartFrame(qreal frame)
{
    Q_D(QQuickKeyframeMutator);
    if (d->startFrame == frame)
        return;
    d->startFrame = frame;
    emit startFrameChanged();
}

qreal QQuickKeyframeMutator::endFrame() const
{
    Q_D(const QQuickKeyframeMutator);
    return d->endFrame;
}

void QQuickKeyframeMutator::setEndFrame(qreal frame)
{
    Q_D(QQuickKeyframeMutator);
    if (d->endFrame == frame)
        return;
    d->endFrame = frame;
    emit endFrameChanged();
}

qreal QQuickKeyframeMutator::currentFrame() const
{
    Q_D(const QQuickKeyframeMutator);
    return d->currentFrame;
}

void QQuickKeyframeMutator::setCurrentFrame(qreal frame)
{
    Q_D(QQuickKeyframeMutator);
    if (d->currentFrame == frame)
        return;
    d->currentFrame = frame;

    if (d->componentComplete && d->enabled)
        for (auto keyFrames : d->keyframes)
            keyFrames->setProperty(d->currentFrame);

    emit currentFrameChanged();
}

void QQuickKeyframeMutator::init()
{
    Q_D(QQuickKeyframeMutator);

    if (d->componentComplete)
        d->init();
}

void QQuickKeyframeMutator::reset()
{
    Q_D(QQuickKeyframeMutator);

    if (d->componentComplete)
        d->disable();
}

void QQuickKeyframeMutator::classBegin()
{
    Q_D(QQuickKeyframeMutator);
    d->componentComplete = false;
}

void QQuickKeyframeMutator::componentComplete()
{
    Q_D(QQuickKeyframeMutator);
    d->componentComplete = true;

    if (d->enabled)
        init();
}

QT_END_NAMESPACE
