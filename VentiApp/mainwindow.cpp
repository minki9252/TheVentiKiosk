#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "databasemanager.h"
#include "menuoptiondialog.h"
#include "discountdialog.h"
#include "couponinputdialog.h"
#include "phonestampdialog.h"
#include "paymentdialog.h"
#include "receiptdialog.h"
#include "goodbyedialog.h"
#include <QMessageBox>
#include <QDebug>
#include <QTimer>
#include <QListWidgetItem>
#include <QIcon>
#include <QListWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>

// 1. 장바구니 각 행(Row)에 들어갈 커스텀 위젯 생성 함수
QWidget* MainWindow::createCartRowWidget(int index, const OrderInfo &order)
{
    QWidget *container = new QWidget();
    QHBoxLayout *layout = new QHBoxLayout(container);

    // [왼쪽] 메뉴명과 옵션 (수직 배치)
    QVBoxLayout *textLayout = new QVBoxLayout();
    QLabel *lblMenu = new QLabel(order.menuName);
    lblMenu->setStyleSheet("font-weight: bold; font-size: 13pt; color: #333;");

    QLabel *lblOptions = new QLabel(order.options);
    lblOptions->setStyleSheet("color: #666; font-size: 10pt;");
    lblOptions->setWordWrap(true); // 옵션이 길면 자동으로 다음 줄로 넘김

    textLayout->addWidget(lblMenu);
    textLayout->addWidget(lblOptions);
    layout->addLayout(textLayout, 4); // 텍스트 영역 비중 확대

    // [중간] 수량 조절 버튼 위젯
    QWidget *qtyWidget = createQuantityWidget(index, order.quantity);
    layout->addWidget(qtyWidget, 2);

    // [오른쪽] 금액
    QLabel *lblPrice = new QLabel(QString::number(order.totalPrice) + "원");
    lblPrice->setStyleSheet("font-weight: bold; font-size: 12pt; color: #5d2d91;");
    lblPrice->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    layout->addWidget(lblPrice, 2);

    layout->setContentsMargins(10, 5, 10, 5);
    container->setLayout(layout);

    return container;
}

// 2. 수량 조절 (+, -) 위젯 생성 및 이벤트 연결
QWidget* MainWindow::createQuantityWidget(int index, int initialQty)
{
    QWidget* pWidget = new QWidget();
    QHBoxLayout* pLayout = new QHBoxLayout(pWidget);

    QPushButton* btnMinus = new QPushButton("-");
    QPushButton* btnPlus = new QPushButton("+");
    QLabel* lblQty = new QLabel(QString::number(initialQty));

    // 디자인 설정
    btnMinus->setFixedSize(30, 30);
    btnPlus->setFixedSize(30, 30);
    lblQty->setAlignment(Qt::AlignCenter);
    lblQty->setStyleSheet("font-weight: bold; font-size: 11pt;");

    // 마이너스 버튼 클릭 시 로직
    connect(btnMinus, &QPushButton::clicked, this, [this, index]() {
        if (index < 0 || index >= cartList.size()) return;

        if (cartList[index].quantity > 1) {
            int unitPrice = cartList[index].totalPrice / cartList[index].quantity;
            cartList[index].quantity--;
            cartList[index].totalPrice = unitPrice * cartList[index].quantity;
        } else {
            cartList.removeAt(index); // 1개일 때 누르면 삭제
        }
        updateCartList(); // 리스트 새로고침
    });

    // 플러스 버튼 클릭 시 로직
    connect(btnPlus, &QPushButton::clicked, this, [this, index]() {
        if (index < 0 || index >= cartList.size()) return;

        int unitPrice = cartList[index].totalPrice / cartList[index].quantity;
        cartList[index].quantity++;
        cartList[index].totalPrice = unitPrice * cartList[index].quantity;
        updateCartList(); // 리스트 새로고침
    });

    pLayout->addWidget(btnMinus);
    pLayout->addWidget(lblQty);
    pLayout->addWidget(btnPlus);
    pLayout->setContentsMargins(0, 0, 0, 0);
    pLayout->setSpacing(5);
    pWidget->setLayout(pLayout);

    return pWidget;
}

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

    // 카테고리 버튼 연결
    connect(ui->pushButton,   &QPushButton::clicked, this, [this](){ updateMenuDisplay("신메뉴"); });
    connect(ui->pushButton_2, &QPushButton::clicked, this, [this](){ updateMenuDisplay("커피(핫)"); });
    connect(ui->pushButton_7, &QPushButton::clicked, this, [this](){ updateMenuDisplay("커피(아이스)"); });
    connect(ui->pushButton_3, &QPushButton::clicked, this, [this](){ updateMenuDisplay("버블티&티"); });
    connect(ui->pushButton_4, &QPushButton::clicked, this, [this](){ updateMenuDisplay("블렌디드"); });
    connect(ui->pushButton_5, &QPushButton::clicked, this, [this](){ updateMenuDisplay("베버리지"); });
    connect(ui->pushButton_6, &QPushButton::clicked, this, [this](){ updateMenuDisplay("에이드"); });

    // 메뉴 리스트 클릭 시 옵션창 호출
    connect(ui->listMenu, &QListWidget::itemClicked, this, &MainWindow::handleMenuItemClick);

    // 장바구니 전체 비우기
    connect(ui->btnClearCart, &QPushButton::clicked, this, [this](){
        cartList.clear();
        updateCartList();
        qDebug() << "장바구니가 비워졌습니다.";
    });

    // 결제하기 버튼
    connect(ui->btnCheckout, &QPushButton::clicked, this, [this](){
        if (cartList.isEmpty()) {
            QMessageBox::warning(this, "알림", "장바구니가 비어있습니다.");
            return;
        }
        processCheckout();
    });

    ui->stackedWidget->setCurrentIndex(0);
}

