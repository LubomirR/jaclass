#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "javaclass.h"
#include "constantpooltablemodel.h"

#include <QDebug>
#include <QMessageBox>

#include "addeditconstantdialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_Load_triggered()
{
    QFileDialog fd(this);

    fd.setNameFilter("Java Class Files(*.class)");
    fd.setFileMode(QFileDialog::ExistingFile);
    fd.setDefaultSuffix("class");

    if (fd.exec() == QDialog::Accepted) {
        QStringList files = fd.selectedFiles();

        if (files.length() >= 1) {
            QString file = files[0];

            javaClass.load(file);

            ui->constantPool->setModel(new ConstantPoolTableModel(javaClass));
        }
    }
}

void MainWindow::on_actionAbout_Qt_triggered()
{
    qApp->aboutQt();
}

void MainWindow::on_constantPool_doubleClicked(const QModelIndex & index)
{
    qDebug() << "DOUBLE CLICK!";
    AddEditConstantDialog dlg(javaClass, javaClass.getConstantPool().numToIndex(index.row()), this);

    dlg.exec();
}
