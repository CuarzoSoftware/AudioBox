#ifndef OUTPUT
#define OUTPUT

#include <AudioBox.h>

class Output : public AudioBox
{
public:
  // Properties
  const char *getName();
  const char *getManufacturer();
  AudioBox::Type getType();
  AudioBox::UI getUIType();

  Output();
  void Initialize();
  void Uninitialize();
  void start();
  int getWindow();
  AudioUnit toneUnit;
};

#endif
