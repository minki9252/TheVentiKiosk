#include "beverage.h"
#include "ui_beverage.h"
#include "databasemanager.h"
#include "option_modal.h"
#include "KioskData.h" // 🛠️ [추가] 공통 구조체 사용을 위해 포함
#include <QListWidgetItem>
#include <QIcon>
#include <QVariantMap>
#include <QDebug>
#include <QStringList>

// 🛠️ [변경] 생성자에서 메뉴 데이터를 초기화할 수 있도록 수정 (헤더 변경에 맞춤)
beverage::beverage(QWidget *parent)
    : QWidget(parent), ui(new Ui::beverage)
// , m_currentOrder(data) // 🛠️ [추가] 초기 데이터 복사
{
    ui->setupUi(this);
    // UI 파일(Qt Designer)에서 설정한 값들이 여기서 적용됩니다.
    ui->listMenu->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->listMenu->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->listMenu->setGridSize(QSize(120, 190));

    // 리스트의 메뉴를 클릭했을 때 option_modal 띄우기
    connect(ui->listMenu, &QListWidget::itemClicked, this, [=](QListWidgetItem *item)
            {

        // 🛠️ [추가] 클릭된 아이템에서 메뉴 정보를 추출하여 구조체에 저장
        // (실제 프로젝트에서는 item->data(Qt::UserRole) 등을 써서 ID를 관리하는 게 좋지만,
        // 우선은 텍스트 분리 방식으로 기본 로직을 잡습니다.)
        QString fullText = item->text();
        QStringList parts = fullText.split("\n");  // ✅ 이 줄이 빠져있었어요!
        QString menuName = fullText.split("\n").at(0);

        KioskData selectedMenu = m_currentOrder; // 기본 카테고리 정보 복사
        selectedMenu.menuName = menuName;        // 클릭한 메뉴 이름 저장

        QString priceStr = parts.at(1);
        priceStr.remove("원");
        selectedMenu.basePrice = priceStr.toInt();  // ✅ 여기가 핵심!

        // ✅ [추가] UserRole에 저장해둔 이미지 경로를 꺼내서 구조체에 담기
        selectedMenu.imagePath = item->data(Qt::UserRole).toString();

        qDebug() << "선택된 메뉴:" << selectedMenu.menuName;
        qDebug() << "클릭된 아이템 텍스트:" << item->text();

        // option_modal 객체 생성
        option_modal *modal = new option_modal(selectedMenu,this);
        // (주의: 클래스 이름이 OptionModal로 자동 생성되었다면 OptionModal로 적어주세요!)

        // 🛠️ [추가] 모달창에서 '장바구니 담기'를 눌렀을 때의 신호 연결
        // 모달에서 데이터를 받아 다시 MainWindow(부모)로 던져줍니다.
        connect(modal, &option_modal::sendToCart, this, &beverage::sendToCart);

        // 모달창 띄우기 (뒤에 있는 메인 화면 클릭 못하게 막음)
        modal->exec();

        // 모달창을 닫으면 메모리에서 깔끔하게 지워주기
        modal->deleteLater(); });
}

beverage::~beverage()
{
    delete ui;
}

// 🧩 DB에서 데이터를 가져와 화면에 그리는 함수
void beverage::loadMenus(const QString &categoryName)
{
    ui->listMenu->clear(); // 기존 메뉴 화면에서 지우기

    QList<QVariantMap> menus = DatabaseManager::instance().getMenusByCategory(categoryName);

    // 디버깅용 출력
    qDebug() << "-----------------------------------";
    qDebug() << "요청한 카테고리 이름:" << categoryName;
    qDebug() << "DB에서 가져온 메뉴 개수:" << menus.size() << "개";
    qDebug() << "-----------------------------------";

    for (const QVariantMap &menuData : std::as_const(menus))
    {
        QString itemText = QString("%1\n%2원")
                               .arg(menuData["name"].toString())
                               .arg(menuData["price"].toInt());

        QListWidgetItem *item = new QListWidgetItem();
        item->setText(itemText);

        QString imgPath = menuData["image"].toString().trimmed();

        // ✅ [추가] 이미지 경로를 아이템의 UserRole에 함께 저장
        item->setData(Qt::UserRole, imgPath);

        QIcon icon(imgPath);

        if (icon.isNull())
        {
            qDebug() << "이미지를 찾을 수 없음 ->" << imgPath;
        }
        else
        {
            item->setIcon(icon);
        }

        item->setTextAlignment(Qt::AlignCenter);
        ui->listMenu->addItem(item);
    }
}
void beverage::updateRealTimeInfo()
{
    // 필요한 로직 나중에 채우기
}

void beverage::on_btnConfirm_clicked()
{
    // 필요한 로직 나중에 채우기
}
