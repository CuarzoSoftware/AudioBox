#include <stdio.h>
#include <DSP.h>
#include <QDebug>
// Must return an instance subclass of AudioBox ( Your plugin class )
extern "C" AudioBox*create()
{
  return new DSP();
}

// Destroy the plugin instance
extern "C" void destoy(AudioBox *box)
{
  delete box;
}

// Returns the plugin name
const char *DSP::getName()
{
    return "Output";
}

// Return your company name
const char *DSP::getManufacturer()
{
    return "Cuarzo";
}

// Return the type of plugin
AudioBox::Type DSP::getType()
{
    return AudioBox::Type::Output;
}

// Return the type of UI
AudioBox::UI DSP::getUIType()
{
    return AudioBox::UI::No;
}


OSStatus AudioCallback( void *inRefCon,AudioUnitRenderActionFlags *ioActionFlags,const AudioTimeStamp *inTimeStamp,UInt32 inBusNumber,UInt32 inNumberFrames,AudioBufferList *ioData)
{
    // Avoid unused warnings
    (void)ioActionFlags;
    (void)inTimeStamp;
    (void)inBusNumber;
    (void)inNumberFrames;

    // Reads the audio output
    AudioBox *box = (AudioBox*)inRefCon;
    box->internalCallback(ioData->mBuffers[0].mData,512,ioData->mBuffers->mNumberChannels,box->getUserData());
    return noErr;
}

void DSP::renderAudio(void *inBuffer,unsigned int bufferSize,unsigned int channels)
{
    // Avoid unused warnings
    (void)inBuffer;
    (void)bufferSize;
    (void)channels;
}

void DSP::Initialize()
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

    // Set the audio callback function
    AURenderCallbackStruct input;
    input.inputProc = AudioCallback;
    input.inputProcRefCon = this;

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

void DSP::Uninitialize()
{
    AudioUnitUninitialize(toneUnit);
}

void DSP::start()
{
    AudioOutputUnitStart(toneUnit);
}

DSP::DSP()
{
    addSampleRate(44100);
    addSampleRate(48000);
    addSampleRate(19200);
    setSampleRate(44100);
}
