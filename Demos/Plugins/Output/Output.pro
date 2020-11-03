QT += widgets

TEMPLATE = lib

TARGET = PluginDemo

CONFIG += c++11

DEFINES += AUDIOBOX_PLUGIN


SOURCES += \
    Output.cpp

HEADERS += \
    AudioBox.h \
    Output.h

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

unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
