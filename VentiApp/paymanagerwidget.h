#ifndef PAYMANAGERWIDGET_H
#define PAYMANAGERWIDGET_H

#include <QWidget>

namespace Ui {
class PayManagerWidget;
}

class PayManagerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PayManagerWidget(QWidget *parent = nullptr);
    ~PayManagerWidget();

private:
    Ui::PayManagerWidget *ui;
};

#endif // PAYMANAGERWIDGET_H
