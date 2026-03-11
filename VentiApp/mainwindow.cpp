#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "databasemanager.h"
#include "categorywidget.h"
#include "beverage.h"  // 🌟 beverage 위젯 기능을 쓰기 위해 필수
#include "KioskData.h" // 🌟 메뉴 상세 정보 구조체 사용
#include "coupondialog.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QTimer>
#include <QListWidgetItem>
#include <QIcon>
// #include <utility>

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
        // 추가: DB가 열리면 테이블을 만들고 메뉴를 집어넣으라는 명령입니다.
        DatabaseManager::instance().setupDatabase();
    }

    // 초기화면 설정
    ui->introButton->setStyleSheet("border-image: url(:/Benner/Image/Benner/g_dragon.png); border: none;");

    // 안내 문구 깜빡임 타이머
    touchTimer = new QTimer(this);
    connect(touchTimer, &QTimer::timeout, this, &MainWindow::toggleTouchText);
    touchTimer->start(700);

    // 4. 🌟 [핵심 연결] 메뉴판(beverage) 위젯 데이터 수신 연결
    // ui->widget_2가 beverage 클래스로 프로모션 되어 있어야 신호를 잡을 수 있습니다.
    connect(ui->widget_2, &beverage::sendToCart, this, &MainWindow::onReceiveCartData);
    
    // 장바구니 -> MainWindow 결제 신호 연결
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

// 🌟 데이터 수신 확인용 슬롯: 관리나 출력 없이 전달받은 정보만 확인
void MainWindow::onReceiveCartData(QList<KioskData> list)
{
    ui->Listcart->updateCart(list); // 냅다 던져줍니다.
}

// 🌟 결제 처리 로직
void MainWindow::processCheckout()
{
    qDebug() << "결제가 완료되었습니다! 초기 화면으로 돌아갑니다.";
    
    // 1. 여기서 DB에 주문 내역 저장(INSERT) 등의 로직을 나중에 추가하시면 됩니다.

    // 2. 결제가 완료되었으니 장바구니 비우기
    ui->Listcart->clearCart();
    
    // 3. 주문 방식(매장/포장) 변수 초기화 및 초기 홍보 화면으로 이동
    currentOrderType = 0; 
    ui->stackedWidget->setCurrentIndex(0); 
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

// 버튼 클릭 이벤트 함수
void MainWindow::on_introButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1); // 매장/포장 선택 페이지로
}

void MainWindow::on_storeButton_clicked()
{
    currentOrderType = 0;                  // 매장
    ui->stackedWidget->setCurrentIndex(2); // 메뉴판 페이지로
    // loadMenus("신메뉴"); // 첫 화면 로딩
}

void MainWindow::on_takeoutButton_clicked()
{
    currentOrderType = 1; // 포장
    ui->stackedWidget->setCurrentIndex(2);
    // loadMenus("신메뉴");
}

// void MainWindow::processCheckout()
// {
//     // 장바구니가 비어있는지 체크는 이미 cartwidget에서 하고 여기로 넘어옵니다.

//     // 1. 쿠폰 모달창 생성 및 띄우기
//     CouponDialog dialog(this);

//     // exec()는 창이 닫힐 때까지 코드를 멈추고 기다립니다.
//     // 사용자가 accept()로 창을 닫았다면 결제를 진행합니다.
//     if (dialog.exec() == QDialog::Accepted) {

//         qDebug() << "결제가 완료되었습니다! 초기 화면으로 돌아갑니다.";

//         // 2. 결제 완료되었으니 장바구니 비우기
//         ui->Listcart->clearCart();

//         // 3. 주문 방식 초기화 및 첫 화면으로 이동
//         currentOrderType = 0;
//         ui->stackedWidget->setCurrentIndex(0);
//     }
//     // 창을 강제로 끄거나 거부(reject)했다면 아무 일도 일어나지 않고 원래 메뉴판에 머뭅니다.
// }


// void MainWindow::updateCartTable() {
//     ui->tableCart->setRowCount(0);

//     // 모든 행의 높이를 50으로 고정
//     ui->tableCart->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
//     ui->tableCart->verticalHeader()->setDefaultSectionSize(50);

//     // 마지막 행이 남은 빈 공간을 채우지 않도록 설정
//     ui->tableCart->verticalHeader()->setStretchLastSection(false);

