#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

// 키오스크에서 발생할 수 있는 액션들을 열거형으로 정의합니다.
enum KioskAction {
    // 카테고리 설정
    CATEGORY_COFFEE,
    CATEGORY_BEVERAGE,
    CATEGORY_DESSERT,

    // 메뉴 선택
    MENU_SELECT_ITEM,
    MENU_CANCEL_ITEM,

    // 장바구니 설정
    CART_ADD,
    CART_REMOVE,
    CART_CHECKOUT
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_introButton_clicked();    // 페이지 0 -> 1 이동
    void on_storeButton_clicked();    // 페이지 1 -> 2 이동 (매장 선택)
    void on_takeoutButton_clicked();  // 페이지 1 -> 2 이동 (포장 선택)

private:
    Ui::MainWindow *ui;

    void setup_db();
    void handle(KioskAction action); // 이벤트를 처리할 핸들 함수
};

#endif // MAINWINDOW_H
