#ifndef AUDIOBOX
#define AUDIOBOX

#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Apple Includes
#ifdef __APPLE__
    #include <TargetConditionals.h>
    #include <AudioToolbox/AudioToolbox.h>
    #include <CoreFoundation/CoreFoundation.h>
#endif

// Apple Structs
#ifdef __APPLE__
struct DeviceBasicInfo
{
    AudioDeviceID id;
    char *name = nullptr;
    char *manufacturer = nullptr;
};

#endif

void getOutputDevices(DeviceBasicInfo **list,unsigned int *size)
{
    AudioObjectID *deviceIDs;
    UInt32 propertySize;
    int numDevices;
    char deviceName[64];
    char manufacturerName[64];

    // To store the calls
    AudioObjectPropertyAddress propertyAddress;
    propertyAddress.mSelector = kAudioHardwarePropertyDevices;
    propertyAddress.mScope = kAudioObjectPropertyScopeGlobal;
    propertyAddress.mElement = kAudioObjectPropertyElementMaster;

    // Get all devices IDs
    AudioObjectGetPropertyDataSize(kAudioObjectSystemObject, &propertyAddress, 0, NULL, &propertySize);
    numDevices = propertySize / sizeof(AudioDeviceID);
    deviceIDs = (AudioDeviceID *)calloc(numDevices, sizeof(AudioDeviceID));
    AudioObjectGetPropertyData(kAudioObjectSystemObject, &propertyAddress, 0, NULL, &propertySize, deviceIDs);

    // Reset size
    *size = 0;

    // Count only the output devices
    propertySize = sizeof(UInt32);
    propertyAddress.mSelector = kAudioDevicePropertyStreams;
    propertyAddress.mScope = kAudioObjectPropertyScopeOutput;
    propertyAddress.mElement = kAudioObjectPropertyElementMaster;
    for (int i = 0; i < numDevices; i++)
    {
        // Check if is Output device
        AudioObjectGetPropertyDataSize(deviceIDs[i], &propertyAddress, 0, NULL, &propertySize);
        if(propertySize / sizeof(AudioStreamID) > 0)
            (*size)++;
    }

    // Creates the devices info array
    (*list) = new DeviceBasicInfo[(*size)];

    DeviceBasicInfo *p = *list;

    unsigned int count = 0;
    for (int i = 0; i < numDevices; i++)
    {
        propertySize = sizeof(UInt32);
        propertyAddress.mSelector = kAudioDevicePropertyStreams;
        propertyAddress.mScope = kAudioObjectPropertyScopeOutput;
        propertyAddress.mElement = kAudioObjectPropertyElementMaster;

        // Check if is Output device
        AudioObjectGetPropertyDataSize(deviceIDs[i], &propertyAddress, 0, NULL, &propertySize);
        if(propertySize / sizeof(AudioStreamID) > 0)
        {

            // Stores the devices ID
            p[count].id = deviceIDs[i];

            // Gets the device name
            propertySize = sizeof(deviceName);
            propertyAddress.mSelector = kAudioDevicePropertyDeviceName;
            propertyAddress.mScope = kAudioObjectPropertyScopeGlobal;
            propertyAddress.mElement = kAudioObjectPropertyElementMaster;
            AudioObjectGetPropertyData(deviceIDs[i], &propertyAddress, 0, NULL, &propertySize, deviceName);
            p[count].name = new char[strlen(deviceName)+1];
            strcpy(p[count].name,deviceName);



            // Gets the manufacturer name
            propertySize = sizeof(manufacturerName);
            propertyAddress.mSelector = kAudioDevicePropertyDeviceManufacturer;
            propertyAddress.mScope = kAudioObjectPropertyScopeGlobal;
            propertyAddress.mElement = kAudioObjectPropertyElementMaster;
            AudioObjectGetPropertyData(deviceIDs[i], &propertyAddress, 0, NULL, &propertySize, manufacturerName);
            p[count].manufacturer = new char[strlen(manufacturerName)+1];
            strcpy(p[count].manufacturer,manufacturerName);

            count++;
        }
    }
}

class AudioBox
{
public:
    // Types of UI ( Compositor )
    enum UI
    {
        No = 0,
        Quartz = 1,
        X11 = 2,
        Wayland = 3
    };

    // Types of plugin
    enum Type
    {
        Output = 0,
        Input = 1,
        Effect = 2
    };

