#ifndef AUDIOBOX_H
#define AUDIOBOX_H

#include <QMainWindow>

class AudioBox : public QMainWindow
{
    Q_OBJECT

public:
    AudioBox(QWidget *parent = nullptr);
    ~AudioBox();
};
#endif // AUDIOBOX_H
