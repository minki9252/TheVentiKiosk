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
    QString menuName;   // 메뉴 이름
    int basePrice;      // 기본 가격
    int totalPrice;     // 옵션 포함 총 가격
    int quantity;       // 수량 (보통 1로 시작)
    QString options;    // 선택한 옵션들을 합친 문자열
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
    void onConfirmClicked(); // 확인 버튼 슬롯
    void onCancelClicked();  // 취소 버튼 슬롯

private:
    Ui::MenuOptionDialog *ui;
    QString currentMenuName; // 현재 선택된 메뉴 이름을 저장할 변수

    int currentBasePrice;
};

#endif // MENUOPTIONDIALOG_H
