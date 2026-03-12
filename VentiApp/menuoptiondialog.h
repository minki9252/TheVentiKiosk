#ifndef MENUOPTIONDIALOG_H
#define MENUOPTIONDIALOG_H

#include <QDialog> // QDialog 클래스를 상속받기 위해 필수
#include <QSqlDatabase> // DB 연결용
#include <QSqlQuery>    // 쿼리 실행용
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

struct OrderInfo {
    QString menuName;
    QString options;
    int quantity;
    int basePrice;
    int totalPrice;
};

namespace Ui {
class MenuOptionDialog;
}

class MenuOptionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MenuOptionDialog(QWidget *parent = nullptr);
    ~MenuOptionDialog();
    void setMenuInfo(const QString &menuName);

    OrderInfo getSelectedOrderInfo() const;

private slots:
    void onConfirmClicked();
    void onCancelClicked();

private:
    Ui::MenuOptionDialog *ui;
    QString currentMenuName; // 현재 선택된 메뉴 이름을 저장할 변수

    int currentBasePrice;
};

#endif // MENUOPTIONDIALOG_H
