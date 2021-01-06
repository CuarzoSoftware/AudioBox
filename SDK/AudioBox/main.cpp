#include "AudioBox.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AudioBox w;
    w.show();
    return a.exec();
}
