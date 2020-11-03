#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    QWidget *central;
    float theta = 0.f;
    ~MainWindow();
};
#endif // MAINWINDOW_H
