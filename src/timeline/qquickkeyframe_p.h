/****************************************************************************
**
** Copyright (C) 2019 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of Qt Quick Designer Components.
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

#include "qtquicktimelineglobal_p.h"

#include <QtCore/QObject>
#include <QtCore/qeasingcurve.h>

#include <QtQml/qqml.h>

QT_BEGIN_NAMESPACE

class QQuickKeyframeGroupPrivate;
class QQuickKeyframePrivate;
class QQuickNumberKeyframePrivate;

class Q_QUICK_TIMELINE_PRIVATE_EXPORT QQuickKeyframe : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QQuickKeyframe)

    Q_PROPERTY(qreal frame READ frame WRITE setFrame NOTIFY frameChanged)
    Q_PROPERTY(QEasingCurve easing READ easing WRITE setEasing NOTIFY easingCurveChanged)
    Q_PROPERTY(QVariant value READ value WRITE setValue NOTIFY valueChanged)

    QML_NAMED_ELEMENT(Keyframe)
    QML_ADDED_IN_VERSION(1, 0)

public:
    explicit QQuickKeyframe(QObject *parent = nullptr);

    qreal frame() const;
    void setFrame(qreal);
    void reset();

    QEasingCurve easing() const;
    void setEasing(const QEasingCurve &);

    QVariant value() const;
    void setValue(const QVariant &v);

    virtual QVariant evaluate(QQuickKeyframe *pre, qreal frame, int userType) const;

protected:
    QQuickKeyframe(QQuickKeyframePrivate &dd, QObject *parent);

Q_SIGNALS:
    void frameChanged();
    void easingCurveChanged();
    void valueChanged();
};

class Q_QUICK_TIMELINE_PRIVATE_EXPORT QQuickKeyframeGroup : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QQuickKeyframeGroup)

    Q_INTERFACES(QQmlParserStatus)

    Q_PROPERTY(QObject *target READ target WRITE setTargetObject NOTIFY targetChanged)
    Q_PROPERTY(QString property READ property WRITE setProperty NOTIFY propertyChanged)
    Q_PROPERTY(QQmlListProperty<QQuickKeyframe> keyframes READ keyframes)
    Q_PROPERTY(QUrl keyframeSource READ keyframeSource WRITE setKeyframeSource NOTIFY keyframeSourceChanged REVISION(1, 1))

    QML_NAMED_ELEMENT(KeyframeGroup)
    QML_ADDED_IN_VERSION(1, 0)

    Q_CLASSINFO("DefaultProperty", "keyframes")

public:
    explicit QQuickKeyframeGroup(QObject *parent = nullptr);

    QQmlListProperty<QQuickKeyframe> keyframes();

    QObject *target() const;
    void setTargetObject(QObject *);

    QString property() const;
    void setProperty(const QString &);

    Q_REVISION(1, 1) QUrl keyframeSource() const;
    Q_REVISION(1, 1) void setKeyframeSource(const QUrl &source);

    const QByteArray keyframeData() const;
    void setKeyframeData(const QByteArray &data);

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
    Q_REVISION(1, 1) void keyframeSourceChanged();
};

QT_END_NAMESPACE

#endif // QQUICKKEYFRAME_P_H
