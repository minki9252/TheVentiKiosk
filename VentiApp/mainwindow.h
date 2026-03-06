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
// 🌟 사용자가 최종적으로 담게 될 주문 아이템 구조체
struct OrderItem {
    int menuId;              // 메뉴 고유 번호
    QString menuName;        // 메뉴 이름 (예: 아메리카노)
    int basePrice;           // 메뉴 기본 가격

    // 🔍 추가되어야 할 상세 옵션들
    QString beanType;        // 원두 선택 (마일드, 다크, 디카페인)
    QString size;            // 사이즈 (기본, 라지 등)
    int extraShots = 0;      // 샷 추가 개수
    QStringList toppings;    // 선택된 토핑 리스트 (펄, 자바칩 등)
    QString whipAmount;      // 휘핑크림 양 (제외, 적게, 보통, 많이)

    // 💰 계산 로직
    int optionPrice = 0;     // 옵션으로 인해 추가된 금액
    int totalPrice = 0;      // (basePrice + optionPrice) * quantity
    int quantity = 1;        // 수량
};

// 🌟 신호 전달용 KioskEvent (수정안)
struct KioskEvent {
    KioskAction action;
    int menuId;
    // 단순한 신호 전달을 넘어 모달에 필요한 "카테고리 정보"를 함께 넘기면 좋습니다.
    QString categoryName;    // "Coffee", "Beverage" 등 (모달 UI 결정용)
    QVariant extraData;
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
    void setup_db();
    QSqlDatabase db;
    // 🌟 파라미터를 구조체 하나로 변경
    void handle(const KioskEvent &event); 
};

#endif // MAINWINDOW_H
