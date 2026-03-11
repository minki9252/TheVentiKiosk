#include "cartwidget.h"
#include "ui_cartwidget.h"
#include "couponmanagerwidget.h"
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QDebug>
#include <QMessageBox>

cartwidget::cartwidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::cartwidget)
{
    ui->setupUi(this);

    // 만약 전체삭제 버튼 이름이 btnClearAll 이 아니라면 UI 파일에 맞춰 이름을 변경하세요.
    // connect(ui->전체삭제버튼이름, &QPushButton::clicked, this, &cartwidget::on_btnClearAll_clicked);
}

cartwidget::~cartwidget()
{
    delete ui;
}

void cartwidget::updateCart(QList<KioskData> list)
{
    m_cartList.append(list);
    refreshCartUI();
}

void cartwidget::calculateTotalAndEmit()
{
    int totalAmount = 0;
    for (const KioskData &item : m_cartList)
    {
        totalAmount += item.totalPrice * item.quantity;
    }

    // 🌟 추가된 부분: 자신의 UI에 있는 lblTotal 라벨을 직접 업데이트!
    ui->lblTotal->setText(QString("총 결제 금액: %1원").arg(totalAmount));

    // (나중을 위해 부모 창으로 금액 정보를 보내는 신호는 그대로 유지합니다)
    emit cartTotalChanged(totalAmount);
}

void cartwidget::refreshCartUI()
{
    ui->ListCart->clear(); // 기존 리스트 UI 비우기

    for (int i = 0; i < m_cartList.size(); ++i)
    {
        // 1. 리스트에 들어갈 빈 아이템 생성
        QListWidgetItem *item = new QListWidgetItem(ui->ListCart);

        // 2. 아이템 안에 들어갈 커스텀 위젯과 레이아웃 생성
        QWidget *rowWidget = new QWidget();
        QHBoxLayout *layout = new QHBoxLayout(rowWidget);
        layout->setContentsMargins(5, 5, 5, 5); // 여백 설정

        // 3. UI 요소들 생성 (X버튼, 메뉴정보, -, 수량, +, 가격)
        QPushButton *btnDel = new QPushButton("X");
        btnDel->setFixedSize(30, 30);

        QLabel *lblInfo = new QLabel(QString("%1 | %2").arg(m_cartList[i].menuName).arg(m_cartList[i].summaryText));
        lblInfo->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred); // 텍스트 영역 확장

        QPushButton *btnMinus = new QPushButton("-");
        btnMinus->setFixedSize(30, 30);

        QLabel *lblQty = new QLabel(QString::number(m_cartList[i].quantity));
        lblQty->setAlignment(Qt::AlignCenter);
        lblQty->setFixedSize(30, 30);

        QPushButton *btnPlus = new QPushButton("+");
        btnPlus->setFixedSize(30, 30);

        int itemTotalPrice = m_cartList[i].totalPrice * m_cartList[i].quantity;
        QLabel *lblPrice = new QLabel(QString("%1원").arg(itemTotalPrice));
        lblPrice->setFixedWidth(70);
        lblPrice->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

        // 4. 레이아웃에 요소들 담기
        layout->addWidget(btnDel);
        layout->addWidget(lblInfo);
        layout->addWidget(btnMinus);
        layout->addWidget(lblQty);
        layout->addWidget(btnPlus);
        layout->addWidget(lblPrice);

        // 5. 생성한 위젯을 리스트 아이템에 세팅
        item->setSizeHint(rowWidget->sizeHint()); // 위젯 크기에 맞게 아이템 높이 조절
        ui->ListCart->setItemWidget(item, rowWidget);

        // 6. 버튼 클릭 이벤트 연결 (람다 함수 사용)
        connect(btnDel, &QPushButton::clicked, this, [this, i]()
                { removeItem(i); });
        connect(btnMinus, &QPushButton::clicked, this, [this, i]()
                { decreaseQty(i); });
        connect(btnPlus, &QPushButton::clicked, this, [this, i]()
                { increaseQty(i); });
    }
    calculateTotalAndEmit();
}

// 개별 삭제 로직
void cartwidget::removeItem(int index)
{
    m_cartList.removeAt(index);
    refreshCartUI();
}

// 수량 증가 로직
void cartwidget::increaseQty(int index)
{
    //예외처리: 99개 제한으로 수정함
    if(m_cartList[index].quantity < 99){
        m_cartList[index].quantity++;
        refreshCartUI();
    }
}

// 수량 감소 로직 (1 이하로는 내려가지 않게)
void cartwidget::decreaseQty(int index)
{
    if (m_cartList[index].quantity > 1)
    {
        m_cartList[index].quantity--;
        refreshCartUI();
    }
}

void cartwidget::on_btnClearAll_clicked()
{
    m_cartList.clear();
    refreshCartUI(); // UI 비우고 총액 0원으로 업데이트됨
}

// 🌟 결제하기 버튼 동작
void cartwidget::on_btnCheckout_clicked()
{
    if (m_cartList.isEmpty())
    {
        QMessageBox::warning(this, "알림", "장바구니가 비어있습니다.");
        return;
    }
    emit checkoutRequested(); // MainWindow에 결제 시작해달라고 알림
}

// 결제 완료 후 외부에서 호출할 비우기 함수
void cartwidget::clearCart()
{
    m_cartList.clear();
    refreshCartUI();
}
