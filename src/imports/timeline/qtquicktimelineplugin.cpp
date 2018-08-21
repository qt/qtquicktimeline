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

#include <QtQml/qqmlextensionplugin.h>

#include "qquickkeyframe_p.h"
#include "qquicktimeline_p.h"
#include "qquicktimelineanimation_p.h"

static inline void initResources()
{
    Q_INIT_RESOURCE(qtquicktimelineplugin);
#ifdef QT_STATIC
    Q_INIT_RESOURCE(qmake_QtQuick_Controls_2);
#endif
}

QT_BEGIN_NAMESPACE

class QtQuickTimelinePlugin: public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)

public:
    QtQuickTimelinePlugin(QObject *parent = nullptr);
    void registerTypes(const char *uri) override;
    void initializeEngine(QQmlEngine *engine, const char *uri) override;
};

QtQuickTimelinePlugin::QtQuickTimelinePlugin(QObject *parent) : QQmlExtensionPlugin(parent)
{
    initResources();
}

void QtQuickTimelinePlugin::registerTypes(const char *uri)
{
    qmlRegisterType<QQuickTimeline>(uri, 1, 0, "Timeline");
    qmlRegisterType<QQuickTimelineAnimation>(uri, 1, 0, "TimelineAnimation");
    qmlRegisterType<QQuickKeyframe>(uri, 1, 0, "Keyframe");
    qmlRegisterType<QQuickKeyframeGroup>(uri, 1, 0, "KeyframeGroup");
}

void QtQuickTimelinePlugin::initializeEngine(QQmlEngine *engine, const char *uri)
{
    QQmlExtensionPlugin::initializeEngine(engine, uri);
}

QT_END_NAMESPACE

#include "qtquicktimelineplugin.moc"
