# Generated from timeline.pro.

#####################################################################
## qtquicktimelineplugin Plugin:
#####################################################################

qt_internal_add_qml_module(qtquicktimelineplugin
    URI "QtQuick.Timeline"
    VERSION "${PROJECT_VERSION}"
    DESIGNER_SUPPORTED
    CLASSNAME QtQuickTimelinePlugin
    SKIP_TYPE_REGISTRATION
    PLUGIN_OPTIONAL
    DEPENDENCIES
        QtQuick
    SOURCES
        qtquicktimelineplugin.cpp
    DEFINES
        QT_NO_CAST_FROM_ASCII
        QT_NO_CAST_TO_ASCII
    LIBRARIES
        Qt::Qml
        Qt::QuickTimelinePrivate
)

#### Keys ignored in scope 1:.:.:timeline.pro:<TRUE>:
# DISTFILES = "qmldir" "plugins.qmltypes"
# IMPORT_VERSION = "1.0"
# TARGETPATH = "QtQuick/Timeline"
qt_internal_add_docs(qtquicktimelineplugin
    doc/qtquicktimeline.qdocconf
)

