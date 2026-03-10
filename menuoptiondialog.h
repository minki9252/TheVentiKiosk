#ifndef MENUOPTIONDIALOG_H
#define MENUOPTIONDIALOG_H

#include <QDialog>

namespace Ui {
class MenuOptionDialog;
}

class MenuOptionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MenuOptionDialog(QWidget *parent = nullptr);
    ~MenuOptionDialog();

private:
    Ui::MenuOptionDialog *ui;
};

#endif // MENUOPTIONDIALOG_H
