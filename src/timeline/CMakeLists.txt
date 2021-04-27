qt_internal_add_module(QuickTimeline
    SOURCES
        qquickkeyframedatautils_p.h
        qquickkeyframe.cpp qquickkeyframe_p.h
        qquicktimeline.cpp qquicktimeline_p.h
        qquicktimelineanimation.cpp qquicktimelineanimation_p.h
        qtquicktimelineglobal.h qtquicktimelineglobal_p.h
    DEFINES
        QT_BUILD_QUICK_TIMELINE_LIB
    PUBLIC_LIBRARIES
        Qt::Core
        Qt::Qml
        Qt::Quick
    LIBRARIES
        Qt::QuickPrivate
)

set_target_properties(QuickTimeline PROPERTIES
    QT_QML_MODULE_INSTALL_QMLTYPES TRUE
    QT_QML_MODULE_VERSION "${PROJECT_VERSION}"
    QT_QML_MODULE_URI QtQuick.Timeline
    QT_QMLTYPES_FILENAME plugins.qmltypes
    QT_QML_MODULE_INSTALL_DIR "${INSTALL_QMLDIR}/QtQuick/Timeline"
)

qt6_qml_type_registration(QuickTimeline)
