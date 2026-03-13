#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "databasemanager.h"
#include "categorywidget.h"
#include "beverage.h"  // 🌟 beverage 위젯 기능을 쓰기 위해 필수
#include "KioskData.h" // 🌟 메뉴 상세 정보 구조체 사용
#include "paymentmaindialog.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QTimer>
#include <QListWidgetItem>
#include <QIcon>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // DB 초기화
    if (!DatabaseManager::instance().initDatabase("venti.db"))
    {
        qDebug() << "DB 연결 실패!";
    }
    else
    {
        // DB가 열리면 테이블을 만들고 메뉴를 집어넣으라는 명령
        DatabaseManager::instance().setupDatabase();
    }

    // 초기화면 설정
    ui->introButton->setStyleSheet("border-image: url(:/Benner/Image/Benner/g_dragon.png); border: none;");

    // 안내 문구 깜빡임 타이머
    touchTimer = new QTimer(this);
    connect(touchTimer, &QTimer::timeout, this, &MainWindow::toggleTouchText);
    touchTimer->start(700);

    // 메뉴판 위젯 데이터 수신 연결
    connect(ui->widget_2, &beverage::sendToCart, this, &MainWindow::onReceiveCartData);

    // 장바구니 위젯 데이터 수신 연결
    connect(ui->Listcart, &cartwidget::checkoutRequested, this, &MainWindow::processCheckout);

    // KioskEvent를 던지는 이 딱 한 줄의 connect문이 모든 카테고리 버튼을 처리함
    connect(ui->categoryWidget, &categorywidget::categorySelected, this, [this](int actionCode)
            {
        KioskEvent event;
        event.action = static_cast<KioskAction>(actionCode);
        handle(event); });

    ui->stackedWidget->setCurrentIndex(0); // 시작은 홍보화면
}

MainWindow::~MainWindow() { delete ui; }

// 데이터 수신 확인용 슬롯
void MainWindow::onReceiveCartData(QList<KioskData> list)
{
    ui->Listcart->updateCart(list); // 냅다 던져줍니다.
}

void MainWindow::processCheckout()
{
    QList<KioskData> currentCart = ui->Listcart->getCartList();
    int currentTotal = ui->Listcart->getTotalAmount();

    // 장바구니 데이터와 총 결제 금액을 결제 메인 창으로 넘겨줍니다.
    PaymentMainDialog payDialog(currentCart, currentTotal, this);

    // 2. 결제 진행
    if (payDialog.exec() == QDialog::Accepted)
    {
        qDebug() << "최종 결제가 완료되었습니다! DB 저장 단계로 넘어갑니다.";

        // (이후 DB 저장, 장바구니 비우기 로직 수행)
        ui->Listcart->clearCart();
        currentOrderType = 0;
        //처음화면으로 돌아가기
        ui->stackedWidget->setCurrentIndex(0);
    }
    else
    {
        qDebug() << "결제가 취소되었습니다.";
    }
}

// 안내 문구 깜빡임 로직
void MainWindow::toggleTouchText()
{
    if (ui->lblTouchNotice)
    {
        if (isVisible)
        {
            ui->lblTouchNotice->setStyleSheet("color: rgba(93, 45, 145, 0); font-size: 20pt; font-weight: bold;");
        }
        else
        {
            ui->lblTouchNotice->setStyleSheet("color: rgba(93, 45, 145, 255); font-size: 20pt; font-weight: bold;");
        }
        isVisible = !isVisible;
    }
}

// 선택 화면 매장 or 포장 함수
void MainWindow::on_introButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1); // 매장 or 포장 선택 페이지로
}

void MainWindow::on_storeButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    ui->widget_2->loadMenus("신메뉴");
}

void MainWindow::on_takeoutButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    ui->widget_2->loadMenus("신메뉴");
}


/////////////////////// 핸들 함수 시작 //////////////////////////////////////
void MainWindow::handle(const KioskEvent &event)
{
    switch (event.action)
    {

        /////////////////// 카테고리 설정 //////////////////////////////
    case CATEGORY_NEW_MENU:
        qDebug() << "신메뉴 카테고리 선택됨";
        ui->widget_2->loadMenus("신메뉴");
        break;
    case CATEGORY_ICED_COFFEE:
        qDebug() << "아이스커피 카테고리 선택됨";
        ui->widget_2->loadMenus("커피(아이스)");
        break;
    case CATEGORY_HOT_COFFEE:
        qDebug() << "핫커피 카테고리 선택됨";
        ui->widget_2->loadMenus("커피(핫)");
        break;
    case CATEGORY_TEA:
        qDebug() << "티 카테고리 선택됨";
        ui->widget_2->loadMenus("버블티&티");
        break;
    case CATEGORY_BLENDED:
        qDebug() << "블렌디드 카테고리 선택됨";
        ui->widget_2->loadMenus("블렌디드");
        break;
    case CATEGORY_BEVERAGE:
        qDebug() << "베버리지 카테고리 선택됨";
        ui->widget_2->loadMenus("베버리지");
        break;
    case CATEGORY_ADE:
        qDebug() << "에이드 카테고리 선택됨";
        ui->widget_2->loadMenus("에이드");
        break;
    case GO_HOME:
        qDebug() << "처음 화면으로";
        // 🌟 추가된 부분: 홈 화면으로 갈 때 장바구니와 주문 상태를 싹 비워줍니다.
        ui->Listcart->clearCart();
        currentOrderType = 0;
        ui->stackedWidget->setCurrentIndex(0);
        break;

    }
}
////////////////////////////// 핸들 함수 끝 //////////////////////////////////////
