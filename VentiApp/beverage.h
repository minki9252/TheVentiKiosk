#ifndef BEVERAGE_H
#define BEVERAGE_H

#include <QWidget>

namespace Ui
{
    class beverage;
}

class beverage : public QWidget
{
    Q_OBJECT

public:
    explicit beverage(QWidget *parent = nullptr);
    ~beverage();

    // 🛠️ MainWindow에서 카테고리 이름을 넘겨받아 메뉴를 그릴 함수
    void loadMenus(const QString &categoryName);

private:
    Ui::beverage *ui;
};

#endif // BEVERAGE_H
