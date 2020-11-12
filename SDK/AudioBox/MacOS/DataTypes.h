#ifdef __APPLE__
#ifndef DATATYPESMACOS_H
#define DATATYPESMACOS_H

    struct DeviceBasicInfo
    {
        AudioDeviceID id;
        char *name = nullptr;
        char *manufacturer = nullptr;
    };

#endif
#endif
