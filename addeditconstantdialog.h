#ifndef ADDEDITCONSTANTDIALOG_H
#define ADDEDITCONSTANTDIALOG_H

#include <QDialog>

#include "javaconstantpool.h"
#include "javaclass.h"

namespace Ui {
    class AddEditConstantDialog;
}

class AddEditConstantDialog : public QDialog
{
    Q_OBJECT

    public:
        explicit AddEditConstantDialog(JavaClass & javaClass, int ind, QWidget * parent = 0);
        ~AddEditConstantDialog();

    private slots:
        void on_buttonBox_accepted();

        void on_comboType_currentIndexChanged(int index);

    private:
        void fillComboName(int selection = -1);
        void fillComboClassRef(int selection = -1);
        void fillComboNameTypeDesc(int selection = -1);
        void fillComboNameAndType(int selection1 = -1, int selection2 = -1);

        Ui::AddEditConstantDialog * ui;
        JavaClass & javaClass;
        const int index;
};

#endif // ADDEDITCONSTANTDIALOG_H
