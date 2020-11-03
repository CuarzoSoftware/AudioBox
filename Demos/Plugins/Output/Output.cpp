#include <stdio.h>
#include "Output.h"
#include <QWidget>
#include <QApplication>
#include <QMacNativeWidget>

// Must return an instance subclass of AudioBox ( Your plugin class )
extern "C" AudioBox*create()
{
  return new Output();
}

// Destroy the plugin instance
extern "C" void destoy(AudioBox *box)
{
  delete box;
}

// Returns the plugin name
const char *Output::getName()
{
    return "Output";
}

// Return your company name
const char *Output::getManufacturer()
{
    return "Cuarzo";
}

// Return the type of plugin
AudioBox::Type Output::getType()
{
    return AudioBox::Type::Output;
}

// Return the type of UI
AudioBox::UI Output::getUIType()
{
    return AudioBox::UI::No;
}

struct CallbackData
{
    int i = 3;
    void (*callBackFunc)(void*,unsigned int,unsigned int,void*);
    void *userData;
};

OSStatus AudioCallback( void *inRefCon,AudioUnitRenderActionFlags *ioActionFlags,const AudioTimeStamp *inTimeStamp,UInt32 inBusNumber,UInt32 inNumberFrames,AudioBufferList *ioData)
{
    Q_UNUSED(inRefCon);
    Q_UNUSED(ioActionFlags);
    Q_UNUSED(inTimeStamp);
    Q_UNUSED(inBusNumber);
    Q_UNUSED(inNumberFrames);

    // Reads the audio output
    CallbackData *data = (CallbackData*)inRefCon;
    data->callBackFunc(ioData->mBuffers[0].mData,512,1,data->userData);

    return noErr;
}

void Output::Initialize()
{
    // The output audio unit description
    AudioComponentDescription defaultOutputDescription;
    defaultOutputDescription.componentType = kAudioUnitType_Output;
    defaultOutputDescription.componentSubType = kAudioUnitSubType_DefaultOutput;
    defaultOutputDescription.componentManufacturer = kAudioUnitManufacturer_Apple;
    defaultOutputDescription.componentFlags = 0;
    defaultOutputDescription.componentFlagsMask = 0;

    // Get the default playback output unit
    AudioComponent defaultOutput = AudioComponentFindNext(NULL, &defaultOutputDescription);
    AudioComponentInstanceNew(defaultOutput, &toneUnit);

    CallbackData *data = new CallbackData;
    data->i = 2;
    data->callBackFunc = _callBack;
    data->userData = getUserData();

    // Set the audio callback function
    AURenderCallbackStruct input;
    input.inputProc = AudioCallback;
    input.inputProcRefCon = data;

    AudioUnitSetProperty(toneUnit,
        kAudioUnitProperty_SetRenderCallback,
        kAudioUnitScope_Input,
        0,
        &input,
        sizeof(input));

    AudioStreamBasicDescription auDesc {};
    auDesc.mSampleRate = getCurrentSampleRate();
    auDesc.mFormatID = kAudioFormatLinearPCM;
    auDesc.mFormatFlags = kAudioFormatFlagsNativeFloatPacked | kAudioFormatFlagIsNonInterleaved;
    auDesc.mBytesPerPacket = 4;
    auDesc.mFramesPerPacket = 1;
    auDesc.mBytesPerFrame = 4;
    auDesc.mChannelsPerFrame = 1;
    auDesc.mBitsPerChannel = 32;

    AudioUnitSetProperty (toneUnit,
        kAudioUnitProperty_StreamFormat,
        kAudioUnitScope_Input,
        0,
        &auDesc,
        sizeof(AudioStreamBasicDescription));

    // Number of frames (4 bytes each)
    UInt32 buffSize = 512;

    AudioUnitSetProperty(toneUnit,
        kAudioDevicePropertyBufferFrameSize,
        kAudioUnitScope_Global,
        0,
        &buffSize, sizeof(UInt32));

    // Playback audio
    AudioUnitInitialize(toneUnit);
}

void Output::Uninitialize()
{
    AudioUnitUninitialize(toneUnit);
}

void Output::start()
{
    AudioOutputUnitStart(toneUnit);
}

int Output::getWindow()
{
    return window->winId();
}

Output::Output()
{
    addSampleRate(44100);
    addSampleRate(48000);
    addSampleRate(19200);
    setSampleRate(44100);
}
