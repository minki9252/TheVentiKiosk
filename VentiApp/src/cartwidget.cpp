#include "cartwidget.h"
#include <QStandardItem>

CartWidget::CartWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::cartwidget),
    m_model(new QStandardItemModel(this))
{
    ui->setupUi(this);

    // 1. 리스트뷰 모델 설정
    ui->listView->setModel(m_model);

    // 2. 버튼 시그널 연결
    connect(ui->pushButton_61, &QPushButton::clicked, this, &CartWidget::onClearButtonClicked);
    connect(ui->pushButton_62, &QPushButton::clicked, this, &CartWidget::onCheckoutButtonClicked);
}

CartWidget::~CartWidget() {
    delete ui;
}

// 이벤트를 받아서 분기 처리하는 함수
void CartWidget::handleKioskEvent(const KioskEvent& event) {
    switch (event.action) {
        case CART_ADD:
            m_cartManager.addItem(event);
            refreshCartUI();
            break;

        case CART_INCREASE_QTY:
            // extraData에 index가 담겨있다고 가정
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
            onCheckoutButtonClicked();
            break;
            
        default:
            break;
    }
}

void CartWidget::refreshCartUI() {
    m_model->clear(); // 기존 리스트 초기화
    const auto& cartList = m_cartManager.getCartList();

    for (int i = 0; i < cartList.size(); ++i) {
        const auto& item = cartList.at(i);
        
        // 화면에 보여줄 텍스트 형식 구성
        QString displayText = QString("%1 x%2\n(%3, %4) - %5원")
                                .arg(item.menuName)
                                .arg(item.quantity)
                                .arg(item.size)
                                .arg(item.beanType)
                                .arg(item.totalPrice);
        
        QStandardItem* listItem = new QStandardItem(displayText);
        listItem->setData(i, Qt::UserRole); // 인덱스 저장 (나중에 삭제/수정 시 사용)
        m_model->appendRow(listItem);
    }

    // 장바구니가 비었을 때 버튼 비활성화 처리 (선택사항)
    ui->pushButton_61->setEnabled(!cartList.isEmpty());
    ui->pushButton_62->setEnabled(!cartList.isEmpty());
}

void CartWidget::onClearButtonClicked() {
    KioskEvent ev;
    ev.action = CART_CLEAR_ALL;
    handleKioskEvent(ev);
}

void CartWidget::onCheckoutButtonClicked() {
    int total = m_cartManager.getTotalPrice();
    // 결제 창 띄우기 로직 (생략)
    qDebug() << "Total Payment:" << total;
}