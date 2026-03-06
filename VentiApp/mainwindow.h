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
    // --- 카테고리 선택 관련 ---
    CATEGORY_COFFEE,      // 커피 탭 클릭
    CATEGORY_BEVERAGE,    // 음료 탭 클릭
    CATEGORY_DESSERT,     // 디저트 탭 클릭

    // --- 메뉴 및 상세 옵션 관련 ---
    MENU_SELECT_ITEM,     // [신호 1] 메뉴 버튼 클릭 (상세 옵션 모달을 띄우는 용도)
    MENU_CANCEL_ITEM,     // 모달 창에서 취소/닫기 클릭

    // --- 장바구니 제어 관련 ---
    CART_ADD,             // [신호 2] 모달에서 '담기(Add to Cart)' 클릭 (구조체 정보와 함께 전달)
    CART_REMOVE,          // 장바구니 리스트에서 특정 항목 삭제
    CART_CHECKOUT         // 최종 결제 진행 신호
};

struct KioskEvent {
    // 1. 시스템 제어 신호
    // 어떤 종류의 이벤트가 발생했는지 식별 (예: 메뉴 선택, 장바구니 담기, 결제 등)
    KioskAction action;

    // 2. 메뉴 기본 데이터 (DB 정보)
    // 버튼 클릭 시점에 DB에서 가져온 변하지 않는 음료의 고유 정보입니다.
    int menuId = -1;         // DB 식별자 (ID)
    QString menuName;        // 음료 명칭
    QString categoryName;    // 카테고리 (Coffee, Tea 등)
    int basePrice = 0;       // 옵션이 추가되지 않은 순수 음료 가격

    // 3. 사용자 커스텀 옵션 (변동 정보)
    // 모달창에서 사용자가 '더 벤티' 특성에 맞춰 선택한 세부 설정값들입니다.
    QString beanType;        // 선택한 원두 (마일드/다크/디카페인)
    QString size;            // 컵 사이즈 (기본/라지)
    int extraShots = 0;      // 추가된 에스프레소 샷 횟수
    QStringList toppings;    // 추가된 토핑 리스트 (펄, 자바칩, 코코젤리 등 다중 선택)
    QString whipAmount;      // 휘핑크림 설정 (제외/적게/보통/많이)

    // 4. 주문 계산 정보
    // 수량과 옵션 가격을 합산한 최종 결과값으로, 장바구니 UI에 표시될 실시간 데이터입니다.
    int quantity = 1;        // 주문할 잔 수
    int optionPrice = 0;     // 선택한 옵션(샷 추가, 사이즈 업 등)으로 인해 추가된 금액
    int totalPrice = 0;      // 최종 결제 금액: (basePrice + optionPrice) * quantity

    // 5. 유연한 확장성
    // 위 항목에 없는 예외적인 데이터(이벤트 쿠폰 번호, 특이사항 등)를 담는 만능 보관함입니다.
    QVariant extraData;
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
    void setup_connections();
    void setup_db();
    QSqlDatabase db;
    // 🌟 파라미터를 구조체 하나로 변경
    void handle(const KioskEvent &event); 
};

#endif // MAINWINDOW_H
