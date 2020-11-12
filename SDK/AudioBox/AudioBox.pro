TEMPLATE = lib

HEADERS += \
        ABOutput.h \
        AudioBox.h \
        Linux/DataTypes.h \
        Linux/Includes.h \
        MacOS/DataTypes.h \
        MacOS/Includes.h \
        MacOS/Utils.h \
        Shared/DataTypes.h \
        Shared/Includes.h

root.path = /usr/include/AudioBox/
shared.path = /usr/include/AudioBox/Shared
linux.path = /usr/include/AudioBox/Linux
macos.path = /usr/include/AudioBox/MacOS

root.files += $$PWD/*.h
shared.files += $$PWD/Shared/*.h
linux.files += $$PWD/Linux/*.h
macos.files += $$PWD/MacOS/*.h

libraryFiles.path = /usr/lib/
libraryFiles.files = $$OUT_PWD/*.so*

INSTALLS += root
INSTALLS += shared
INSTALLS += linux
INSTALLS += macos
INSTALLS += libraryFiles

SOURCES += \
    ABOutput.cpp \
    AudioBox.cpp
