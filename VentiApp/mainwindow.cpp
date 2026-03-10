#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "databasemanager.h"
// #include "menuoptiondialog.h"
#include <QMessageBox>
#include <QDebug>
#include <QTimer>
#include <QListWidgetItem>
#include <QIcon>
#include <QListWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // DB 초기화
    if(!DatabaseManager::instance().initDatabase("venti.db")) {
        qDebug() << "DB 연결 실패!";
    }

    // 안내 문구 깜빡임 타이머
    touchTimer = new QTimer(this);
    connect(touchTimer, &QTimer::timeout, this, &MainWindow::toggleTouchText);
    touchTimer->start(700);

    // // 카테고리 버튼들 연결
    // connect(ui->pushButton,   &QPushButton::clicked, this, [this](){ loadMenus("신메뉴"); });
    // connect(ui->pushButton_2, &QPushButton::clicked, this, [this](){ loadMenus("커피(핫)"); });
    // connect(ui->pushButton_7, &QPushButton::clicked, this, [this](){ loadMenus("커피(아이스)"); });
    // connect(ui->pushButton_3, &QPushButton::clicked, this, [this](){ loadMenus("버블티&티"); });
    // connect(ui->pushButton_4, &QPushButton::clicked, this, [this](){ loadMenus("블렌디드"); });
    // connect(ui->pushButton_5, &QPushButton::clicked, this, [this](){ loadMenus("베버리지"); });
    // connect(ui->pushButton_6, &QPushButton::clicked, this, [this](){ loadMenus("에이드"); });

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

//         QString imgPath = menuData["image"].toString().trimmed();

//         QIcon icon(imgPath);
//         if (icon.isNull()) {
//             qDebug() << "경고: 이미지를 찾을 수 없음 ->" << imgPath;
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
    ui->stackedWidget->setCurrentIndex(1); // 매장or포장 선택 페이지로
}

void MainWindow::on_storeButton_clicked() {
    currentOrderType = 0; // 매장 선택 저장
    ui->stackedWidget->setCurrentIndex(2); // 메뉴판 페이지(page_3)로 이동

    // loadMenus(QString("신메뉴").trimmed());
}

void MainWindow::on_takeoutButton_clicked() {
    currentOrderType = 1; // 포장 선택 저장
    ui->stackedWidget->setCurrentIndex(2); // 메뉴판 페이지로 이동

    // loadMenus(QString("신메뉴").trimmed());
}

// void MainWindow::loadCategoriesToUI() {}

// void MainWindow::updateMenuDisplay(const QString &categoryName) {
//     loadMenus(categoryName);
// }

// void MainWindow::on_listMenu_itemClicked(QListWidgetItem *item)
// {
//     // 이제 에러 없이 item 객체를 사용할 수 있습니다.
//     QString menuName = item->text();
//     qDebug() << "선택된 메뉴:" << menuName;

//     // 1. 옵션 선택 창(다이얼로그) 객체를 메모리에 만듭니다.
//     MenuOptionDialog *optionDialog = new MenuOptionDialog(this);

//     // 2. 창을 화면에 띄웁니다 (모달 방식: 이 창이 닫히기 전까지 메인창 클릭 불가)
//     // exec()가 실행되면 코드는 여기서 잠시 멈추고 사용자의 입력을 기다립니다.
//     if (optionDialog->exec() == QDialog::Accepted) {
//         // 사용자가 다이얼로그에서 '확인'을 눌렀을 때 실행됩니다.
//         qDebug() << "아메리카노 옵션 선택 완료!";

//         // 여기서 나중에 장바구니 리스트에 추가하는 코드를 넣을 겁니다.
//     } else {
//         // 사용자가 '취소'나 '창 닫기'를 눌렀을 때 실행됩니다.
//         qDebug() << "주문 취소";
//     }

//     // 3. 다이얼로그 사용이 끝났으므로 메모리에서 삭제합니다.
//     delete optionDialog;
// }

// void MainWindow::changeCartQuantity(const QString &menuName, int delta) {
//     // 현재 수량에 +1 또는 -1
//     cartData[menuName] += delta;

//     // 수량이 0 이하라면 장바구니에서 삭제
//     if (cartData[menuName] <= 0) {
//         cartData.remove(menuName);
//     }

//     // UI 갱신
//     updateCartTable();
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

