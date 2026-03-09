#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "databasemanager.h"
#include <QDebug>
#include <QTimer>
// #include <QListWidgetItem>
// #include <QIcon>
// #include <utility>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // DB 초기화
    if(!DatabaseManager::instance().initDatabase("venti.db")) {
        qDebug() << "DB 연결 실패!";
    }

    // 초기화면 설정
    ui->introButton->setStyleSheet("border-image: url(:/G-dragon/벤티홍보.jpg); border: none;");

    // 안내 문구 깜빡임 타이머
    touchTimer = new QTimer(this);
    connect(touchTimer, &QTimer::timeout, this, &MainWindow::toggleTouchText);
    touchTimer->start(700);

    // // 리스트 위젯 강제 적용
    // ui->listMenu->setViewMode(QListWidget::IconMode);
    // ui->listMenu->setMovement(QListWidget::Static);
    // ui->listMenu->setResizeMode(QListWidget::Adjust);
    // ui->listMenu->setSpacing(20);
    // ui->listMenu->setIconSize(QSize(150, 150));
    // ui->listMenu->setStyleSheet(
    //     "QListWidget { border: none; background-color: white; }"
    //     "QListWidget::item { background-color: #f8f8f8; border-radius: 15px; margin: 5px; padding: 10px; }"
    //     "QListWidget::item:selected { border: 2px solid #5d2d91; background-color: #f0eaf6; }"
    //     );

    // // 카테고리 버튼들 연결
    // connect(ui->pushButton,   &QPushButton::clicked, this, [this](){ loadMenus("신메뉴"); });
    // connect(ui->pushButton_2, &QPushButton::clicked, this, [this](){ loadMenus("커피"); });
    // connect(ui->pushButton_3, &QPushButton::clicked, this, [this](){ loadMenus("버블티&티"); });
    // connect(ui->pushButton_4, &QPushButton::clicked, this, [this](){ loadMenus("블렌디드"); });
    // connect(ui->pushButton_5,   &QPushButton::clicked, this, [this](){ loadMenus("베버리지"); });
    // connect(ui->pushButton_6,   &QPushButton::clicked, this, [this](){ loadMenus("에이드"); });

    // connect(ui->listMenu, &QListWidget::itemClicked, this, [this](QListWidgetItem *item){
    //     // 클릭한 아이템에서 메뉴 이름만 분리 (줄바꿈 기준 첫 줄)
    //     QString menuName = item->text().split("\n")[0];

    //     // 장바구니 데이터에 추가 (이미 있으면 수량 +1, 없으면 1로 시작)
    //     cartData[menuName]++;

    //     // UI 테이블 갱신
    //     updateCartTable();

    //     qDebug() << menuName << "이(가) 장바구니에 담겼습니다. 현재 수량:" << cartData[menuName];
    // });

    // connect(ui->btnClearCart, &QPushButton::clicked, this, [this](){
    //     // 장바구니 데이터를 모두 지우고 테이블을 갱신
    //     cartData.clear();
    //     updateCartTable();
    //     qDebug() << "장바구니가 비워졌습니다.";
    // });

    // // 결제하기 버튼 연결
    // connect(ui->btnCheckout, &QPushButton::clicked, this, [this](){
    //     if (cartData.isEmpty()) {
    //         qDebug() << "장바구니가 비어있어 결제할 수 없습니다.";
    //         return;
    //     }


    //     processCheckout();
    // });

    // // 테이블의 특정 셀을 더블클릭하면 해당 항목 삭제
    // connect(ui->tableCart, &QTableWidget::cellDoubleClicked, this, [this](int row, int column){
    //     (void)column;

    //     QString menuName = ui->tableCart->item(row, 0)->text();


    //     cartData.remove(menuName);
    //     updateCartTable();

    //     qDebug() << menuName << " 항목이 삭제되었습니다.";
    // });

    ui->stackedWidget->setCurrentIndex(0); // 시작은 홍보화면
}

MainWindow::~MainWindow() { delete ui; }

