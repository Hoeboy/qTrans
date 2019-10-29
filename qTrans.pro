#-------------------------------------------------
#
# Project created by QtCreator 2018-12-25T18:57:02
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qTrans
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

CONFIG += c++11

SOURCES += \
    dialogchangeangle.cpp \
    dialogchangezone.cpp \
    dialogcoordinatedirectsolution.cpp \
    dialogcoordinateinvertsolution.cpp \
    dialogcoordinatetokml.cpp \
        main.cpp \
    mainwindow.cpp \
    qtranswindow.cpp \
    SurveyMath/CoordinateTransformation.cpp \
    SurveyMath/SurveyMath.cpp \
    sevenparameterdialog.cpp \
    sevenparameterhassistant.cpp \
    viewsevenparameterdialog.cpp

HEADERS += \
    dialogchangeangle.h \
    dialogchangezone.h \
    dialogcoordinatedirectsolution.h \
    dialogcoordinateinvertsolution.h \
    dialogcoordinatetokml.h \
    mainwindow.h \
        qtranswindow.h \
    SurveyMath/BasePoint.h \
    SurveyMath/CoordinateTransformation.h \
    SurveyMath/Ellipsoid.h \
    SurveyMath/Enumerate.h \
    SurveyMath/Projection.h \
    SurveyMath/SevenParameter.h \
    SurveyMath/SurveyMath.h \
    sevenparameterdialog.h \
    sevenparameterhassistant.h \
    SurveyPoint.h \
    viewsevenparameterdialog.h

FORMS += \
    dialogchangeangle.ui \
    dialogchangezone.ui \
    dialogcoordinatedirectsolution.ui \
    dialogcoordinateinvertsolution.ui \
    dialogcoordinatetokml.ui \
    dialogkmltocoordinate.ui \
    mainwindow.ui \
    qtranswindow.ui \
    sevenparameterdialog.ui \
    viewsevenparameterdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target



INCLUDEPATH += D:/libkml/include
DEPENDPATH += D:/libkml/include
LIBS += -LD:/libkml/lib/ -llibkmlbase \
    -llibkmlconvenience \
    -llibkmldom \
    -llibkmlengine \
    -llibkmlregionator \
    -llibkmlxsd \
    -lminizip \
    -luriparser \
    -lzlib \
    -lexpat
