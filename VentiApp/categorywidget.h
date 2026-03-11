#ifndef CATEGORYWIDGET_H
#define CATEGORYWIDGET_H

#include <QWidget>

namespace Ui
{
    class categorywidget;
}

class categorywidget : public QWidget
{
    Q_OBJECT

public:
    explicit categorywidget(QWidget *parent = nullptr);
    ~categorywidget();

signals:
    // 메뉴 종류(enum)를 데이터로 같이 넘겨주는 시그널
    void categorySelected(int actionCode);

private:
    Ui::categorywidget *ui;
};

#endif // CATEGORYWIDGET_H
