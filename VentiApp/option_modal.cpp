#include "option_modal.h"
#include "ui_option_modal.h"
#include "KioskData.h" // 🛠️ [추가] 구조체 활용을 위해 포함
#include <QDebug>
#include <QPushButton> // 버튼 객체를 다루기 위해 추가



// 🛠️ [변경] 생성자에서 KioskData를 받아 멤버 변수에 보관
option_modal::option_modal(KioskData data, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::option_modal)
    , m_selectedMenu(data) // 넘어온 기본 정보(이름, 기본가 등)를 저장
{
    ui->setupUi(this);

    // [로직 추가] UI에 선택한 메뉴 이름과 기본 가격 표시
    ui->lblMenuName->setText(m_selectedMenu.menuName);
    ui->lblPrice->setText(QString::number(m_selectedMenu.basePrice) + "원");

    // ✅ 버튼 클릭할 때마다 가격 업데이트
    connect(ui->btnSizeJumbo, &QPushButton::clicked, this, &option_modal::updatePrice);
    connect(ui->btnSizeLarge, &QPushButton::clicked, this, &option_modal::updatePrice);
    // ✅ 변경
    connect(ui->cbShotAdd, &QPushButton::clicked, this, [this](){
        updatePrice();
    });
}

option_modal::~option_modal()
{
    delete ui;
}

// 🧩 [추가] 확인 버튼 클릭 시 옵션을 확정하고 데이터를 밖으로 던지는 함수
void option_modal::on_btnConfirm_clicked()
{
    QStringList options; // 옵션들을 모을 바구니
    int extraPrice = 0;  // 추가 금액 계산용

    // 1. 사이즈 체크 (예시: rbLarge가 체크되었다면)
    if (ui->btnSizeJumbo->isChecked()) {
        options << "jumbo(960ml)";
        extraPrice += 1000;

    }
    if(ui->btnSizeLarge->isChecked()){
        options << "Large(600ml)";
        extraPrice += 0;
    }

    // 2. 추가 옵션 체크 (예시: 샷 추가 체크박스)
    if (ui->cbShotAdd->isChecked()) {
        options << "샷추가";
        extraPrice += 500;
    }

    // 3. 🌟 데이터 최종 가공 (KioskData 완성하기)
    m_selectedMenu.summaryText = options.join("/"); // 예: "Regular/샷추가"
    m_selectedMenu.totalPrice = m_selectedMenu.basePrice + extraPrice; // 최종가
    m_selectedMenu.quantity = 1; // 기본 수량 설정

    // 4. 완성된 데이터를 리스트에 담아 발송 (MainWindow가 이 신호를 받음)
    QList<KioskData> list;
    list.append(m_selectedMenu);

    emit sendToCart(list); // 밖으로 데이터 발사!

    this->accept(); // 모달 닫기
}

void option_modal::updatePrice()
{
    int extraPrice = 0;

    if (ui->btnSizeJumbo->isChecked()) extraPrice += 1000;
    if (ui->cbShotAdd->isChecked())    extraPrice += 500;

    int total = m_selectedMenu.basePrice + extraPrice;
    ui->lblPrice->setText(QString::number(total) + "원");
}

// 🧩 [추가] 취소 버튼 로직
// void option_modal::on_btnCancel_clicked()
// {
//     this->reject(); // 아무 데이터도 안 보내고 그냥 닫기
// }