    // Stream formats
    enum Format
    {
        Int16Bits   = 0,
        Int32Bits   = 1,
        Int64Bits   = 2,
        Float16Bits = 3,
        Float32Bits = 4,
        Float64Bits = 5
    };

    // Errors
    enum Error
    {
        NoError = 1,
        InvalidSampleRate = 2
    };

    // General info
    virtual const char *getName(){return NULL;};
    virtual const char *getManufacturer(){return NULL;};
    AudioBox::Type getType();
    virtual AudioBox::UI getUIType(){ return UI::No;};
    void *getUserData()
    {
        return _userData;
    }


    virtual void getSampleRates(unsigned int **list, unsigned int *size)
    {
        *size = 1;
        SampleRate *p = &_sampleRates;
        while(p->next != nullptr)
        {
            p = p->next;
            (*size)++;
        }

        p = &_sampleRates;
        (*list) = new unsigned int[(*size)];

        unsigned int *s = *list;
        for(unsigned int i = 0; i < (*size); i++)
        {
            s[i] = p->rate;
            p = p->next;
        }
    }

    // Audio
    virtual unsigned int getCurrentSampleRate()
    {
        if(_currentSampleRate == 0)
            return _sampleRates.rate;
        else
            return _currentSampleRate;
    };

    virtual Error setSampleRate(unsigned int rate)
    {
        unsigned int *rates;
        unsigned int size;
        getSampleRates(&rates,&size);

        for(unsigned int i = 0; i < size; i++)
        {
            if(rate == rates[i])
            {
                _currentSampleRate = rate;
                return Error::NoError;
            }
        }
        return Error::InvalidSampleRate;
    }

    void setCallbackFunction(void (*callBack)(void*,unsigned int,unsigned int,void*),void *userData)
    {
        _callBack = callBack;
        _userData = userData;
    }

    virtual void Initialize(){};
    virtual void Uninitialize(){};
    virtual void start(){};
    virtual ~AudioBox(){}

private:
    void *_userData;

    // Sample rates
    struct SampleRate
    {
        unsigned int rate = 0;
        SampleRate *next = nullptr;
    };

    unsigned int _currentSampleRate = 0;
    SampleRate _sampleRates;

protected:
    void (*_callBack)(void*,unsigned int,unsigned int,void*);

    // Add a sample rate that the plugin can work with
    void addSampleRate(unsigned int sampleRate)
    {
        // If adding the first sample rate
        if(_sampleRates.rate == 0)
        {
            _sampleRates.rate = sampleRate;
            return;
        }
        else
        {
            // Search for the last sample rate added
            SampleRate *p = &_sampleRates;
            while(p->next != nullptr)
                p = p->next;

            // Stores the new sample rate
            p->next = (SampleRate*)malloc(sizeof(SampleRate));
            p->next->rate = sampleRate;
            p->next->next = nullptr;
        }
    }
};

// Apple utilities
#ifdef __APPLE__

    char * CFStringToChar(CFStringRef aString)
    {
        if (aString == NULL)
            return NULL;

        CFIndex length = CFStringGetLength(aString);
        CFIndex maxSize = CFStringGetMaximumSizeForEncoding(length, kCFStringEncodingUTF8) + 1;
        char *buffer = (char *)malloc(maxSize);

        if (CFStringGetCString(aString, buffer, maxSize,kCFStringEncodingUTF8))
            return buffer;

        free(buffer);
        return NULL;
    }
#endif


// Host Related
#ifndef AUDIOBOX_PLUGIN

// the types of the class factories
typedef AudioBox* create_t();
typedef void destroy_t(AudioBox*);

AudioBox *loadBox(const char *path)
{
  char *dspPath = new char[strlen(path)+10];
  strcpy(dspPath,path);
  dspPath = strcat(dspPath,"/DSP/Core");
  void *handle = dlopen(dspPath, RTLD_LAZY);
  if(handle == NULL)
  {
    printf("%s%s\n","Could not load AudioBox: ",dlerror());
    return NULL;
  }
  create_t *create = (create_t*) dlsym(handle, "create");
  return create();
}
AudioBox *loadDSP(const char *path)
{
  void *handle = dlopen(path, RTLD_LAZY);
  if(handle == NULL)
  {
    printf("%s%s\n","Could not load AudioBox: ",dlerror());
    return NULL;
  }
  create_t *create = (create_t*) dlsym(handle, "create");
  return create();
}

#endif


#endif
