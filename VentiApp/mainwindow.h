#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVariant>
#include <QSqlDatabase>
#include <QTimer>
#include <QMap>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

enum KioskAction {
    CATEGORY_NEW_MENU,
    CATEGORY_ICED_COFFEE,
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
    void toggleTouchText(); // 텍스트 깜빡임용 슬롯

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    QTimer *touchTimer;     // 타이머 객체
    QMap<QString, int> cartData;

    bool isVisible = true;  // 가시성 상태 변수
    int currentOrderType = 0; // 0: 매장, 1: 포장
    void handle(const KioskEvent &event); // 이벤트를 처리할 핸들 함수
    void loadMenus(const QString &categoryName);    // 메뉴판을 채우는 함수
    void updateCartTable();
    void processCheckout();

};

#endif // MAINWINDOW_H
