#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include "javaclass.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

    private slots:
        void on_action_Load_triggered();

        void on_actionAbout_Qt_triggered();

    private:
        Ui::MainWindow * ui;
        JavaClass javaClass;

};

#endif // MAINWINDOW_H
