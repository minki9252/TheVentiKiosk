#ifndef BEVERAGE_H
#define BEVERAGE_H

#include <QWidget>

namespace Ui {
class beverage;
}

class beverage : public QWidget
{
    Q_OBJECT

public:
    explicit beverage(QWidget *parent = nullptr);
    ~beverage();

private:
    Ui::beverage *ui;
};

#endif // BEVERAGE_H
