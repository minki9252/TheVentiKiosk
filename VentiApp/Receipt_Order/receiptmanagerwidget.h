#ifndef RECEIPTMANAGERWIDGET_H
#define RECEIPTMANAGERWIDGET_H

#include <QWidget>

namespace Ui {
class ReceiptManagerWidget;
}

class ReceiptManagerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ReceiptManagerWidget(QWidget *parent = nullptr);
    ~ReceiptManagerWidget();

private:
    Ui::ReceiptManagerWidget *ui;
};

#endif // RECEIPTMANAGERWIDGET_H
