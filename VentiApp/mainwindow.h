#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVariant>
#include <QtSql/QSqlDatabase>
#include <QSqlDatabase> // (아래 팁 참고)

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

enum KioskAction {
    CATEGORY_COFFEE,
    CATEGORY_BEVERAGE,
    CATEGORY_DESSERT,
    MENU_SELECT_ITEM,
    MENU_CANCEL_ITEM,
    CART_ADD,
    CART_REMOVE,
    CART_CHECKOUT
};

// 🌟 이벤트를 담을 구조체 정의
struct KioskEvent {
    KioskAction action;
    int menuId = -1;           // 선택한 메뉴의 DB ID
    int quantity = 1;          // 수량 (기본값 1)
    QString option = "";       // ICE/HOT 등 옵션 정보
    QVariant extraData;        // 기타 범용 데이터가 필요할 때를 대비
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_introButton_clicked();   
    void on_storeButton_clicked();   
    void on_takeoutButton_clicked(); 

private:
    Ui::MainWindow *ui;

    QSqlDatabase db;

    void handle(const KioskEvent &event); // 이벤트를 처리할 핸들 함수
};

#endif // MAINWINDOW_H
