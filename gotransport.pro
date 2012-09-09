#-------------------------------------------------
#
# Project created by QtCreator
#
#-------------------------------------------------

QT += core gui network xml

maemo5 {
    QT += maemo5
}

CONFIG += mobility
MOBILITY += location

TARGET = gotransport
TEMPLATE = app

SOURCES += main.cpp mainwindow.cpp \
    qdatetimeediturl.cpp \
    qsliderurl.cpp \
    qcomboboxurl.cpp \
    position.cpp \
    tflxmlhandler.cpp \
    routeviewer.cpp \
    routedatagen.cpp \
    routemodel.cpp \
    routetreeview.cpp \
    routedelegate.cpp
HEADERS += mainwindow.h \
    qdatetimeediturl.h \
    qsliderurl.h \
    qcomboboxurl.h \
    position.h \
    tflxmlhandler.h \
    routeviewer.h \
    routedatagen.h \
    routemodel.h \
    routetreeview.h \
    routedelegate.h
FORMS += mainwindow.ui \
    routeviewer.ui

RESOURCES += \
    resources.qrc

OTHER_FILES += \
    qtc_packaging/debian_fremantle/rules \
    qtc_packaging/debian_fremantle/README \
    qtc_packaging/debian_fremantle/copyright \
    qtc_packaging/debian_fremantle/control \
    qtc_packaging/debian_fremantle/compat \
    qtc_packaging/debian_fremantle/changelog

maemo5 {
    target.path = /opt/gotransport/bin
    INSTALLS += target
}

maemo5 {
    icon.files = gotransport.png
    icon.path = /usr/share/icons/hicolor/64x64/apps
    INSTALLS += icon
}

maemo5 {
    desktopfile.files = gotransport.desktop
    desktopfile.path = /usr/share/applications/hildon
    INSTALLS += desktopfile
}
