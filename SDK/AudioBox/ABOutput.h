#ifndef ABOUTPUT_H
#define ABOUTPUT_H

#include "AudioBox.h"

class ABOutput : public AudioBox
{
public:
    virtual AudioBox::BoxType getType() override;
    virtual void start();
    virtual AudioBox::ABErr setBufferSize(unsigned int bufferSize);
};

#endif // OUTPUT_H
