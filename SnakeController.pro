#-------------------------------------------------
#
# Project created by QtCreator 2015-09-01T13:20:10
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SnakeController
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11

RESOURCES = qtgui/snakecontroller.qrc

SOURCES += \
    dxsystem/dxllibclass/dxlhal.cpp \
    dxsystem/dxllibclass/dynamixelclass.cpp \
    dxsystem/dmodeldetails.cpp \
    dxsystem/dxmotorsystem.cpp \
    dxsystem/dxsetupmotor.cpp \
    dxsystem/dxsystem.cpp \
    dxsystem/additionaltools.cpp \
    dxsystem/setup.cpp \
    dxsystem/userinputfn.cpp \
    src/main.cpp \
    qtgui/controller.cpp \
    qtgui/advgaits.cpp \
    qtgui/simpgaits.cpp \
    qtgui/extendedsliders.cpp \
    qtgui/replica.cpp

HEADERS  += \
    dxsystem/dxllibclass/dxlhal.h \
    dxsystem/dxllibclass/dynamixelclass.h \
    dxsystem/control_table_constants.h \
    dxsystem/dconstants.h \
    dxsystem/dmodeldetails.h \
    dxsystem/dxmotorsystem.h \
    dxsystem/dxsetupmotor.h \
    dxsystem/dxsystem.h \
    dxsystem/additionaltools.h \
    dxsystem/setup.h \
    dxsystem/user_settings.h \
    dxsystem/userinputfn.h \
    qtgui/controller.h \
    qtgui/advgaits.h \
    qtgui/params.h \
    qtgui/simpgaits.h \
    qtgui/extendedsliders.h \
    qtgui/replica.h

FORMS    += \
    qtgui/controller.ui \
    qtgui/advgaits.ui \
    qtgui/simpgaits.ui \
    qtgui/extendedsliders.ui \
    qtgui/replica.ui
