#include "menuoptiondialog.h"
#include "ui_menuoptiondialog.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QIcon>
#include <QFileInfo>

MenuOptionDialog::MenuOptionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MenuOptionDialog)
{
    ui->setupUi(this);

    // 1. 아이콘 크기를 크게 키웁니다 (예: 200x200)
    ui->listSelectedMenu->setIconSize(QSize(200, 200));

    // 3. 아이템이 창 크기에 맞춰 정렬되도록 설정
    ui->listSelectedMenu->setResizeMode(QListView::Adjust);

    // 4. 사용자가 클릭해서 위치를 옮기지 못하도록 고정
    ui->listSelectedMenu->setMovement(QListView::Static);

    // 확인/취소 버튼 시그널 연결
    connect(ui->onConfirmClicked, &QPushButton::clicked, this, &MenuOptionDialog::accept);
    connect(ui->onCancelClicked, &QPushButton::clicked, this, &MenuOptionDialog::reject);
}

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

void MenuOptionDialog::setMenuInfo(const QString &menuName)
{
    // 1. 기존 리스트 항목 초기화
    ui->listSelectedMenu->clear();

    QString pureMenuName = menuName.split("\n").at(0);
    qDebug() << "DB 조회를 위한 순수 메뉴명:" << pureMenuName;

    QSqlQuery query;
    // kr_name 컬럼과 일치시키기 위해 추출한 순수 이름을 바인딩합니다.
    query.prepare("SELECT kr_name, price, image_path FROM MENU_INFO WHERE kr_name = :name");
    query.bindValue(":name", pureMenuName);

    if (query.exec() && query.next()) {
        QString name = query.value("kr_name").toString();
        int price = query.value("price").toInt();
        QString imgPath = query.value("image_path").toString();

        qDebug() << "이미지 경로 확인:" << imgPath;

        // 리스트 아이템 생성
        QString itemText = QString("%1\n(%2원)").arg(name).arg(price);
        QListWidgetItem *item = new QListWidgetItem(itemText, ui->listSelectedMenu);

        item->setTextAlignment(Qt::AlignCenter);

        // 이미지 설정
        QIcon menuIcon(imgPath);
        if (menuIcon.isNull()) {
            qDebug() << "이미지 로드 실패. 경로를 다시 확인하세요:" << imgPath;
        } else {
            item->setIcon(menuIcon);
        }
    } else {
        qDebug() << "메뉴 데이터 로드 실패. 쿼리문 확인:" << query.lastQuery();
        qDebug() << "에러 메시지:" << query.lastError().text();
    }
}

OrderInfo MenuOptionDialog::getSelectedOrderInfo() const
{
    OrderInfo info;
    info.menuName = this->currentMenuName.split("\n").at(0);
    info.basePrice = this->currentBasePrice;
    info.quantity = 1; // 필요 시 SpinBox를 추가해 조절 가능

    int addedPrice = 0; // 추가 금액 합계
    QStringList selectedOpts;

    // 1. 사이즈 선택 (예: Jumbo 선택 시 금액 추가가 있다면 처리)
    if (ui->radioButton_5->isChecked()) selectedOpts << "Jumbo";
    else selectedOpts << "Large";

    // 2. 원두 선택
    if (ui->radioButton_3->isChecked()) {
        selectedOpts << "디카페인";
        addedPrice += 800;}
    if (ui->radioButton_2->isChecked()) {
        selectedOpts << "다크원두";}
    if (ui->radioButton->isChecked()) {
        selectedOpts << "시그니처";}

    // 3. 샷 선택
    if (ui->radioButton_8->isChecked()) {
        selectedOpts << "시그니처 샷추가";
        addedPrice += 500;
    }

    // 4. 당도 선택 (예: 바닐라시럽 추가 등)
    if (ui->checkBox->isChecked()) {
        selectedOpts << "달게";
    }
    if (ui->checkBox_2->isChecked()) {
        selectedOpts << "바닐라시럽";
        addedPrice += 500;
    }
    if (ui->checkBox_3->isChecked()) {
        selectedOpts << "헤이즐넛시럽";
        addedPrice += 500;
    }
    if (ui->checkBox_4->isChecked()) {
        selectedOpts << "흑설탕시럽";
        addedPrice += 500;
    }

    // 5. 토핑 선택
    if (ui->checkBox_5->isChecked()) {
        selectedOpts << "아이스크림 추가";
        addedPrice += 500;
    }
    if (ui->checkBox_6->isChecked()) {
        selectedOpts << "버블 추가";
        addedPrice += 900;
    }
    if (ui->checkBox_7->isChecked()) {
        selectedOpts << "화이트펄 추가";
        addedPrice += 900;
    }


    info.options = selectedOpts.join(", ");
    info.totalPrice = info.basePrice + addedPrice;

    return info;
}
