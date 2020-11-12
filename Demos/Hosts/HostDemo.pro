QT += core gui widgets
TEMPLATE = app
CONFIG += c++11
CONFIG += release

LIBS= -L/usr/lib -lAudioBox

DESTDIR = ../../Builds/HostDemo

SOURCES += \
    main.cpp \
    MainWindow.cpp

HEADERS += \
    MainWindow.h \


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
