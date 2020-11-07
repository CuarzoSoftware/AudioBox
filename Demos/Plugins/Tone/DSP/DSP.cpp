#include <stdio.h>
#include <DSP.h>

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
    return "Tone";
}

// Return your company name
const char *DSP::getManufacturer()
{
    return "Cuarzo";
}

// Return the type of plugin
AudioBox::Type DSP::getType()
{
    return AudioBox::Type::Generator;
}

// Return the type of UI
AudioBox::UI DSP::getUIType()
{
    return AudioBox::UI::Cocoa;
}

void DSP::Initialize()
{

}

void DSP::Uninitialize()
{

}

void DSP::start()
{

}

void DSP::renderAudio(void *inBuffer, unsigned int framesNum, unsigned int channels)
{
    (void) channels;

    float *buffer = (float*)inBuffer;

    for(unsigned int i = 0; i < framesNum; i++)
    {
        buffer[i] = sinf(theta);
        theta += (2*M_PI*440.f)/44100.f;
    }
}

DSP::DSP()
{
    addSampleRate(44100);
    addSampleRate(48000);
    addSampleRate(19200);
    setSampleRate(44100);
}

