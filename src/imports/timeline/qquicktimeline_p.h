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

#ifndef QQUICKTIMELINE_P_H
#define QQUICKTIMELINE_P_H

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
#include "qquicktimelineanimation_p.h"

#include <QtQml/qqml.h>

QT_BEGIN_NAMESPACE

class QQuickTimelinePrivate;

class QQuickTimeline : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QQuickTimeline)

    Q_INTERFACES(QQmlParserStatus)

    Q_PROPERTY(qreal startFrame READ startFrame WRITE setStartFrame  NOTIFY startFrameChanged)
    Q_PROPERTY(qreal endFrame READ endFrame WRITE setEndFrame  NOTIFY endFrameChanged)
    Q_PROPERTY(qreal currentFrame READ currentFrame WRITE setCurrentFrame  NOTIFY currentFrameChanged)
    Q_PROPERTY(QQmlListProperty<QQuickKeyframeGroup> keyframes READ keyframes)
    Q_PROPERTY(QQmlListProperty<QQuickTimelineAnimation> animations READ animations)
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged)

    Q_CLASSINFO("DefaultProperty", "keyframes")

public:
    explicit QQuickTimeline(QObject *parent = nullptr);

    QQmlListProperty<QQuickKeyframeGroup> keyframes();
    QQmlListProperty<QQuickTimelineAnimation> animations();

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

    QList<QQuickTimelineAnimation*> getAnimations() const;

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

#endif // QQUICKTIMELINE_P_H
