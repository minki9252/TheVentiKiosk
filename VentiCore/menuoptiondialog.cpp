#include "menuoptiondialog.h"
#include "ui_menuoptiondialog.h"

MenuOptionDialog::MenuOptionDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::MenuOptionDialog)
{
    ui->setupUi(this);
}

MenuOptionDialog::~MenuOptionDialog()
{
    delete ui;
}
