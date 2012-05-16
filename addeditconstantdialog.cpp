#include "addeditconstantdialog.h"
#include "ui_addeditconstantdialog.h"

#include <QDebug>
#include "javahelper.h"

AddEditConstantDialog::AddEditConstantDialog(JavaClass & javaClass, int ind, QWidget * parent) :
    QDialog(parent),
    ui(new Ui::AddEditConstantDialog),
    javaClass(javaClass),
    index(ind)
{
    ui->setupUi(this);

    const JavaConstantPool::constant & con = javaClass.getConstantPool().getConstantByIndex(index);

    for (int i = 0; i < JavaConstantPool::NUM_CONSTANT_TYPES; i++) {
        ui->comboType->addItem(JavaHelper::constantTypeToName(JavaConstantPool::constantTypes[i]), JavaConstantPool::constantTypes[i]);
        if (con.tag == JavaConstantPool::constantTypes[i]) {
            ui->comboType->setCurrentIndex(i);
        }
    }

    switch (con.tag) {
        case JavaConstantPool::CONSTANT_STRING:
            ui->editContent->setText(JavaHelper::fromClassString(con.strdata, con.strlength));
            ui->stackedWidget->setCurrentIndex(0);
            break;
        case JavaConstantPool::CONSTANT_INTEGER:
            ui->editContent->setText(QString("%1").arg(con.intval));
            ui->stackedWidget->setCurrentIndex(0);
            break;
        case JavaConstantPool::CONSTANT_FLOAT:
            ui->editContent->setText(QString("%1").arg(con.floatval));
            ui->stackedWidget->setCurrentIndex(0);
            break;
        case JavaConstantPool::CONSTANT_LONG:
            ui->editContent->setText(QString("%1").arg(con.longval));
            ui->stackedWidget->setCurrentIndex(0);
            break;
        case JavaConstantPool::CONSTANT_DOUBLE:
            ui->editContent->setText(QString("%1").arg(con.doubleval));
            ui->stackedWidget->setCurrentIndex(0);
            break;

        case JavaConstantPool::CONSTANT_CLASSREF:
        case JavaConstantPool::CONSTANT_STRINGREF:
            ui->stackedWidget->setCurrentIndex(1);
            fillComboName(con.ref);
            break;

        case JavaConstantPool::CONSTANT_FIELDREF:
        case JavaConstantPool::CONSTANT_METHODREF:
        case JavaConstantPool::CONSTANT_INTMETHODREF:
            ui->stackedWidget->setCurrentIndex(2);
            fillComboClassRef(con.classref);
            fillComboNameTypeDesc(con.nametypedesc);
            break;

        case JavaConstantPool::CONSTANT_NAMETYPEDESC:
            ui->stackedWidget->setCurrentIndex(3);
            fillComboNameAndType(con.namedesc, con.typedesc);
            break;
    }

    ui->spinNumber->setValue(index);
}

AddEditConstantDialog::~AddEditConstantDialog()
{
    delete ui;
}

void AddEditConstantDialog::on_buttonBox_accepted()
{
    qDebug() << "Accepted!";
}

void AddEditConstantDialog::on_comboType_currentIndexChanged(int index)
{

}

void AddEditConstantDialog::fillComboName(int selection)
{
    ui->comboName->clear();

    QList<quint16> items = javaClass.getConstantPool().getConstantIndices(JavaConstantPool::CONSTANT_STRING);
    QList<quint16>::const_iterator i = items.constBegin();
    while (i != items.constEnd()) {
        //const JavaConstantPool::constant & con = javaClass.getConstantPool().getConstantByIndex(*i);
        ui->comboName->addItem(QString("[%1] ").arg(*i) + javaClass.getConstantPool().getString(*i));
        if (selection != -1 && selection == (*i)) {
            ui->comboName->setCurrentIndex(ui->comboName->count() - 1);
        }
        ++i;
    }

}

void AddEditConstantDialog::fillComboClassRef(int selection)
{
    ui->comboClassRef->clear();

    QList<quint16> items = javaClass.getConstantPool().getConstantIndices(JavaConstantPool::CONSTANT_CLASSREF);
    QList<quint16>::const_iterator i = items.constBegin();
    while (i != items.constEnd()) {
        const JavaConstantPool::constant & con = javaClass.getConstantPool().getConstantByIndex(*i);
        ui->comboClassRef->addItem(QString("[%1] ").arg(*i) + "('" + javaClass.getConstantPool().getString(con.ref) + "')");
        if (selection != -1 && selection == (*i)) {
            ui->comboClassRef->setCurrentIndex(ui->comboClassRef->count() - 1);
        }
        ++i;
    }
}

void AddEditConstantDialog::fillComboNameTypeDesc(int selection)
{
    ui->comboNameTypeDesc->clear();

    QList<quint16> items = javaClass.getConstantPool().getConstantIndices(JavaConstantPool::CONSTANT_NAMETYPEDESC);
    QList<quint16>::const_iterator i = items.constBegin();
    while (i != items.constEnd()) {
        const JavaConstantPool::constant & con = javaClass.getConstantPool().getConstantByIndex(*i);
        ui->comboNameTypeDesc->addItem(QString("[%1] ").arg(*i) + "('" + javaClass.getConstantPool().getString(con.namedesc) + "' '" + javaClass.getConstantPool().getString(con.typedesc) + "')");
        if (selection != -1 && selection == (*i)) {
            ui->comboNameTypeDesc->setCurrentIndex(ui->comboNameTypeDesc->count() - 1);
        }
        ++i;
    }
}

void AddEditConstantDialog::fillComboNameAndType(int selection1, int selection2)
{
    ui->comboName2->clear();
    ui->comboType2->clear();

    QList<quint16> items = javaClass.getConstantPool().getConstantIndices(JavaConstantPool::CONSTANT_STRING);
    QList<quint16>::const_iterator i = items.constBegin();
    while (i != items.constEnd()) {
        QString str = (QString("[%1] ").arg(*i) + javaClass.getConstantPool().getString(*i));
        ui->comboName2->addItem(str);
        ui->comboType2->addItem(str);

        if (selection1 != -1 && selection1 == (*i)) {
            ui->comboName2->setCurrentIndex(ui->comboName2->count() - 1);
        }

        if (selection2 != -1 && selection2 == (*i)) {
            ui->comboType2->setCurrentIndex(ui->comboType2->count() - 1);
        }
        ++i;
    }
}
