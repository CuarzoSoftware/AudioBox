#include <stdio.h>
#include "EQ.h"

// Must return an instance subclass of AudioBox ( Your plugin class )
extern "C" AudioBox*create()
{
  return new EQ();
}

// Destroy the plugin instance
extern "C" void destoy(AudioBox *box)
{
  delete box;
}

// Returns the plugin name
const char *EQ::getName()
{
    return "EQ";
}

// Return your company name
const char *EQ::getManufacturer()
{
    return "Cuarzo";
}

// Return the type of plugin
AudioBox::Type EQ::getType()
{
    return AudioBox::Type::Effect;
}

// Return the type of UI
AudioBox::UI EQ::getUIType()
{
    return AudioBox::UI::Cocoa;
}


void EQ::Initialize()
{

}

void EQ::Uninitialize()
{

}

void EQ::start()
{

}

EQ::EQ()
{
    addSampleRate(44100);
    addSampleRate(48000);
    addSampleRate(19200);
    setSampleRate(44100);
}
