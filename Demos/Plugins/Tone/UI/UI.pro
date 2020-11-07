include( ../Common.pri )

# Target must be UI ( Don't touch )
TARGET = UI

QT += core gui
QT += widgets

HEADERS += \
    UI.h

SOURCES += \
    UI.cpp \
    main.cpp

target.path = $${DESTDIR}/
!isEmpty(target.path): INSTALLS += target

QMAKE_POST_LINK += $$quote($${BUNDLER_PATH} $${DESTDIR}/Core.dsp $${DESTDIR}$$escape_expand(\n\t))
