#include "MainWindow.h"
#include <AudioBox/AudioBox.h>
#include <QDebug>
#include <QCoreApplication>


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    /*
    QString examplesPath = QCoreApplication::applicationDirPath() + "/../../../";

    AudioBox *output = loadBox((examplesPath + "Output.audiobox").toUtf8().data());
    AudioBox *tone = loadBox((examplesPath + "Tone.audiobox").toUtf8().data());

    connectBoxes(tone,output);

    tone->Initialize();
    output->Initialize();

    output->start();
    */
}



MainWindow::~MainWindow()
{
}