MainWindow::~MainWindow() { delete ui; }

// 메뉴판에 메뉴들을 로드하는 함수
void MainWindow::loadMenus(const QString &categoryName) {
    ui->listMenu->clear();
    QList<QVariantMap> menus = DatabaseManager::instance().getMenusByCategory(categoryName);

    for (const QVariantMap &menuData : menus) {
        QString itemText = QString("%1\n%2원")
                               .arg(menuData["name"].toString())
                               .arg(menuData["price"].toInt());

        QListWidgetItem *item = new QListWidgetItem();
        item->setText(itemText);
        item->setIcon(QIcon(menuData["image"].toString().trimmed()));
        item->setTextAlignment(Qt::AlignCenter);
        ui->listMenu->addItem(item);
    }
}

// 장바구니 UI 갱신 (리스트 위젯 방식)
void MainWindow::updateCartList()
{
    ui->listCart->clear();
    int totalAmount = 0;

    for (int i = 0; i < cartList.size(); ++i) {
        QListWidgetItem *item = new QListWidgetItem(ui->listCart);
        QWidget *rowWidget = createCartRowWidget(i, cartList[i]);

        item->setSizeHint(rowWidget->sizeHint());
        ui->listCart->addItem(item);
        ui->listCart->setItemWidget(item, rowWidget);

        totalAmount += cartList[i].totalPrice;
    }

    // [수정된 부분] 총 결제 금액 디자인 적용
    // 천 단위 콤마 추가 (예: 12,500원)
    QString formattedPrice = ui->listCart->locale().toString(totalAmount);

    // 금액 부분만 다른 색상이나 크기로 강조하고 싶을 때 HTML 사용 가능
    ui->lblTotal->setText(QString("<html><body>"
                                  "<span style='font-size:14pt; color:#333333; font-weight:normal;'>총 주문 금액: </span>"
                                  "<span style='font-size:20pt; color:#5d2d91; font-weight:bold;'>%1원</span>"
                                  "</body></html>").arg(formattedPrice));

    // 라벨 배경이나 테두리를 더 예쁘게 (스타일시트)
    ui->lblTotal->setText(QString("총 주문 금액: %1원").arg(totalAmount));
    ui->lblTotal->setAlignment(Qt::AlignCenter);
}

