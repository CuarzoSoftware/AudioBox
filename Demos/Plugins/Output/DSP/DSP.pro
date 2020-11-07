include( ../Common.pri )

# Target must be Core ( Don't touch )
TARGET = Core

# Template must be lib ( Don't touch )
TEMPLATE = lib

# Naming configuration ( Don't touch )
CONFIG += plugin

# Must be defined for plugins ( Don't touch )
DEFINES += AUDIOBOX_PLUGIN


SOURCES += \
    DSP.cpp \

HEADERS += \
    DSP.h \

target.path = $${DESTDIR}/
!isEmpty(target.path): INSTALLS += target

QMAKE_POST_LINK += $$quote($${BUNDLER_PATH} $${DESTDIR}/Core.dsp $${DESTDIR}$$escape_expand(\n\t))

