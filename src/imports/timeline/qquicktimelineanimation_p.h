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
