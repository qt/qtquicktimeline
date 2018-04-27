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

#include "qquicktimeline_p.h"

#include <QtCore/QVariantAnimation>
#include <QtCore/qmath.h>
#include <QtGui/qpainter.h>
#include <QtQuick/private/qquickitem_p.h>
#include <QtQml/QQmlProperty>

#include <private/qvariantanimation_p.h>

#include <algorithm>

QT_BEGIN_NAMESPACE

class QQuickKeyframeGroupPrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(QQuickKeyframeGroup)
public:
    QQuickKeyframeGroupPrivate()
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

void QQuickKeyframeGroupPrivate::setupKeyframes()
{
    sortedKeyframes = keyframes;
    std::sort(sortedKeyframes.begin(), sortedKeyframes.end(), [](const QQuickKeyframe *first, const QQuickKeyframe *second) {
        return first->frame() < second->frame();
    });
}

void QQuickKeyframeGroupPrivate::append_keyframe(QQmlListProperty<QQuickKeyframe> *list, QQuickKeyframe *a)
{
    QQuickKeyframeGroup *q = static_cast<QQuickKeyframeGroup *>(list->object);
    q->d_func()->keyframes.append(a);
    q->d_func()->setupKeyframes();
    q->reset();
}

int QQuickKeyframeGroupPrivate::keyframe_count(QQmlListProperty<QQuickKeyframe> *list)
{
    QQuickKeyframeGroup *q = static_cast<QQuickKeyframeGroup *>(list->object);
    return q->d_func()->keyframes.count();
}

QQuickKeyframe* QQuickKeyframeGroupPrivate::keyframe_at(QQmlListProperty<QQuickKeyframe> *list, int pos)
{
    QQuickKeyframeGroup *q = static_cast<QQuickKeyframeGroup *>(list->object);
    return q->d_func()->keyframes.at(pos);
}

void QQuickKeyframeGroupPrivate::clear_keyframes(QQmlListProperty<QQuickKeyframe> *list)
{
    QQuickKeyframeGroup *q = static_cast<QQuickKeyframeGroup *>(list->object);
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
    QQuickKeyframeGroup *keyframes = qobject_cast<QQuickKeyframeGroup*>(parent());
    if (keyframes)
        keyframes->reset();
}

QQuickKeyframe::QQuickKeyframe(QQuickKeyframePrivate &dd, QObject *parent)
    : QObject(dd, parent)
{

}

QQuickKeyframeGroup::QQuickKeyframeGroup(QObject *parent)
    : QObject(*(new QQuickKeyframeGroupPrivate), parent)
{

}

QQmlListProperty<QQuickKeyframe> QQuickKeyframeGroup::keyframes()
{
    Q_D(QQuickKeyframeGroup);

    return QQmlListProperty<QQuickKeyframe>(this, &d->keyframes, QQuickKeyframeGroupPrivate::append_keyframe,
                                            QQuickKeyframeGroupPrivate::keyframe_count,
                                            QQuickKeyframeGroupPrivate::keyframe_at,
                                            QQuickKeyframeGroupPrivate::clear_keyframes);
}

QObject *QQuickKeyframeGroup::target() const
{
    Q_D(const QQuickKeyframeGroup);
    return d->target;
}

void QQuickKeyframeGroup::setTargetObject(QObject *o)
{
    Q_D(QQuickKeyframeGroup);
    if (d->target == o)
        return;
    d->target = o;

    if (!property().isEmpty())
        init();

    emit targetChanged();
}

QString QQuickKeyframeGroup::property() const
{
    Q_D(const QQuickKeyframeGroup);
    return d->propertyName;
}

void QQuickKeyframeGroup::setProperty(const QString &n)
{
    Q_D(QQuickKeyframeGroup);
    if (d->propertyName == n)
        return;
    d->propertyName = n;

    if (target())
        init();

    emit propertyChanged();
}

QVariant QQuickKeyframeGroup::evaluate(qreal frame) const
{
    Q_D(const QQuickKeyframeGroup);

    if (d->sortedKeyframes.isEmpty())
        return QVariant();

    static QQuickKeyframe dummy;
    QQuickTimeline *timeline = qobject_cast<QQuickTimeline*>(parent());
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

void QQuickKeyframeGroup::setProperty(qreal frame)
{
    if (target()) {
        QQmlProperty qmlProperty(target(), property());

        qmlProperty.write(evaluate(frame));
    }
}

void QQuickKeyframeGroup::init()
{
    Q_D(QQuickKeyframeGroup);
    if (target())
        d->originalValue = QQmlProperty::read(target(), property());
}

void QQuickKeyframeGroup::resetDefaultValue()
{
    Q_D(QQuickKeyframeGroup);
    QQmlProperty::write(target(), property(), d->originalValue);
}

void QQuickKeyframeGroup::reset()
{
    Q_D(QQuickKeyframeGroup);
    if (!d->componentComplete)
        return;

    auto *timeline = qobject_cast<QQuickTimeline*>(parent());
    if (timeline)
        setProperty(timeline->currentFrame());
}

void QQuickKeyframeGroup::setupKeyframes()
{
    Q_D(QQuickKeyframeGroup);

    if (d->componentComplete)
        d->setupKeyframes();
}

void QQuickKeyframeGroup::classBegin()
{
    Q_D(QQuickKeyframeGroup);
    d->componentComplete = false;
}

void QQuickKeyframeGroup::componentComplete()
{
    Q_D(QQuickKeyframeGroup);
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

    if (preValue.isValid() && convertedValue.isValid())
        return interpolator(preValue.constData(), convertedValue.constData(), progress);

    qWarning() << "invalid keyframe target";

    return QVariant();
}

QT_END_NAMESPACE


