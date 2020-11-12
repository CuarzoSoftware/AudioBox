# Common configurations
include( Common.pri )

# The name of the plugin
TARGET = Output

# Output directory ( To your choice )
DESTDIR = ../../../Builds/Linux/


# Linux
unix:!macx {

SOURCES += \
    Linux/DSP.cpp \

HEADERS += \
    Linux/DSP.h \

}
# Mac OS X
macx {

SOURCES += \
    MacOS/DSP.cpp \

HEADERS += \
    MacOS/DSP.h \

}
# Windows
win32 {

}

target.path = $${DESTDIR}/
!isEmpty(target.path): INSTALLS += target


