#ifndef AUDIOBOX
#define AUDIOBOX

#include "Shared/Includes.h"
#include "Linux/Includes.h"
#include "MacOS/Includes.h"


void getOutputDevices(DeviceBasicInfo **list,unsigned int *size);

class AudioBox
{
public:

    // Types of UI ( Compositor )
    enum UIType
    {
        No      = 0,
        Carbon  = 1,
        Cocoa   = 2,
        Qt      = 3,
        Wayland = 4,
        X11     = 5
    };

    // Types of plugin
    enum BoxType
    {
        Output      = 0,
        Input       = 1,
        Generator   = 2,
        Effect      = 3
    };

    // Stream formats
    enum StreamFormat
    {
        Int16Bits   = 0,
        Int32Bits   = 1,
        Int64Bits   = 2,
        Float16Bits = 3,
        Float32Bits = 4,
        Float64Bits = 5
    };

    // Bus Types
    enum BusType
    {
        Mono    = 1,
        Stereo  = 2
    };

    // Errors
    enum ABErr
    {
        NoError = 1,
        InvalidSampleRate = 2
    };

    // Sample rates
    struct SampleRate
    {
        unsigned int rate = 0;
        SampleRate *next = nullptr;
    };

    // General info
    virtual const char *getName();
    virtual const char *getManufacturer();
    virtual BoxType getType();
    virtual UIType getUIType();

    virtual void Initialize();
    virtual void Uninitialize();
    virtual void renderAudio(void *inBuffer,unsigned int framesNum,unsigned int channels);

    virtual ABErr setSampleRate(unsigned int rate);
    virtual void getSampleRates(unsigned int **list, unsigned int *size);
    virtual unsigned int getCurrentSampleRate();

    void *getUserData();
    void setCallbackFunction(void (*callBack)(void*,unsigned int,unsigned int,void*),void *userData);
    void internalCallback(void *inBuffer,unsigned int framesNum,unsigned int channels, void*userData);

    virtual ~AudioBox();

private:
    void *_userData;
    unsigned int _currentSampleRate = 0;
    SampleRate _sampleRates;

protected:
    void addSampleRate(unsigned int sampleRate);


    void (*externalCallback)(void*,unsigned int,unsigned int,void*);


};



// Host Related
#ifndef AUDIOBOX_PLUGIN

// the types of the class factories
typedef AudioBox* create_t();
typedef void destroy_t(AudioBox*);
AudioBox *loadBox(const char *path);
void callbackJoin(void *buffer,unsigned int bufferSize,unsigned int channels,void *userData);
void connectBoxes(AudioBox *source,AudioBox *destination);

#endif


#endif
