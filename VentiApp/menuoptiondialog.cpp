#include "menuoptiondialog.h"
#include "ui_menuoptiondialog.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QIcon>
#include <QFileInfo>
#include <QButtonGroup>

MenuOptionDialog::MenuOptionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MenuOptionDialog)
{
    ui->setupUi(this);

    // --- 1. 사이즈 선택 그룹 (Large, Jumbo) ---
    // 이 그룹은 radioButton, radioButton_2를 관리합니다.
    QButtonGroup *sizeGroup = new QButtonGroup(this);
    sizeGroup->addButton(ui->radioButton_4);   // Large
    sizeGroup->addButton(ui->radioButton_5); // Jumbo
    sizeGroup->setExclusive(true);           // 그룹 내 딱 하나만 선택됨

    // --- 2. 원두 선택 그룹 (시그니처, 다크, 디카페인) ---
    // 이 그룹은 radioButton_3, 4, 5를 관리합니다.
    QButtonGroup *beanGroup = new QButtonGroup(this);
    beanGroup->addButton(ui->radioButton); // 시그니처
    beanGroup->addButton(ui->radioButton_2); // 다크
    beanGroup->addButton(ui->radioButton_3); // 디카페인(+800원)
    beanGroup->setExclusive(true);

    // --- 3. 샷 선택 그룹 (기본, 연하게, 시그니처샷추가) ---
    // 이 그룹은 radioButton_6, 7, 8을 관리합니다.
    QButtonGroup *shotGroup = new QButtonGroup(this);
    shotGroup->addButton(ui->radioButton_6); // 기본
    shotGroup->addButton(ui->radioButton_7); // 연하게
    shotGroup->addButton(ui->radioButton_8); // 시그니처샷추가(2샷 +500원)
    shotGroup->setExclusive(true);

    ui->listSelectedMenu->setIconSize(QSize(200, 200));
    ui->listSelectedMenu->setResizeMode(QListView::Adjust);
    ui->listSelectedMenu->setMovement(QListView::Static);

    connect(ui->btnConfirm, &QPushButton::clicked, this, &MenuOptionDialog::onConfirmClicked);
    connect(ui->btnCancel, &QPushButton::clicked, this, &MenuOptionDialog::onCancelClicked);
}

MenuOptionDialog::~MenuOptionDialog()
{
    delete ui;
}

void MenuOptionDialog::onConfirmClicked()
{
    qDebug() << "담기 버튼 클릭됨 (수동 호출)";
    accept();
}

void MenuOptionDialog::onCancelClicked()
{
    reject();
}

void MenuOptionDialog::setMenuInfo(const QString &menuName)
{
    // [위치: 옵션창이 뜰 때 초기 데이터를 로드하는 부분]

    // 1. 기존 리스트 항목 초기화 (이전 메뉴 잔상 제거)
    ui->listSelectedMenu->clear();

    // 2. 전달받은 문자열에서 순수 메뉴 이름만 추출 (개행 문자 기준)
    QString pureMenuName = menuName.split("\n").at(0);

    QSqlQuery query;
    // 3. 데이터베이스에서 메뉴 상세 정보(이름, 가격, 이미지) 조회
    query.prepare("SELECT kr_name, price, image_path FROM MENU_INFO WHERE kr_name = :name");
    query.bindValue(":name", pureMenuName);

    if (query.exec() && query.next()) {
        QString name = query.value("kr_name").toString();
        int price = query.value("price").toInt();
        QString imgPath = query.value("image_path").toString();

        // [중요] 멤버 변수에 현재 메뉴 정보를 저장하여 나중에 '담기' 버튼 클릭 시 사용함
        this->currentMenuName = name;
        this->currentBasePrice = price;

        // 4. 옵션창 상단 리스트 위젯에 선택한 메뉴 표시
        QString itemText = QString("%1\n(%2원)").arg(name).arg(price);
        QListWidgetItem *item = new QListWidgetItem(itemText, ui->listSelectedMenu);
        item->setTextAlignment(Qt::AlignCenter);

        // 5. 메뉴 이미지 로드 및 설정
        QIcon menuIcon(imgPath);
        if (!menuIcon.isNull()) {
            item->setIcon(menuIcon);
        } else {
            qDebug() << "이미지를 찾을 수 없음:" << imgPath;
        }
    } else {
        qDebug() << "DB 조회 실패:" << query.lastError().text();
    }
}

OrderInfo MenuOptionDialog::getSelectedOrderInfo() const
{
    OrderInfo info;
    // setMenuInfo에서 저장한 멤버 변수를 사용해야 함
    info.menuName = this->currentMenuName;
    info.basePrice = this->currentBasePrice;
    info.quantity = 1;

    int addedPrice = 0;

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
    info.totalPrice = (info.basePrice + addedPrice) * info.quantity;

    return info;
}
