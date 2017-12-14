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

#include "qquickkeyframe_p.h"

#include "qquickkeyframemutator_p.h"

#include <QtCore/QVariantAnimation>
#include <QtCore/qmath.h>
#include <QtGui/qpainter.h>
#include <QtQuick/private/qquickitem_p.h>
#include <QtQml/QQmlProperty>

#include <private/qvariantanimation_p.h>

#include <algorithm>

QT_BEGIN_NAMESPACE

class QQuickKeyframesPrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(QQuickKeyframes)
public:
    QQuickKeyframesPrivate()
        : target(nullptr),
          componentComplete(false)
    {
    }

    QObject *target;
    QString propertyName;
    bool componentComplete;

protected:
    void setupKeyframes();

    static void append_keyframe(QQmlListProperty<QQuickKeyframe> *list, QQuickKeyframe *a);
    static int keyframe_count(QQmlListProperty<QQuickKeyframe> *list);
    static QQuickKeyframe* keyframe_at(QQmlListProperty<QQuickKeyframe> *list, int pos);
    static void clear_keyframes(QQmlListProperty<QQuickKeyframe> *list);

    QList<QQuickKeyframe *> keyframes;
    QList<QQuickKeyframe *> sortedKeyframes;

    QVariant originalValue;
};

void QQuickKeyframesPrivate::setupKeyframes()
{
    sortedKeyframes = keyframes;
    std::sort(sortedKeyframes.begin(), sortedKeyframes.end(), [](const QQuickKeyframe *first, const QQuickKeyframe *second) {
        return first->frame() < second->frame();
    });
}

void QQuickKeyframesPrivate::append_keyframe(QQmlListProperty<QQuickKeyframe> *list, QQuickKeyframe *a)
{
    QQuickKeyframes *q = static_cast<QQuickKeyframes *>(list->object);
    q->d_func()->keyframes.append(a);
    q->d_func()->setupKeyframes();
    q->reset();
}

int QQuickKeyframesPrivate::keyframe_count(QQmlListProperty<QQuickKeyframe> *list)
{
    QQuickKeyframes *q = static_cast<QQuickKeyframes *>(list->object);
    return q->d_func()->keyframes.count();
}

QQuickKeyframe* QQuickKeyframesPrivate::keyframe_at(QQmlListProperty<QQuickKeyframe> *list, int pos)
{
    QQuickKeyframes *q = static_cast<QQuickKeyframes *>(list->object);
    return q->d_func()->keyframes.at(pos);
}

void QQuickKeyframesPrivate::clear_keyframes(QQmlListProperty<QQuickKeyframe> *list)
{
    QQuickKeyframes *q = static_cast<QQuickKeyframes *>(list->object);
    while (q->d_func()->keyframes.count()) {
        QQuickKeyframe *firstKeyframe = q->d_func()->keyframes.at(0);
        q->d_func()->keyframes.removeAll(firstKeyframe);
    }
}

class QQuickKeyframePrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(QQuickKeyframe)
public:
    QQuickKeyframePrivate()
        : frame(0)
    {
    }

    qreal frame;
    QEasingCurve easingCurve;
    QVariant value;
};

QQuickKeyframe::QQuickKeyframe(QObject *parent)
    : QObject(*(new QQuickKeyframePrivate), parent)
{

}

qreal QQuickKeyframe::frame() const
{
    Q_D(const QQuickKeyframe);
    return d->frame;
}

void QQuickKeyframe::setFrame(qreal f)
{
    Q_D(QQuickKeyframe);
    if (d->frame == f)
        return;
    d->frame = f;
    emit frameChanged();
}

void QQuickKeyframe::reset()
{
    QQuickKeyframes *keyframes = qobject_cast<QQuickKeyframes*>(parent());
    if (keyframes)
        keyframes->reset();
}

QQuickKeyframe::QQuickKeyframe(QQuickKeyframePrivate &dd, QObject *parent)
    : QObject(dd, parent)
{

}

QQuickKeyframes::QQuickKeyframes(QObject *parent)
    : QObject(*(new QQuickKeyframesPrivate), parent)
{

}

QQmlListProperty<QQuickKeyframe> QQuickKeyframes::keyframes()
{
    Q_D(QQuickKeyframes);

    return QQmlListProperty<QQuickKeyframe>(this, &d->keyframes, QQuickKeyframesPrivate::append_keyframe,
                                            QQuickKeyframesPrivate::keyframe_count,
                                            QQuickKeyframesPrivate::keyframe_at,
                                            QQuickKeyframesPrivate::clear_keyframes);
}

