#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "databasemanager.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)  
{

    ui->setupUi(this);

    // [위치: 생성자 내부]
    // UI가 뜨기 전 혹은 직후에 DB 매니저를 통해 세팅을 끝냅니다.
    if(!DatabaseManager::instance().initDatabase("venti.db")) {
        qDebug() << "DB 연결 실패!";
    }

    // mainwindow.ui에서 파일 선택후 경로 설정
    QString imagePath = ":/G-dragon/벤티홍보.jpg";

    // 버튼의 배경으로 이미지 넣기 (CSS 이용)
    ui->introButton->setStyleSheet(QString("border-image: url(%1); border: none;").arg(imagePath));

    // 프로그램 시작 시 무조건 0번 페이지(홍보 모델)부터 보여줌
    ui->stackedWidget->setCurrentIndex(0);

    // 나중에 Qt Creator에서 만든 버튼을 클릭했을 때 handle 함수를 호출하는 예시 (C++11 람다 사용)
    // connect(ui->btnCoffee, &QPushButton::clicked, this, [this](){ handle(CATEGORY_COFFEE); });
}

MainWindow::~MainWindow()
{
    delete ui;
}

// 홍보 모델 이미지 버튼 클릭 시
void MainWindow::on_introButton_clicked()
{
    // 0번 페이지(초기화면)에서 1번 페이지(선택화면)로 전환
    ui->stackedWidget->setCurrentIndex(1);
    qDebug() << "초기화면 -> 선택화면 이동 완료";
}

// 매장 버튼 클릭 시
void MainWindow::on_storeButton_clicked()
{
    // 1번 페이지(선택화면)에서 2번 페이지(메뉴화면)로 전환
    ui->stackedWidget->setCurrentIndex(2);
    qDebug() << "매장 선택 -> 메뉴판 이동 완료";
}

// 포장 버튼 클릭 시
void MainWindow::on_takeoutButton_clicked()
{
    // 2번 페이지(메뉴화면)로 이동
    ui->stackedWidget->setCurrentIndex(2);
    qDebug() << "포장 선택 -> 메뉴판 이동 완료";
}

/////////////////////// 핸들 함수 시작 //////////////////////////////////////
void MainWindow::handle(const KioskEvent &event) {

    switch(event.action){

        /////////////////// 카테고리 설정 //////////////////////////////
    case CATEGORY_COFFEE:
        qDebug() << "커피 카테고리 선택됨";
        // TODO: 커피 메뉴 리스트를 UI에 출력하는 로직



        break;

    case CATEGORY_BEVERAGE:
        qDebug() << "음료 카테고리 선택됨";
        break;

    case CATEGORY_DESSERT:
        qDebug() << "디저트 카테고리 선택됨";
        break;
        /////////////////// 카테고리 설정 끝 //////////////////////////////


        /////////////////// 메뉴 선택 설정 //////////////////////////////
    case MENU_SELECT_ITEM:
        qDebug() << "메뉴가 선택됨";
        // TODO: 선택된 메뉴의 상세 정보 팝업 띄우기 또는 임시 변수에 저장
        break;

    case MENU_CANCEL_ITEM:
        qDebug() << "메뉴 선택 취소됨";
        break;
        /////////////////// 메뉴 선택 끝 //////////////////////////////


       /////////////////// 장바구니 설정 //////////////////////////////
    case CART_ADD:
        m_cartManager.addItem(event); // 로직은 관리자에게 위임
        refreshCartUI();              // 나는 UI만 다시 그림
        break;

    case CART_INCREASE_QTY:
        m_cartManager.updateQty(event.extraData.toInt(), 1);
        refreshCartUI();
        break;

    case CART_DECREASE_QTY:
        m_cartManager.updateQty(event.extraData.toInt(), -1);
        refreshCartUI();
        break;

    case CART_REMOVE:
        m_cartManager.removeItem(event.extraData.toInt());
        refreshCartUI();
        break;

    case CART_CLEAR_ALL:
        m_cartManager.clear();
        refreshCartUI();
        break;

    case CART_CHECKOUT:
        // 결제 로직 등...
        break;
        /////////////////// 장바구니 설정 끝 //////////////////////////////

    default:
        qDebug() << "정의되지 않은 액션입니다.";
        break;
    }
}
////////////////////////////// 핸들 함수 끝 //////////////////////////////////////

// 🌟 UI 갱신은 MainWindow가 담당합니다.
void MainWindow::refreshCartUI() {
    // 1. m_cart 객체에서 최신 리스트와 총액을 가져옵니다.
    const QList<KioskEvent>& currentList = m_cart.getList();
    int totalAmount = m_cart.getTotalPrice();

    // 2. TODO: ui->cartListWidget->clear() 등 기존 UI 위젯 초기화

    // 3. TODO: currentList를 반복문으로 돌면서 UI 위젯에 아이템 그리기
    for(const auto& item : currentList) {
        // 예: QString text = QString("%1 %2원").arg(item.menuName).arg(item.totalPrice);
    }

    // 4. 하단 총 결제금액 라벨 업데이트 
    // ui->lblTotalAmount->setText(QString::number(totalAmount) + "원");
    
    qDebug() << "현재 장바구니 갱신 완료 - 총 금액:" << totalAmount << "원";
}