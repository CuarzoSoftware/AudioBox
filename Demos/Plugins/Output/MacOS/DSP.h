#ifndef OUTPUT
#define OUTPUT

#include <AudioBox.h>

class DSP : public AudioBox
{
public:
  // Properties
  const char *getName();
  const char *getManufacturer();
  AudioBox::Type getType();
  AudioBox::UI getUIType();

  DSP();
  void Initialize();
  void Uninitialize();
  void start();
  int getWindow();
  void renderAudio(void *inBuffer, unsigned int bufferSize, unsigned int channels);

  // Plugin
  AudioUnit toneUnit;
  float theta = 0.f;
};

#endif
