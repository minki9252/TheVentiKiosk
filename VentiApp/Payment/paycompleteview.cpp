#include "paycompleteview.h"
#include "ui_paycompleteview.h"

PayCompleteView::PayCompleteView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PayCompleteView)
{
    ui->setupUi(this);
}

PayCompleteView::~PayCompleteView()
{
    delete ui;
}
