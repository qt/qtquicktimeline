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

#include <QtQml/qqmlextensionplugin.h>

#include "qquickkeyframemutator_p.h"
#include "qquickkeyframe_p.h"

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
    qmlRegisterType<QQuickKeyframeMutator>(uri, 1, 0, "KeyframeMutator");
    qmlRegisterType<QQuickKeyframe>(uri, 1, 0, "Keyframe");
    qmlRegisterType<QQuickKeyframes>(uri, 1, 0, "Keyframes");
}

void QtQuickTimelinePlugin::initializeEngine(QQmlEngine *engine, const char *uri)
{
    QQmlExtensionPlugin::initializeEngine(engine, uri);
}

QT_END_NAMESPACE

#include "qtquicktimelineplugin.moc"
