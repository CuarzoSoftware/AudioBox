#include "MainWindow.h"
#include <AudioBox.h>
#include <QDebug>
#include <QWindow>
#include <QCoreApplication>


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    QString examplesPath = QCoreApplication::applicationDirPath() + "/../../../";

    AudioBox *output = loadBox((examplesPath + "Output.audiobox").toUtf8().data());
    AudioBox *tone = loadBox((examplesPath + "Tone.audiobox").toUtf8().data());

    connectBoxes(tone,output);

    tone->Initialize();
    output->Initialize();

    output->start();

}

/*
    unsigned int size;
    DeviceBasicInfo *list;
    getOutputDevices(&list,&size);
    qDebug() << " Output Devices:";
    for(unsigned int i = 0; i < size; i++)
      qDebug() << "Device:" << list[i].name;
    if(box == 0)
    {
        qDebug() << "Could not open AudioBox\n";
        exit(1);
    }

    unsigned int *rates;
    box->getSampleRates(&rates,&size);
    qDebug() << " Avaliable Box Sample Rates:";
    for(unsigned int i = 0; i < size; i++)
      qDebug() << rates[i];
*/

MainWindow::~MainWindow()
{
}

