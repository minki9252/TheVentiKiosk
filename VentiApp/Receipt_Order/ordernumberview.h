#ifndef ORDERNUMBERVIEW_H
#define ORDERNUMBERVIEW_H

#include <QWidget>

namespace Ui {
class OrderNumberView;
}

class OrderNumberView : public QWidget
{
    Q_OBJECT

public:
    explicit OrderNumberView(QWidget *parent = nullptr);
    ~OrderNumberView();

private:
    Ui::OrderNumberView *ui;
};

#endif // ORDERNUMBERVIEW_H
