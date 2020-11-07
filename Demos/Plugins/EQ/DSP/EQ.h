#ifndef OUTPUT
#define OUTPUT

#include <AudioBox.h>

class EQ : public AudioBox
{
public:
  // Properties
  const char *getName();
  const char *getManufacturer();
  AudioBox::Type getType();
  AudioBox::UI getUIType();

  EQ();
  void Initialize();
  void Uninitialize();
  void start();
  int getWindow();
};

#endif
