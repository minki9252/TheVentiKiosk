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

    // 아이콘 크기
    ui->listSelectedMenu->setIconSize(QSize(200, 200));

    // 아이템이 창 크기에 맞춰 정렬되도록 설정
    ui->listSelectedMenu->setResizeMode(QListView::Adjust);

    // 사용자가 클릭해서 위치를 옮기지 못하도록 고정
    ui->listSelectedMenu->setMovement(QListView::Static);

    // 담기/취소 버튼 시그널 연결
    connect(ui->onConfirmClicked, &QPushButton::clicked, this, &MenuOptionDialog::accept);
    connect(ui->onCancelClicked, &QPushButton::clicked, this, &MenuOptionDialog::reject);
}

MenuOptionDialog::~MenuOptionDialog()
{
    delete ui;
}

// 담기 버튼 클릭 시 실행되는 슬롯 함수
void MenuOptionDialog::onConfirmClicked()
{
    qDebug() << "옵션 선택 완료";
    accept();
}

// 취소 버튼 클릭 시 실행되는 슬롯 함수
void MenuOptionDialog::onCancelClicked()
{
    reject();
}

void MenuOptionDialog::setMenuInfo(const QString &menuName)
{
    // 기존 리스트 항목 초기화
    ui->listSelectedMenu->clear();

    QString pureMenuName = menuName.split("\n").at(0);
    qDebug() << "DB 조회를 위한 순수 메뉴명:" << pureMenuName;

    QSqlQuery query;

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

    // 사이즈 선택
    if (ui->radioButton_5->isChecked()) selectedOpts << "Jumbo";
    else selectedOpts << "Large";

    // 원두 선택
    if (ui->radioButton_3->isChecked()) {
        selectedOpts << "디카페인";
        addedPrice += 800;}
    if (ui->radioButton_2->isChecked()) {
        selectedOpts << "다크원두";}
    if (ui->radioButton->isChecked()) {
        selectedOpts << "시그니처";}

    // 샷 선택
    if (ui->radioButton_8->isChecked()) {
        selectedOpts << "시그니처 샷추가";
        addedPrice += 500;
    }

    // 당도 선택
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

    // 토핑 선택
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
