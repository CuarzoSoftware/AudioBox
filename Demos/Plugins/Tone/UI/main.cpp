#include <QApplication>
#include <UI.h>

int main(int argc, char *argv[])
{
    // Creates the Qt application
    QApplication *app = new QApplication(argc, argv);
    UI *ui = new UI();
    ui->show();
    return app->exec();
}
