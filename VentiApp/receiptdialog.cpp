#include "receiptdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

ReceiptDialog::ReceiptDialog(int orderNum, const QString &menuDetails, QWidget *parent) : QDialog(parent) {
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    setFixedSize(450, 550); // 메뉴 내역이 들어가므로 높이를 키움
    setStyleSheet("background-color: white; border: 3px solid #5d2d91; border-radius: 20px;");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(30, 20, 30, 20);
    mainLayout->setSpacing(10);

    // 상단 아이콘 및 타이틀
    QLabel *lblTitle = new QLabel("Receipt", this);
    lblTitle->setAlignment(Qt::AlignCenter);
    lblTitle->setStyleSheet("font-size: 20pt; font-weight: bold; color: #5d2d91; border: none;");

    // 주문 번호
    QLabel *lblOrderNum = new QLabel(QString("No. %1").arg(orderNum), this);
    lblOrderNum->setAlignment(Qt::AlignCenter);
    lblOrderNum->setStyleSheet("font-size: 18pt; font-weight: bold; border: none;");

    // [추가] 상세 메뉴 내역 표시 (회색 박스 형태)
    QLabel *lblDetails = new QLabel(menuDetails, this);
    lblDetails->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    lblDetails->setStyleSheet(
        "background-color: #f9f9f9; "
        "border: 1px dashed #ccc; "
        "padding: 15px; "
        "font-family: 'Courier New'; " // 영수증 느낌을 주는 고정폭 글꼴
        "font-size: 11pt; "
        "color: #333;"
        );

    QLabel *lblMsg = new QLabel("영수증을 출력하시겠습니까?", this);
    lblMsg->setAlignment(Qt::AlignCenter);
    lblMsg->setStyleSheet("font-size: 13pt; color: #666; border: none; margin-top: 10px;");

    // 레이아웃 구성
    mainLayout->addWidget(lblTitle);
    mainLayout->addWidget(lblOrderNum);
    mainLayout->addWidget(new QLabel("------------------------------------------")); // 구분선
    mainLayout->addWidget(lblDetails); // 메뉴 내역 추가
    mainLayout->addWidget(lblMsg);

    // 버튼 레이아웃 (기존 동일)
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->setContentsMargins(10, 10, 10, 10);

    QPushButton *btnYes = new QPushButton("예 (출력)", this);
    QPushButton *btnNo = new QPushButton("아니오", this);

    // 버튼 스타일 (더벤티 테마)
    QString btnStyle = "QPushButton { font-size: 14pt; padding: 15px; border-radius: 10px; font-weight: bold; }";
    btnYes->setStyleSheet(btnStyle + "QPushButton { background-color: #5d2d91; color: white; }");
    btnNo->setStyleSheet(btnStyle + "QPushButton { background-color: #eee; color: #333; }");

    btnLayout->addWidget(btnNo); // '아니오'를 왼쪽에 배치
    btnLayout->addWidget(btnYes);

    // mainLayout->addWidget(lblIcon);
    mainLayout->addWidget(lblMsg);
    mainLayout->addLayout(btnLayout);

    // 4. 이벤트 연결
    connect(btnYes, &QPushButton::clicked, this, &QDialog::accept);
    connect(btnNo, &QPushButton::clicked, this, &QDialog::reject);
}
