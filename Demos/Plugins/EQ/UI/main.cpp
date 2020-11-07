#include <QApplication>
#include <EQ.h>

int main(int argc, char *argv[])
{

    // Creates the Qt application
    QApplication *app = new QApplication(argc, argv);
    EQ *eq = new EQ();
    eq->show();
    return app->exec();
}
