QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11



SOURCES += \
    main.cpp \
    MainWindow.cpp

HEADERS += \
    MainWindow.h \
    ../AudioBox/AudioBox.h

macx {
    QMAKE_LFLAGS += -F /System/Library/Frameworks/CoreFoundation.framework/
    QMAKE_LFLAGS += -F /System/Library/Frameworks/CoreAudio.framework/
    QMAKE_LFLAGS += -F /System/Library/Frameworks/AudioToolBox.framework/
    QMAKE_LFLAGS += -F /System/Library/Frameworks/AudioUnit.framework/
    QMAKE_LFLAGS += -F /System/Library/Frameworks/AppKit.framework/

    LIBS += -framework CoreFoundation
    LIBS += -framework CoreAudio
    LIBS += -framework AudioToolbox
    LIBS += -framework AudioUnit
    LIBS += -framework AppKit
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