// // 메뉴 로딩 로직
// void MainWindow::loadMenus(const QString &categoryName) {
//     ui->listMenu->clear(); // 기존 아이템 삭제

//     QList<QVariantMap> menus = DatabaseManager::instance().getMenusByCategory(categoryName);

//     for (const QVariantMap &menuData : std::as_const(menus)) {
//         QString itemText = QString("%1\n%2원")
//                                .arg(menuData["name"].toString())
//                                .arg(menuData["price"].toInt());

//         QListWidgetItem *item = new QListWidgetItem();
//         item->setText(itemText);

//         // DB에서 경로를 가져온 후 양끝 공백을 제거(.trimmed()) 합니다.
//         QString imgPath = menuData["image"].toString().trimmed();

//         // 디버깅용: 실제 어떤 경로를 찾고 있는지 콘솔에 찍어봅니다.
//         qDebug() << "이미지 로드 시도:" << imgPath;

//         QIcon icon(imgPath);
//         if (icon.isNull()) {
//             qDebug() << "경고: 이미지를 찾을 수 없음 ->" << imgPath;
//             // 이미지를 못 찾을 때를 대비해 기본 아이콘을 넣거나 빈 공간을 확보합니다.
//         } else {
//             item->setIcon(icon);
//         }

//         item->setTextAlignment(Qt::AlignCenter);
//         ui->listMenu->addItem(item);
//     }
// }

// 안내 문구 깜빡임 로직
void MainWindow::toggleTouchText()
{
    if (ui->lblTouchNotice) {
        if (isVisible) {
            ui->lblTouchNotice->setStyleSheet("color: rgba(93, 45, 145, 0); font-size: 20pt; font-weight: bold;");
        } else {
            ui->lblTouchNotice->setStyleSheet("color: rgba(93, 45, 145, 255); font-size: 20pt; font-weight: bold;");
        }
        isVisible = !isVisible;
    }
}

// 버튼 클릭 이벤트 함수
void MainWindow::on_introButton_clicked() {
    ui->stackedWidget->setCurrentIndex(1); // 매장/포장 선택 페이지로
}

void MainWindow::on_storeButton_clicked() {
    currentOrderType = 0; // 매장
    ui->stackedWidget->setCurrentIndex(2); // 메뉴판 페이지로
    // loadMenus("신메뉴"); // 첫 화면 로딩
}

void MainWindow::on_takeoutButton_clicked() {
    currentOrderType = 1; // 포장
    ui->stackedWidget->setCurrentIndex(2);
    // loadMenus("신메뉴");
}

// void MainWindow::updateCartTable() {
//     // 기존 테이블 내용 싹 비우기
//     ui->tableCart->setRowCount(0);

//     int row = 0;
//     int totalAmount = 0;

//     // 장바구니 맵을 순회하며 테이블에 한 줄씩 추가
//     for (auto it = cartData.begin(); it != cartData.end(); ++it) {
//         QString name = it.key();
//         int count = it.value();

//         int pricePerItem = 2000; // 일단 예시로 2000원 고정
//         int subTotal = pricePerItem * count;

//         ui->tableCart->insertRow(row);
//         ui->tableCart->setItem(row, 0, new QTableWidgetItem(name));
//         ui->tableCart->setItem(row, 1, new QTableWidgetItem(QString::number(count)));
//         ui->tableCart->setItem(row, 2, new QTableWidgetItem(QString::number(subTotal) + "원"));

//         totalAmount += subTotal;void processCheckout();      // 결제 처리 로직
//         row++;
//     }
//     ui->lblTotal->setText(QString("총 결제 금액: %1원").arg(totalAmount));
// }

// void MainWindow::processCheckout() {
//     qDebug() << "결제 진행 중... 총 항목 수:" << cartData.size();
//     qDebug() << "결제가 완료되었습니다! 초기 화면으로 돌아갑니다.";

//     // 결제 완료 후 장바구니 비우기 및 초기 화면으로 이동
//     cartData.clear();
//     updateCartTable();
//     ui->stackedWidget->setCurrentIndex(0);
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
