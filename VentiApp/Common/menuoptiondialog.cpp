#include "menuoptiondialog.h"
#include "ui_menuoptiondialog.h"
#include <QMessageBox>
#include <QDebug>

// 생성자: UI를 초기화하고 부모 위젯을 설정합니다.
MenuOptionDialog::MenuOptionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MenuOptionDialog)
{
    ui->setupUi(this); // 디자인한 UI 요소들을 로드합니다.

    // 초기 설정이나 시그널/슬롯 연결을 여기서 수행합니다.
    connect(ui->pushButton, &QPushButton::clicked, this, &MenuOptionDialog::onConfirmClicked);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &MenuOptionDialog::onCancelClicked);
}

// 소멸자: 동적으로 할당된 ui 객체를 메모리에서 해제합니다.
MenuOptionDialog::~MenuOptionDialog()
{
    delete ui;
}

// '확인' 버튼 클릭 시 실행되는 슬롯 함수
void MenuOptionDialog::onConfirmClicked()
{
    // 선택된 옵션 데이터를 처리하는 로직
    qDebug() << "옵션 선택 완료";
    accept(); // 다이얼로그를 닫고 QDialog::Accepted를 반환합니다.
}

// '취소' 버튼 클릭 시 실행되는 슬롯 함수
void MenuOptionDialog::onCancelClicked()
{
    reject(); // 다이얼로그를 닫고 QDialog::Rejected를 반환합니다.
}
