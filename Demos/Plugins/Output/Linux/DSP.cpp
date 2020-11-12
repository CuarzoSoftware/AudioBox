#include "DSP.h"
#include <QDebug>

DSP::DSP()
{

}

// List avaliable devices
static void device_list(snd_pcm_stream_t stream)
{
    // Card handle
    snd_ctl_t *handle;
    int err, dev, idx;
    snd_ctl_card_info_t *info;
    snd_pcm_info_t *pcminfo;
    snd_ctl_card_info_alloca(&info);
    snd_pcm_info_alloca(&pcminfo);

    // First card index
    int card = -1;

    // Look for the first card avaliable
    if (snd_card_next(&card) < 0 || card < 0)
    {
        qDebug() << "No soundcards found.";
        return;
    }

    qDebug() << "---- List of" << snd_pcm_stream_name(stream) << "Hardware Devices ----";

    // Loop the cards
    while (card >= 0)
    {

        char name[32];

        // Creates the card name (hw:card)
        sprintf(name, "hw:%d", card);

        // Open a card ( gets the card handle )
        if ((err = snd_ctl_open(&handle, name, 0)) < 0)
        {
            // If couldn't be open
            qDebug() << "Card" << card << ":" << snd_strerror(err);
            goto next_card;
        }

        // Gets the card info
        if ((err = snd_ctl_card_info(handle, info)) < 0)
        {
            qDebug() << "Control hardware info" << card << ":" << snd_strerror(err);
            snd_ctl_close(handle);
            goto next_card;
        }

        dev = -1;
        while (1)
        {
            unsigned int count;

            if (snd_ctl_pcm_next_device(handle, &dev) < 0)
                qDebug() << "snd_ctl_pcm_next_device";

            if (dev < 0)
                break;

            snd_pcm_info_set_device(pcminfo, dev);
            snd_pcm_info_set_subdevice(pcminfo, 0);
            snd_pcm_info_set_stream(pcminfo, stream);

            if ((err = snd_ctl_pcm_info(handle, pcminfo)) < 0)
            {
                if (err != -ENOENT)
                    qDebug() << "control digital audio info" << card << ":" << snd_strerror(err);

                continue;
            }

            qDebug() << "Card" << card << ":"
                     << "[" << name << "," << dev << "]"
                     << snd_ctl_card_info_get_id(info)
                     << snd_ctl_card_info_get_name(info)
                     << dev << snd_pcm_info_get_id(pcminfo)
                     << snd_pcm_info_get_name(pcminfo);

            count = snd_pcm_info_get_subdevices_count(pcminfo);

            qDebug() << "  Subdevices:" << snd_pcm_info_get_subdevices_avail(pcminfo) << count;

            for (idx = 0; idx < (int)count; idx++)
            {
                snd_pcm_info_set_subdevice(pcminfo, idx);

                if ((err = snd_ctl_pcm_info(handle, pcminfo)) < 0)
                {
                    qDebug() << "Control digital audio playback info" << card << snd_strerror(err);
                }
                else
                {
                    qDebug() << "  Subdevice" << idx << snd_pcm_info_get_subdevice_name(pcminfo);
                }
            }
        }
        snd_ctl_close(handle);

        next_card:
        if (snd_card_next(&card) < 0)
        {
            qDebug() << "snd_card_next";
            break;
        }
    }
}

const char *DSP::getName()
{
    return "Output Audio Box";
}

const char *DSP::getManufacturer()
{
    return "Cuarzo Software";
}

AudioBox::UIType DSP::getUIType()
{
    return AudioBox::UIType::Qt;
}
