#-------------------------------------------------
#
# Project created by QtCreator 2019-04-22T22:50:04
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VoronoiDrawer
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++17

SOURCES += \
    dependencies/src/lapi.c \
    dependencies/src/lauxlib.c \
    dependencies/src/lbaselib.c \
    dependencies/src/lbitlib.c \
    dependencies/src/lcode.c \
    dependencies/src/lcorolib.c \
    dependencies/src/lctype.c \
    dependencies/src/ldblib.c \
    dependencies/src/ldebug.c \
    dependencies/src/ldo.c \
    dependencies/src/ldump.c \
    dependencies/src/lfunc.c \
    dependencies/src/lgc.c \
    dependencies/src/linit.c \
    dependencies/src/liolib.c \
    dependencies/src/llex.c \
    dependencies/src/lmathlib.c \
    dependencies/src/lmem.c \
    dependencies/src/loadlib.c \
    dependencies/src/lobject.c \
    dependencies/src/lopcodes.c \
    dependencies/src/loslib.c \
    dependencies/src/lparser.c \
    dependencies/src/lstate.c \
    dependencies/src/lstring.c \
    dependencies/src/lstrlib.c \
    dependencies/src/ltable.c \
    dependencies/src/ltablib.c \
    dependencies/src/ltm.c \
    dependencies/src/lundump.c \
    dependencies/src/lutf8lib.c \
    dependencies/src/lvm.c \
    dependencies/src/lzio.c \
        main.cpp \
        mainwindow.cpp \
    map.cpp \
    mywidget/mygraphicsellipseitem.cpp \
    mywidget/myscene.cpp \
    dialogs/newmap/newmapdialog.cpp \
    voronoi/FastNoise.cpp \
    voronoi/voronoigen.cpp \
    voronoi/voronoimap.cpp \
    voronoi/sweepline.cpp \
    bst.cpp

HEADERS += \
    dependencies/sol.hpp \
    dependencies/src/lapi.h \
    dependencies/src/lauxlib.h \
    dependencies/src/lcode.h \
    dependencies/src/lctype.h \
    dependencies/src/ldebug.h \
    dependencies/src/ldo.h \
    dependencies/src/lfunc.h \
    dependencies/src/lgc.h \
    dependencies/src/llex.h \
    dependencies/src/llimits.h \
    dependencies/src/lmem.h \
    dependencies/src/lobject.h \
    dependencies/src/lopcodes.h \
    dependencies/src/lparser.h \
    dependencies/src/lprefix.h \
    dependencies/src/lstate.h \
    dependencies/src/lstring.h \
    dependencies/src/ltable.h \
    dependencies/src/ltm.h \
    dependencies/src/lua.h \
    dependencies/src/lua.hpp \
    dependencies/src/luaconf.h \
    dependencies/src/lualib.h \
    dependencies/src/lundump.h \
    dependencies/src/lvm.h \
    dependencies/src/lzio.h \
        mainwindow.h \
    map.h \
    mywidget/mygraphicsellipseitem.h \
    mywidget/myscene.h \
    dialogs/newmap/newmapdialog.h \
    voronoi/FastNoise.h \
    voronoi/json.hpp \
    voronoi/voronoigen.h \
    voronoi/voronoimap.h \
    voronoi/sweepline.h \
    bst.h

INCLUDEPATH += \
    dependencies/src

FORMS += \
        mainwindow.ui \
    dialogs/newmap/newmapdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    src.qrc
