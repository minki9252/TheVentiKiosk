#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVariant>
#include <QSqlDatabase>
#include <QListWidget>

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
    void toggleTouchText(); // 텍스트 깜빡임용 슬롯
    // void loadCategoriesToUI();
    // void on_listMenu_itemClicked(QListWidgetItem *item);
    // void updateMenuDisplay(const QString &categoryName);


private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    QTimer *touchTimer;     // 타이머 객체
    QMap<QString, int> cartData;

    // QTimer *adTimer;          // 광고 전환용 타이머
    // QStringList adImages;     // 광고 이미지 경로 목록
    // int currentAdIndex = 0;   // 현재 보여주는 이미지 번호
    // void showNextAd();        // 다음 이미지로 교체하는 함수

    bool isVisible = true;  // 가시성 상태 변수

    int currentOrderType = 0; // 0: 매장, 1: 포장
    // int orderNumber = 100;  // 주문번호 (100번부터)

    void handle(const KioskEvent &event); // 이벤트를 처리할 핸들 함수
    // void loadMenus(const QString &categoryName);    // 메뉴판을 채우는 함수
    // void updateCartTable();
    // void processCheckout();
    // void changeCartQuantity(const QString &menuName, int delta);    // 수량 변경
};

#endif // MAINWINDOW_H
