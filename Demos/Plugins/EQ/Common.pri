CONFIG += c++11
CONFIG += release
CONFIG += no_plugin_name_prefix
CONFIG += no_plugin_extension
CONFIG -= app_bundle
QMAKE_EXTENSION_SHLIB = dsp

# Path to the AudioBox SDK ( Fix it if necessary )
SDK_PATH = ../../../../SDK/
BUNDLER_PATH = ../../../../Tools/Bundler

# Output directory ( To your choice )
DESTDIR = $${SDK_PATH}../Builds/

# Includes the SDK path
INCLUDEPATH += $${SDK_PATH}

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

