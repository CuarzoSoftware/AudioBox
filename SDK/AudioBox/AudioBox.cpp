#include "AudioBox.h"

const char *AudioBox::getName()
{
    return NULL;
}

const char *AudioBox::getManufacturer()
{
    return NULL;
}

AudioBox::BoxType AudioBox::getType()
{
    return Effect;
}

AudioBox::UIType AudioBox::getUIType()
{
    return UIType::No;
}

void *AudioBox::getUserData()
{
    return _userData;
}

void AudioBox::getSampleRates(unsigned int **list, unsigned int *size)
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

unsigned int AudioBox::getCurrentSampleRate()
{
    if(_currentSampleRate == 0)
        return _sampleRates.rate;
    else
        return _currentSampleRate;
};

AudioBox::ABErr AudioBox::setSampleRate(unsigned int rate)
{
    unsigned int *rates;
    unsigned int size;
    getSampleRates(&rates,&size);

    for(unsigned int i = 0; i < size; i++)
    {
        if(rate == rates[i])
        {
            _currentSampleRate = rate;
            return AudioBox::ABErr::NoError;
        }
    }
    return AudioBox::ABErr::InvalidSampleRate;
}

void AudioBox::setCallbackFunction(void (*callBack)(void*,unsigned int,unsigned int,void*),void *userData)
{
    externalCallback = callBack;
    _userData = userData;
}

void AudioBox::Initialize(){};
void AudioBox::Uninitialize(){};
void AudioBox::renderAudio(void *inBuffer,unsigned int framesNum,unsigned int channels)
{
  (void)inBuffer;
  (void)framesNum;
  (void)channels;
}

void AudioBox::internalCallback(void *inBuffer,unsigned int framesNum,unsigned int channels, void*userData)
{
  externalCallback(inBuffer,framesNum,channels,userData);
  renderAudio(inBuffer,framesNum,channels);
}

AudioBox::~AudioBox(){}

// Add a sample rate that the plugin can work with
void AudioBox::addSampleRate(unsigned int sampleRate)
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

void callbackJoin(void *buffer,unsigned int bufferSize,unsigned int channels,void *userData)
{
  AudioBox *source = (AudioBox*)userData;
  source->renderAudio(buffer,bufferSize,channels);
}

void connectBoxes(AudioBox *source,AudioBox *destination)
{
  destination->setCallbackFunction(callbackJoin,source);
}

// Apple Includes
#ifdef __APPLE__

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

AudioBox *loadBox(const char *path)
{
  char *dspPath = new char[strlen(path)+10];
  strcpy(dspPath,path);
  dspPath = strcat(dspPath,"/DSP/Core.dsp");
  void *handle = dlopen(dspPath, RTLD_LAZY);
  if(handle == NULL)
  {
    printf("%s%s\n","Could not load AudioBox: ",dlerror());
    return NULL;
  }
  create_t *create = (create_t*) dlsym(handle, "create");
  return create();
}
#endif

#ifdef __linux__

AudioBox *loadBox(const char *path)
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

void getOutputDevices(DeviceBasicInfo **list,unsigned int *size)
{

}

#endif
