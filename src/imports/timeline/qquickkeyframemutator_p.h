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

#ifndef QQUICKKEYFRAMEMUTATOR_P_H
#define QQUICKKEYFRAMEMUTATOR_P_H

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

#include "qquickkeyframe_p.h"

#include <QtQml/qqml.h>

QT_BEGIN_NAMESPACE

class QQuickKeyframeMutatorPrivate;

class QQuickKeyframeMutator : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QQuickKeyframeMutator)

    Q_INTERFACES(QQmlParserStatus)

    Q_PROPERTY(qreal startFrame READ startFrame WRITE setStartFrame  NOTIFY startFrameChanged)
    Q_PROPERTY(qreal endFrame READ endFrame WRITE setEndFrame  NOTIFY endFrameChanged)
    Q_PROPERTY(qreal currentFrame READ currentFrame WRITE setCurrentFrame  NOTIFY currentFrameChanged)
    Q_PROPERTY(QQmlListProperty<QQuickKeyframes> keyframes READ keyframes)
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged)

    Q_CLASSINFO("DefaultProperty", "keyframes")

public:
    explicit QQuickKeyframeMutator(QObject *parent = nullptr);

    QQmlListProperty<QQuickKeyframes> keyframes();

    bool enabled() const;
    void setEnabled(bool enabled);

    qreal startFrame() const;
    void setStartFrame(qreal);

    qreal endFrame() const;
    void setEndFrame(qreal);

    qreal currentFrame() const;
    void setCurrentFrame(qreal);

    void init();
    void reset();

protected:
    void classBegin() override;
    void componentComplete() override;

Q_SIGNALS:
    void enabledChanged();
    void startFrameChanged();
    void endFrameChanged();
    void currentFrameChanged();
};

QT_END_NAMESPACE

#endif // QQUICKKEYFRAMEMUTATOR_P_H
