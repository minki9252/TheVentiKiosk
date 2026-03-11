#ifndef PAYPROCESSVIEW_H
#define PAYPROCESSVIEW_H

#include <QWidget>

namespace Ui {
class PayProcessView;
}

class PayProcessView : public QWidget
{
    Q_OBJECT

public:
    explicit PayProcessView(QWidget *parent = nullptr);
    ~PayProcessView();

private:
    Ui::PayProcessView *ui;
};

#endif // PAYPROCESSVIEW_H
