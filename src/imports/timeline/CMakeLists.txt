# Generated from timeline.pro.

#####################################################################
## qtquicktimelineplugin Plugin:
#####################################################################

qt_internal_add_qml_module(qtquicktimelineplugin
    URI "QtQuick.Timeline"
    VERSION "1.0"
    DESIGNER_SUPPORTED
    CLASSNAME QtQuickTimelinePlugin
    SKIP_TYPE_REGISTRATION
    SOURCES
        keyframedatautils_p.h
        qquickkeyframe.cpp qquickkeyframe_p.h
        qquicktimeline.cpp qquicktimeline_p.h
        qquicktimelineanimation.cpp qquicktimelineanimation_p.h
        qtquicktimelineplugin.cpp
    DEFINES
        QT_NO_CAST_FROM_ASCII
        QT_NO_CAST_TO_ASCII
    LIBRARIES
        Qt::CorePrivate
        Qt::GuiPrivate
        Qt::QmlPrivate
        Qt::QuickPrivate
    PUBLIC_LIBRARIES
        Qt::Core
        Qt::CorePrivate
        Qt::Gui
        Qt::Qml
        Qt::Quick
)

#### Keys ignored in scope 1:.:.:timeline.pro:<TRUE>:
# DISTFILES = "qmldir" "plugins.qmltypes"
# IMPORT_VERSION = "1.0"
# TARGETPATH = "QtQuick/Timeline"
qt_internal_add_docs(qtquicktimelineplugin
    doc/qtquicktimeline.qdocconf
)

