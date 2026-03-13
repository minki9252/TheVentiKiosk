#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "databasemanager.h"
#include "menuoptiondialog.h"
#include "discountdialog.h"
#include "couponinputdialog.h"
#include "phonestampdialog.h"
#include "paymentdialog.h"
#include "receiptdialog.h"
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
    // 1. 초기 금액 계산
    int totalAmount = 0;
    for(const auto& item : cartList) {
        totalAmount += item.totalPrice;
    }

    int discountAmount = 0; // 초기 할인 금액은 0원

    // 2. 할인 선택창 (App쿠폰/모바일쿠폰 등)
    DiscountDialog discountDlg(this);
    if (discountDlg.exec() == QDialog::Accepted) {

        // 3. 쿠폰 번호 입력창
        CouponInputDialog couponDlg(this);
        if(couponDlg.exec() == QDialog::Accepted) {
            // [수정 포인트] 쿠폰 인증 성공 시 할인 금액을 2000원으로 설정
            discountAmount = 2000;
        }
    }

    // 4. 휴대폰 번호 적립창 (적립 여부와 상관없이 진행)
    PhoneStampDialog stampDlg(this);
    stampDlg.exec();

    // 5. 최종 결제 상세 창 띄우기 (계산된 total과 discount를 넘김)
    PaymentDialog paymentDlg(totalAmount, discountAmount, this);

    if (paymentDlg.exec() == QDialog::Accepted) {
        ReceiptDialog receiptDlg(this);
        if (receiptDlg.exec() == QDialog::Accepted) {
            // '예'를 눌렀을 때의 로직 (예: 콘솔에 영수증 내용 출력)
            qDebug() << "------- 영수증 출력 -------";
            qDebug() << "주문번호:" << orderNumber;
            qDebug() << "결제금액:" << (totalAmount - discountAmount) << "원";
            qDebug() << "-------------------------";
        }

        orderNumber++;
        cartList.clear();
        updateCartList();
        ui->stackedWidget->setCurrentIndex(0);
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
