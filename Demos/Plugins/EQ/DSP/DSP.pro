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
    EQ.cpp \

HEADERS += \
    EQ.h \

target.path = $${DESTDIR}/
!isEmpty(target.path): INSTALLS += target
