QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    QRibbon/QRibbon.cpp \
    addmember.cpp \
    displaywidget.cpp \
    main.cpp \
    mainwindow.cpp \
    showtree.cpp \
    toolwidget.cpp \
    treedata.cpp

HEADERS += \
    QRibbon/QRibbon.h \
    addmember.h \
    displaywidget.h \
    mainwindow.h \
    showtree.h \
    toolwidget.h \
    treedata.h

FORMS += \
    QRibbon/qribbon.ui \
    addmember.ui \
    displaywidget.ui \
    mainwindow.ui \
    showtree.ui \
    toolwidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    QRibbon/QRibbon.qrc \
    resource.qrc

ICON += ":/resource/logo.webp"
