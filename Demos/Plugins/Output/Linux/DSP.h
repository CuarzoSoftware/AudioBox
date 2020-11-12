#ifndef DSP_H
#define DSP_H

#include <AudioBox/ABOutput.h>

class DSP : public ABOutput
{
public:
    DSP();
    // General info
    virtual const char *getName() override;
    virtual const char *getManufacturer() override;
    virtual UIType getUIType() override;

    virtual void Initialize() override;
    virtual void Uninitialize() override;
    virtual void renderAudio(void *inBuffer,unsigned int framesNum,unsigned int channels) override;

    virtual ABErr setSampleRate(unsigned int rate) override;
    virtual void getSampleRates(unsigned int **list, unsigned int *size) override;
    virtual unsigned int getCurrentSampleRate() override;
};

#endif // DSP_H