// 메뉴 클릭 시 옵션 다이얼로그 실행
void MainWindow::handleMenuItemClick(QListWidgetItem *item)
{
    if (!item) return;

    MenuOptionDialog optionDialog(this);
    optionDialog.setMenuInfo(item->text());

    if (optionDialog.exec() == QDialog::Accepted) {
        OrderInfo order = optionDialog.getSelectedOrderInfo();

        bool found = false;
        for(int i = 0; i < cartList.size(); ++i) {
            if(cartList[i].menuName == order.menuName && cartList[i].options == order.options) {
                cartList[i].quantity += order.quantity;
                cartList[i].totalPrice += order.totalPrice;
                found = true;
                break;
            }
        }

        if(!found) cartList.append(order);
        updateCartList();
    }
}

void MainWindow::processCheckout() {
    // 1. 초기 합계 금액 계산
    int totalAmount = 0;
    for(const auto& item : cartList) {
        totalAmount += item.totalPrice;
    }

    int discountAmount = 0; // 할인 초기값

    // 2. 할인 선택창 실행
    DiscountDialog discountDlg(this);
    if (discountDlg.exec() == QDialog::Accepted) {
        // 3. 쿠폰 번호 입력창 실행
        CouponInputDialog couponDlg(this);
        if(couponDlg.exec() == QDialog::Accepted) {
            discountAmount = 2000; // 쿠폰 적용 시 2,000원 할인
        }
    }

    // 4. 휴대폰 번호 적립창 실행
    PhoneStampDialog stampDlg(this);
    stampDlg.exec();

    // 5. 최종 결제 수단 선택 및 결제 진행 (로딩창 포함)
    PaymentDialog paymentDlg(totalAmount, discountAmount, this);
    if (paymentDlg.exec() == QDialog::Accepted) {

        // 6. 영수증에 표시할 메뉴 상세 내역 문자열 조립
        QString menuDetails = "  메뉴명            수량    금액\n";
        menuDetails += "----------------------------------\n";

        for(const auto& item : cartList) {
            QString name = item.menuName.leftJustified(12, ' ');

            // 수량 변수명이 quantity인지 꼭 확인하세요!
            QString countStr = QString::number(item.quantity).rightJustified(2, ' ');

            QString price = QString::number(item.totalPrice).rightJustified(6, ' ');

            menuDetails += QString("%1   %2   %3원\n").arg(name).arg(countStr).arg(price);
        }

        menuDetails += "----------------------------------\n";
        int finalPrice = totalAmount - discountAmount;
        if(finalPrice < 0) finalPrice = 0;
        menuDetails += QString("최종 합계:           %1원").arg(finalPrice);

        // 7. 영수증 확인창 호출 (주문번호와 메뉴 상세 내역을 함께 전달)
        ReceiptDialog receiptDlg(orderNumber, menuDetails, this);

        if (receiptDlg.exec() == QDialog::Accepted) {
            // '예'를 눌렀을 때 콘솔에 출력 시뮬레이션
            qDebug() << "******** 영수증 출력 중 ********";
            qDebug() << "주문번호: " << orderNumber;
            qDebug() << menuDetails;
            qDebug() << "******************************";
        }

        // 2. [추가] 인사 창 띄우기
        GoodbyeDialog goodbye(this);
        goodbye.exec(); // 2초 동안 노출된 후 자동으로 닫힙니다.

        // 8. 주문 완료 후 데이터 초기화 및 인트로 화면 이동
        orderNumber++;
        cartList.clear();
        updateCartList();
        ui->stackedWidget->setCurrentIndex(0); // 첫 화면으로 복귀
    }
}

// 기타 UI 보조 함수들
void MainWindow::toggleTouchText() {
    if (ui->lblTouchNotice) {
        ui->lblTouchNotice->setVisible(isVisible);
        isVisible = !isVisible;
    }
}

void MainWindow::on_introButton_clicked() { ui->stackedWidget->setCurrentIndex(1); }
void MainWindow::on_storeButton_clicked() { currentOrderType = 0; ui->stackedWidget->setCurrentIndex(2); loadMenus("신메뉴"); }
void MainWindow::on_takeoutButton_clicked() { currentOrderType = 1; ui->stackedWidget->setCurrentIndex(2); loadMenus("신메뉴"); }
void MainWindow::updateMenuDisplay(const QString &categoryName) { loadMenus(categoryName); }
