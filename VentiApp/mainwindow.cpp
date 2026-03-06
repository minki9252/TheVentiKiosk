#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "databasemanager.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QTimer>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // DB 초기화: 라이브러리를 통해 venti.db 연결 및 테이블/데이터 생성
    if(!DatabaseManager::instance().initDatabase("venti.db")) {
        qDebug() << "DB 연결 실패!";
    }

    // 초기화면 홍보 이미지 설정
    QString imagePath = ":/G-dragon/벤티홍보.jpg";
    ui->introButton->setStyleSheet(QString("border-image: url(%1); border: none;").arg(imagePath));

    // 안내 문구 깜빡임 타이머 설정
    touchTimer = new QTimer(this);
    connect(touchTimer, &QTimer::timeout, this, &MainWindow::toggleTouchText);
    touchTimer->start(700);

    // 프로그램 시작 시 0번 페이지(홍보 모델) 노출
    ui->stackedWidget->setCurrentIndex(0);

    // // 신메뉴 버튼 클릭 시
    // connect(ui->pushButton, &QPushButton::clicked, this, [this](){
    //     loadMenus("신메뉴");
    //     qDebug() << "카테고리 변경: 신메뉴";
    // });

    // // 커피 버튼 클릭 시
    // connect(ui->pushButton_2, &QPushButton::clicked, this, [this](){
    //     loadMenus("커피");
    //     qDebug() << "카테고리 변경: 커피";
    // });

    // // 음료 버튼 클릭 시
    // connect(ui->pushButton_3, &QPushButton::clicked, this, [this](){
    //     loadMenus("버블티");
    //     qDebug() << "카테고리 변경: 버블";
    // });

    // // 디저트 버튼 클릭 시
    // connect(ui->pushButton_4, &QPushButton::clicked, this, [this](){
    //     loadMenus("디저트");
    //     qDebug() << "카테고리 변경: 디저트";
    // });

    // // 디저트 버튼 클릭 시
    // connect(ui->pushButton_5, &QPushButton::clicked, this, [this](){
    //     loadMenus("디저트");
    //     qDebug() << "카테고리 변경: 디저트";
    // });

    // // 디저트 버튼 클릭 시
    // connect(ui->pushButton_6, &QPushButton::clicked, this, [this](){
    //     loadMenus("디저트");
    //     qDebug() << "카테고리 변경: 디저트";
    // });
}

MainWindow::~MainWindow()
{
    delete ui;
}

// 안내 문구 깜빡임 로직
void MainWindow::toggleTouchText()
{
    if (ui->lblTouchNotice) {
        if (isVisible) {
            // 투명하게 만들기 (보라색 계열)
            ui->lblTouchNotice->setStyleSheet("color: rgba(93, 45, 145, 0); font-size: 20pt; font-weight: bold;");
        } else {
            // 불투명하게 만들기 (더벤티 브랜드 컬러)
            ui->lblTouchNotice->setStyleSheet("color: rgba(93, 45, 145, 255); font-size: 20pt; font-weight: bold;");
        }
        isVisible = !isVisible;
    }
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
    //0은 매장 1은 포장(db 반영)
    currentOrderType = 0;

    // 2번 페이지(메뉴판)로 전환
    ui->stackedWidget->setCurrentIndex(2);
    qDebug() << "매장 선택됨 -> 메뉴판으로 이동합니다.";
}

// 포장 버튼 클릭 시
void MainWindow::on_takeoutButton_clicked()
{
    currentOrderType = 1;

    ui->stackedWidget->setCurrentIndex(2);
    qDebug() << "포장 선택 -> 메뉴판 이동 완료";
}

// // 메뉴 초기화
// void MainWindow::clearMenuGrid() {
//     QLayoutItem *item;
//     // 그리드 안에 있는 모든 위젯을 찾아 삭제
//     while ((item = ui->menuGrid->takeAt(0)) != nullptr) {
//         if (item->widget()) {
//             item->widget()->deleteLater();
//         }
//         delete item;
//     }
// }

// // DB 데이터를 읽어와서 메뉴 생성
// void MainWindow::loadMenus(const QString &categoryName) {
//     clearMenuGrid(); // 이전 카테고리 메뉴 삭제

//     // DB 매니저를 통해 특정 카테고리의 메뉴 리스트를 가져오기
//     QList<QVariantMap> menus = DatabaseManager::instance().getMenusByCategory(categoryName);

//     int row = 0;
//     int col = 0;
//     const int max_columns = 3; // 한 줄에 3개씩 배치

//     for (const auto &menuData : menus) {
//         // 메뉴 하나를 담을 '컨테이너(Frame)' 생성
//         QFrame *menuCard = new QFrame();
//         QVBoxLayout *layout = new QVBoxLayout(menuCard);

//         // 메뉴 이미지 추가
//         QLabel *imgLabel = new QLabel();
//         QPixmap pix(menuData["image"].toString());
//         imgLabel->setPixmap(pix.scaled(150, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation));
//         imgLabel->setAlignment(Qt::AlignCenter);

//         // 메뉴 이름 및 가격 추가
//         QLabel *nameLabel = new QLabel(menuData["name"].toString());
//         nameLabel->setAlignment(Qt::AlignCenter);
//         nameLabel->setStyleSheet("font-weight: bold; font-size: 14pt;");

//         QLabel *priceLabel = new QLabel(QString("%1원").arg(menuData["price"].toInt()));
//         priceLabel->setAlignment(Qt::AlignCenter);
//         priceLabel->setStyleSheet("color: #5d2d91; font-weight: bold;");

//         // 레이아웃에 조립
//         layout->addWidget(imgLabel);
//         layout->addWidget(nameLabel);
//         layout->addWidget(priceLabel);

//         // 그리드 레이아웃의 (row, col) 위치에 추가
//         ui->menuGrid->addWidget(menuCard, row, col);

//         col++;
//         if (col >= max_columns) {
//             col = 0;
//             row++;
//         }
//     }
// }


/////////////////////// 핸들 함수 시작 //////////////////////////////////////
void MainWindow::handle(const KioskEvent &event) {

    switch(event.action){

        /////////////////// 카테고리 설정 //////////////////////////////
    case CATEGORY_COFFEE:
        qDebug() << "커피 카테고리 선택됨";
        // TODO: DatabaseManager::instance().getMenusByCategory("커피") 호출하여 UI 출력
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
        break;

    case MENU_CANCEL_ITEM:
        qDebug() << "메뉴 선택 취소됨";
        break;
        /////////////////// 메뉴 선택 끝 //////////////////////////////


        /////////////////// 장바구니 설정 //////////////////////////////
    case CART_ADD:
        qDebug() << "장바구니에 담기";
        break;

    case CART_REMOVE:
        qDebug() << "장바구니 항목 취소";
        break;

    case CART_CHECKOUT:
        qDebug() << "결제 진행";
        break;
        /////////////////// 장바구니 설정 끝 //////////////////////////////

    default:
        qDebug() << "정의되지 않은 액션입니다.";
        break;
    }
}
////////////////////////////// 핸들 함수 끝 //////////////////////////////////////
