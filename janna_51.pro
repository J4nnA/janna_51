QT       += core gui
QT += core gui widgets printsupport
QT       += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Device.cpp \
    Service.cpp \
    main.cpp \
    mainwindow.cpp \
    myserialport.cpp \
    newserialport.cpp \
    plotwindow.cpp \
    qcustomplot.cpp \
    serialport.cpp

HEADERS += \
    Device.h \
    Service.h \
    mainwindow.h \
    myserialport.h \
    newserialport.h \
    plotwindow.h \
    qcustomplot.h \
    serialport.h

FORMS += \
    mainwindow.ui \
    plotwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32: LIBS += -L$$PWD/./ -lvisa64

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.
