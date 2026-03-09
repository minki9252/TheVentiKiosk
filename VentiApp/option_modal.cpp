#include "option_modal.h"
#include "ui_option_modal.h"

option_modal::option_modal(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::option_modal)
{
    ui->setupUi(this);
}

option_modal::~option_modal()
{
    delete ui;
}