QObject *QQuickKeyframes::target() const
{
    Q_D(const QQuickKeyframes);
    return d->target;
}

void QQuickKeyframes::setTargetObject(QObject *o)
{
    Q_D(QQuickKeyframes);
    if (d->target == o)
        return;
    d->target = o;

    if (!property().isEmpty())
        init();

    emit targetChanged();
}

QString QQuickKeyframes::property() const
{
    Q_D(const QQuickKeyframes);
    return d->propertyName;
}

void QQuickKeyframes::setProperty(const QString &n)
{
    Q_D(QQuickKeyframes);
    if (d->propertyName == n)
        return;
    d->propertyName = n;

    if (target())
        init();

    emit propertyChanged();
}

QVariant QQuickKeyframes::evaluate(qreal frame) const
{
    Q_D(const QQuickKeyframes);

    if (d->sortedKeyframes.isEmpty())
        return QVariant();

    static QQuickKeyframe dummy;
    QQuickKeyframeMutator *timeline = qobject_cast<QQuickKeyframeMutator*>(parent());
    if (timeline)
        dummy.setFrame(timeline->startFrame() - 0.0001);
    dummy.setValue(d->originalValue);

     QQuickKeyframe *lastFrame = &dummy;

    for (auto keyFrame :  qAsConst(d->sortedKeyframes)) {
        if (qFuzzyCompare(frame, keyFrame->frame()) || frame < keyFrame->frame())
            return keyFrame->evaluate(lastFrame, frame, QQmlProperty(target(), property()).property().userType());
        lastFrame = keyFrame;
    }

    return lastFrame->value();
}

void QQuickKeyframes::setProperty(qreal frame)
{
    if (target()) {
        QQmlProperty qmlProperty(target(), property());

        qmlProperty.write(evaluate(frame));
    }
}

void QQuickKeyframes::init()
{
    Q_D(QQuickKeyframes);
    if (target())
        d->originalValue = QQmlProperty::read(target(), property());
}

void QQuickKeyframes::resetDefaultValue()
{
    Q_D(QQuickKeyframes);
    QQmlProperty::write(target(), property(), d->originalValue);
}

void QQuickKeyframes::reset()
{
    Q_D(QQuickKeyframes);
    if (!d->componentComplete)
        return;

    QQuickKeyframeMutator *mutator = qobject_cast<QQuickKeyframeMutator*>(parent());
    if (mutator)
        setProperty(mutator->currentFrame());
}

void QQuickKeyframes::setupKeyframes()
{
    Q_D(QQuickKeyframes);

    if (d->componentComplete)
        d->setupKeyframes();
}

void QQuickKeyframes::classBegin()
{
    Q_D(QQuickKeyframes);
    d->componentComplete = false;
}

void QQuickKeyframes::componentComplete()
{
    Q_D(QQuickKeyframes);
    d->componentComplete = true;
    setupKeyframes();
}

QEasingCurve QQuickKeyframe::easing() const
{
    Q_D(const QQuickKeyframe);
    return d->easingCurve;
}

void QQuickKeyframe::setEasing(const QEasingCurve &e)
{
    Q_D(QQuickKeyframe);
    if (d->easingCurve == e)
        return;

    d->easingCurve = e;
    emit easingCurveChanged();
}

QVariant QQuickKeyframe::value() const
{
    Q_D(const QQuickKeyframe);
    return d->value;
}

void QQuickKeyframe::setValue(QVariant v)
{
    Q_D(QQuickKeyframe);
    if (d->value == v)
        return;
    d->value = v;

    reset();

    emit valueChanged();
}

QVariant QQuickKeyframe::evaluate(QQuickKeyframe *pre, qreal frametime, int userType) const
{
    QVariantAnimation::Interpolator interpolator = QVariantAnimationPrivate::getInterpolator(userType);
    if (!pre)
        return value();

    QVariant preValue = pre->value();
    qreal preFrame = pre->frame();

    qreal duration = frame() - preFrame;
    qreal offset = frametime - preFrame;

    qreal progress = easing().valueForProgress(offset / duration);

    preValue.convert(userType);
    QVariant convertedValue = value();
    convertedValue.convert(userType);

    return interpolator(preValue.constData(), convertedValue.constData(), progress);
}

QT_END_NAMESPACE


