#ifndef PAYMETHODVIEW_H
#define PAYMETHODVIEW_H

#include <QWidget>

namespace Ui {
class PayMethodView;
}

class PayMethodView : public QWidget
{
    Q_OBJECT

public:
    explicit PayMethodView(QWidget *parent = nullptr);
    ~PayMethodView();

private:
    Ui::PayMethodView *ui;
};

#endif // PAYMETHODVIEW_H
