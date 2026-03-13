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
    // 리스트 위젯의 가로 스크롤바를 항상 숨김 처리합니다.
    ui->ListCart->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

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
        QListWidgetItem *item = new QListWidgetItem(ui->ListCart);

        QWidget *rowWidget = new QWidget();
        QHBoxLayout *layout = new QHBoxLayout(rowWidget);
        layout->setContentsMargins(5, 5, 5, 5); // 여백 설정

        QPushButton *btnDel = new QPushButton("X");
        btnDel->setFixedSize(30, 30);

        // 🌟 수정된 부분: 텍스트 말줄임표(...) 처리 로직
        QString fullText = QString("%1 | %2").arg(m_cartList[i].menuName).arg(m_cartList[i].summaryText);
        
        QLabel *lblInfo = new QLabel();
        lblInfo->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        
        // 폰트 메트릭스 생성
        QFontMetrics metrics(lblInfo->font());
        
        // 텍스트가 표시될 최대 너비 계산
        // 전체 리스트 너비에서 X버튼(30), -버튼(30), 수량(30), +버튼(30), 가격(70) 및 여백을 고려해 대략 250px 정도를 뺍니다.
        // 키오스크 화면 레이아웃에 맞춰 이 수치(250)를 조절해 보세요.
        int maxWidth = ui->ListCart->width() - 250; 
        if(maxWidth <= 0) maxWidth = 300; // 초기화 전 너비가 0일 때를 대비한 기본값
        
        // 너비를 초과하면 '...'으로 자르기
        QString elidedText = metrics.elidedText(fullText, Qt::ElideRight, maxWidth);
        lblInfo->setText(elidedText);
        
        // 🌟 보너스: 글자가 잘렸을 경우 마우스를 올리면 전체 텍스트가 툴팁으로 보이게 합니다.
        lblInfo->setToolTip(fullText);

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
        item->setSizeHint(rowWidget->sizeHint()); 
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

QList<KioskData> cartwidget::getCartList() const
{
    return m_cartList;
}

// 🌟 추가된 부분: 현재 장바구니의 총 결제 금액 계산 및 반환
int cartwidget::getTotalAmount() const
{
    int totalAmount = 0;
    for (const KioskData &item : m_cartList)
    {
        totalAmount += item.totalPrice * item.quantity;
    }
    return totalAmount;
}
