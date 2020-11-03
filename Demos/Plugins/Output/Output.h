#ifndef OUTPUT
#define OUTPUT

#include <AudioBox.h>
#include <QWidget>

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
  QWidget *window;
  AudioUnit toneUnit;
};

#endif
