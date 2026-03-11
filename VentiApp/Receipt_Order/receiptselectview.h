#ifndef RECEIPTSELECTVIEW_H
#define RECEIPTSELECTVIEW_H

#include <QWidget>

namespace Ui {
class ReceiptSelectView;
}

class ReceiptSelectView : public QWidget
{
    Q_OBJECT

public:
    explicit ReceiptSelectView(QWidget *parent = nullptr);
    ~ReceiptSelectView();

private:
    Ui::ReceiptSelectView *ui;
};

#endif // RECEIPTSELECTVIEW_H