//     // 테이블 행 번호 안 보이게
//     ui->tableCart->verticalHeader()->setVisible(false);

//     int row = 0;
//     int totalAmount = 0;

//     for (auto it = cartData.begin(); it != cartData.end(); ++it) {
//         QString name = it.key();
//         int count = it.value();

//         // DB에서 가격 가져오기
//         QSqlQuery query;
//         query.prepare("SELECT price FROM MENU_INFO WHERE kr_name = :name");
//         query.bindValue(":name", name.trimmed()); // 공백 제거 후 조회

//         int realPrice = 0;
//         if (query.exec() && query.next()) {
//             realPrice = query.value(0).toInt();
//         }

//         int subTotal = realPrice * count;
//         totalAmount += subTotal;

//         ui->tableCart->insertRow(row);

//         QTableWidgetItem *nameItem = new QTableWidgetItem(name);
//         nameItem->setTextAlignment(Qt::AlignCenter);
//         ui->tableCart->setItem(row, 0, nameItem);


//         QWidget *pWidget = new QWidget();
//         QHBoxLayout *pLayout = new QHBoxLayout(pWidget);
//         QPushButton *btnMinus = new QPushButton("-");
//         QPushButton *btnPlus = new QPushButton("+");
//         QLabel *lblCount = new QLabel(QString::number(count));

//         btnMinus->setFixedSize(30, 30);
//         btnPlus->setFixedSize(30, 30);
//         lblCount->setAlignment(Qt::AlignCenter);
//         lblCount->setStyleSheet("font-weight: bold;");

//         pLayout->addWidget(btnMinus);
//         pLayout->addWidget(lblCount);
//         pLayout->addWidget(btnPlus);
//         pLayout->setContentsMargins(5, 0, 5, 0);
//         pLayout->setSpacing(10);
//         pWidget->setLayout(pLayout);

//         // 버튼 클릭 시 수량 변경
//         connect(btnMinus, &QPushButton::clicked, this, [this, name](){ changeCartQuantity(name, -1); });
//         connect(btnPlus, &QPushButton::clicked, this, [this, name](){ changeCartQuantity(name, 1); });

//         ui->tableCart->setCellWidget(row, 1, pWidget);

//         QTableWidgetItem *priceItem = new QTableWidgetItem(QString::number(subTotal) + "원");
//         priceItem->setTextAlignment(Qt::AlignCenter);
//         ui->tableCart->setItem(row, 2, priceItem);

//         row++;
//     }

//     // 총 결제 금액
//     if(ui->lblTotal) ui->lblTotal->setText(QString("총 결제 금액: %1원").arg(totalAmount));
// }

// void MainWindow::processCheckout() {
//     orderNumber++; // 주문번호 증가
//     int totalAmount = 0;

//     //  DB에서 정보를 불러와 합산
//     for (int i = 0; i < ui->tableCart->rowCount(); ++i) {

//         QString priceText = ui->tableCart->item(i, 2)->text();

//         int price = priceText.replace("원", "").trimmed().toInt();
//         totalAmount += price;
//     }

//     // 메시지 박스 설정
//     QMessageBox msgBox(this);
//     msgBox.setWindowTitle("결제 확인");

//     // 영수증 및 결제금액 창
//     QString msg = QString("주문번호 : %1\n"
//                           "총 결제 금액 : %2원\n\n"
//                           "결제를 완료하시겠습니까?\n"
//                           "영수증 출력을 원하시면 아래 버튼을 눌러주세요.")
//                       .arg(orderNumber)
//                       .arg(totalAmount);
//     msgBox.setText(msg);

//     // 버튼 및 아이콘 설정
//     QPushButton *btnReceipt = msgBox.addButton("영수증 출력", QMessageBox::AcceptRole);
//     QPushButton *btnNoReceipt = msgBox.addButton("출력 안 함", QMessageBox::RejectRole);
//     QPushButton *btnCancel = msgBox.addButton("결제 취소", QMessageBox::DestructiveRole);

//     msgBox.setIcon(QMessageBox::Information);
//     msgBox.exec();

//     // 결과 처리
//     if (msgBox.clickedButton() == btnCancel) {
//         return; // 결제 취소 시 함수 종료
//     }

//     // 결제 완료
//     cartData.clear();   // 카트 초기화
//     updateCartTable();
//     ui->stackedWidget->setCurrentIndex(0); // 초기화면으로 이동
// }

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

