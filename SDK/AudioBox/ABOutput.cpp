#include "ABOutput.h"

AudioBox::BoxType ABOutput::getType()
{
    return AudioBox::BoxType::Output;
}

void ABOutput::start()
{

}

AudioBox::ABErr ABOutput::setBufferSize(unsigned int bufferSize)
{
    (void)bufferSize;
    return AudioBox::ABErr::NoError;
}
