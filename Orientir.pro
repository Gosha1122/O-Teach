QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    linerpoint.cpp \
    main.cpp \
    mapcontrolpoint.cpp \
    mapcropborderitem.cpp \
    mapiconbutton.cpp \
    mapitem.cpp \
    maplinekp.cpp \
    mapscene.cpp \
    mapsettings.cpp \
    mapview.cpp \
    poliline.cpp \
    stylehelper.cpp \
    widget.cpp

HEADERS += \
    defines.h \
    linerpoint.h \
    mapcontrolpoint.h \
    mapcropborderitem.h \
    mapiconbutton.h \
    mapitem.h \
    maplinekp.h \
    mapscene.h \
    mapsettings.h \
    mapview.h \
    poliline.h \
    stylehelper.h \
    widget.h

FORMS += \
    mapiconbutton.ui \
    mapsettings.ui \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resourses.qrc
