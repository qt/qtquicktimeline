/******************************************************************************
**
** Copyright (C) 2018 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the Qt Quick Timeline module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:COMM$
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** $QT_END_LICENSE$
**
******************************************************************************/

#ifndef QQUICKKEYFRAME_P_H
#define QQUICKKEYFRAME_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QtCore/QObject>
#include <QtCore/qeasingcurve.h>

#include <QtQml/qqml.h>

QT_BEGIN_NAMESPACE

class QQuickKeyframeGroupPrivate;
class QQuickKeyframePrivate;
class QQuickNumberKeyframePrivate;

class QQuickKeyframe : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QQuickKeyframe)

    Q_PROPERTY(qreal frame READ frame WRITE setFrame NOTIFY frameChanged)
    Q_PROPERTY(QEasingCurve easing READ easing WRITE setEasing NOTIFY easingCurveChanged)
    Q_PROPERTY(QVariant value READ value WRITE setValue NOTIFY valueChanged)

public:
    explicit QQuickKeyframe(QObject *parent = nullptr);

    qreal frame() const;
    void setFrame(qreal);
    void reset();

    QEasingCurve easing() const;
    void setEasing(const QEasingCurve &);

    QVariant value() const;
    void setValue(QVariant);

    virtual QVariant evaluate(QQuickKeyframe *pre, qreal frame, int userType) const;

protected:
    QQuickKeyframe(QQuickKeyframePrivate &dd, QObject *parent);

Q_SIGNALS:
    void frameChanged();
    void easingCurveChanged();
    void valueChanged();
};

class QQuickKeyframeGroup : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QQuickKeyframeGroup)

    Q_INTERFACES(QQmlParserStatus)

    Q_PROPERTY(QObject *target READ target WRITE setTargetObject NOTIFY targetChanged)
    Q_PROPERTY(QString property READ property WRITE setProperty NOTIFY propertyChanged)
    Q_PROPERTY(QQmlListProperty<QQuickKeyframe> keyframes READ keyframes)

    Q_CLASSINFO("DefaultProperty", "keyframes")

public:
    explicit QQuickKeyframeGroup(QObject *parent = nullptr);

    QQmlListProperty<QQuickKeyframe> keyframes();

    QObject *target() const;
    void setTargetObject(QObject *);

    QString property() const;
    void setProperty(const QString &);

    QVariant evaluate(qreal frame) const;

    void setProperty(qreal frame);

    void init();

    void resetDefaultValue();

    void reset();

protected:
    void setupKeyframes();

    void classBegin() override;
    void componentComplete() override;

Q_SIGNALS:
    void targetChanged();
    void propertyChanged();
};

QT_END_NAMESPACE

#endif // QQUICKKEYFRAME_P_H
