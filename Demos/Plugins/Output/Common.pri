CONFIG += c++11
CONFIG += release
CONFIG += no_plugin_name_prefix
CONFIG += no_plugin_extension
QMAKE_EXTENSION_SHLIB = audiobox

LIBS= -L/usr/lib -lAudioBox

# Template must be lib ( Don't touch )
TEMPLATE = lib

# Naming configuration ( Don't touch )
CONFIG += plugin

# Must be defined only for plugins ( Don't touch )
DEFINES += AUDIOBOX_PLUGIN

BUNDLER_PATH = ../../../Tools/Bundler

# Necessary Mac OS X Libraries
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

    # Creates the bundle
    QMAKE_POST_LINK += $$quote($${BUNDLER_PATH} $${DESTDIR}/Core.dsp $${DESTDIR}$$escape_expand(\n\t))

}



