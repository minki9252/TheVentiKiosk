#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QButtonGroup> // 🌟 [추가] 버튼 그룹 관리를 위해 필요
#include <QVariant>
#include <QSqlDatabase>
#include <QTimer>
#include <QMap>
#include "KioskData.h" // 🌟 [추가] 공통 구조체(KioskEvent) 파일 사용
#include "cartwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

enum KioskAction
{
    CATEGORY_NEW_MENU,
    CATEGORY_ICED_COFFEE,
    CATEGORY_HOT_COFFEE,
    CATEGORY_DECAFF,
    CATEGORY_LOWCAL,
    CATEGORY_BUBBLE,
    CATEGORY_JUICE,
    CATEGORY_BLENDED,
    CATEGORY_TEA,
    MENU_SELECT_ITEM,
    MENU_CANCEL_ITEM,
    CART_ADD,
    CART_REMOVE,
    CART_CHECKOUT
};

// 🌟 이벤트를 담을 구조체 정의
struct KioskEvent
{
    KioskAction action;
    int menuId = -1;     // 선택한 메뉴의 DB ID
    int quantity = 1;    // 수량 (기본값 1)
    QString option = ""; // ICE/HOT 등 옵션 정보
    QVariant extraData;  // 기타 범용 데이터가 필요할 때를 대비
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
    // 🌟 [수정] 인자 타입을 KioskData로 변경 (beverage에서 보낼 타입)
    void onReceiveCartData(QList<KioskData> list);
    // 🌟 [추가] 옵션 버튼이 눌렸을 때 가격을 실시간으로 계산할 슬롯

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    QTimer *touchTimer; // 타이머 객체
    QMap<QString, int> cartData;

    // 🌟 [추가] 배타적 선택(하나만 선택)을 위한 버튼 그룹
    QButtonGroup *sizeGroup; // 라지, 점보 등 사이즈 그룹
    QButtonGroup *beanGroup; // 원두 선택 그룹 (필요 시)

    // 🌟 [추가] 장바구니에 담길 '진짜 데이터' 보관함
    QList<KioskData> m_cartList;

    bool isVisible = true;                       // 가시성 상태 변수
    int currentOrderType = 0;                    // 0: 매장, 1: 포장
    void handle(const KioskEvent &event);        // 이벤트를 처리할 핸들 함수
    void loadMenus(const QString &categoryName); // 메뉴판을 채우는 함수
    void updateCartTable();
    void processCheckout();
    // 🌟 [추가] 현재 최종 합산 금액을 UI에 업데이트하는 함수
    void calculateTotalPrice();
};

#endif // MAINWINDOW_H
