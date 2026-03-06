#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QSqlError>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)  
{
    ui->setupUi(this);

    // mainwindow.ui에서 파일 선택후 경로 설정
    QString imagePath = ":/G-dragon/벤티홍보.jpg";
    ui->introButton->setStyleSheet(QString("border-image: url(%1); border: none;").arg(imagePath));

    // 프로그램 시작 시 무조건 0번 페이지부터 보여줌
    ui->stackedWidget->setCurrentIndex(0);

    setup_db();

    // 🌟 분리한 connect 전용 함수 호출
    setup_connections(); 
}

MainWindow::~MainWindow()
{
    delete ui;
}

// ... (기존 on_introButton_clicked 등 페이지 이동 함수들 생략) ...
// ... (기존 setup_db 함수 생략) ...

// =====================================================================
// 🌟 새롭게 분리한 이벤트 연결 함수
// =====================================================================
void MainWindow::setup_connections() 
{
    // 1. 카테고리 버튼 연결
    connect(ui->btnCategoryCoffee, &QPushButton::clicked, this, [this](){
        KioskEvent ev;
        ev.action = CATEGORY_COFFEE;
        handle(ev);
    });

    connect(ui->btnCategoryBeverage, &QPushButton::clicked, this, [this](){
        KioskEvent ev;
        ev.action = CATEGORY_BEVERAGE;
        handle(ev);
    });

    connect(ui->btnCategoryDessert, &QPushButton::clicked, this, [this](){
        KioskEvent ev;
        ev.action = CATEGORY_DESSERT;
        handle(ev);
    });

    // 2. 메뉴 선택 버튼 연결 (예시)
    connect(ui->btnMenuAmericano, &QPushButton::clicked, this, [this](){
        KioskEvent ev;
        ev.action = MENU_SELECT_ITEM;
        ev.menuId = 1; 
        handle(ev);
    });

    // 3. 장바구니 관련 버튼 연결
    connect(ui->btnAddToCart, &QPushButton::clicked, this, [this](){
        KioskEvent ev;
        ev.action = CART_ADD;
        ev.menuId = 1;         
        ev.quantity = 1;       
        ev.option = "ICE";     
        handle(ev);
    });

    connect(ui->btnRemoveFromCart, &QPushButton::clicked, this, [this](){
        KioskEvent ev;
        ev.action = CART_REMOVE;
        handle(ev);
    });

    // 4. 결제 버튼 연결
    connect(ui->btnCheckout, &QPushButton::clicked, this, [this](){
        KioskEvent ev;
        ev.action = CART_CHECKOUT;
        handle(ev);
    });
}

// ... (기존 handle 함수 생략) ...