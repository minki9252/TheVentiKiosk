#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVariant>
#include <QSqlDatabase>
#include <QListWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

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
    void loadCategoriesToUI();
    void on_listMenu_itemClicked(QListWidgetItem *item);
    void updateMenuDisplay(const QString &categoryName);
    void updateTotalAmount(int amount);
    void changeCartQuantity(const QString &menuName, int delta);

    QWidget* createQuantityWidget(int row, int initialQty);

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    QTimer *touchTimer;     // 타이머 객체

    bool isVisible = true;  // 가시성 상태 변수

    int currentOrderType = 0; // 0: 매장, 1: 포장
    int orderNumber = 100;  // 주문번호 (100번부터)
    int totalAmount = 0; // 전체 합계를 저장할 변수

    void loadMenus(const QString &categoryName);    // 메뉴판을 채우는 함수
    void updateCartTable();
    void processCheckout();
};

#endif // MAINWINDOW_H
