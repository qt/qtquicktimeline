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

#ifndef QQUICKTIMELINEANIMATION_H
#define QQUICKTIMELINEANIMATION_H

#include <QtQuick/private/qquickanimation_p.h>

QT_BEGIN_NAMESPACE

class QQuickTimelineAnimation : public QQuickNumberAnimation
{
    Q_OBJECT

    Q_PROPERTY(bool pingPong READ pingPong WRITE setPingPong NOTIFY pingPongChanged)

public:
    QQuickTimelineAnimation(QObject *parent = nullptr);

    void setPingPong(bool b);
    bool pingPong() const;

Q_SIGNALS:
    void pingPongChanged();
    void finished();

private:
    void handleStarted();
    void handleStopped();

    bool m_pinpong = false;
    bool m_reversed = false;
    bool m_originalStart = true;
    int m_currentLoop = 0;
    int m_originalLoop = 0;
};

QT_END_NAMESPACE

#endif // QQUICKTIMELINEANIMATION_H
