#include "beverage.h"
#include "ui_beverage.h"
#include "databasemanager.h"
#include "option_modal.h"
#include <QListWidgetItem>
#include <QIcon>
#include <QVariantMap>
#include <QDebug>

beverage::beverage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::beverage)
{
    ui->setupUi(this);
    // UI 파일(Qt Designer)에서 설정한 값들이 여기서 적용됩니다.
    ui->listMenu->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->listMenu->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->listMenu->setGridSize(QSize(125, 190));

    // 리스트의 메뉴를 클릭했을 때 option_modal 띄우기
    connect(ui->listMenu, &QListWidget::itemClicked, this, [=](QListWidgetItem *item){

        qDebug() << "클릭된 아이템 텍스트:" << item->text();

        // option_modal 객체 생성
        option_modal *modal = new option_modal(this);
        // (주의: 클래스 이름이 OptionModal로 자동 생성되었다면 OptionModal로 적어주세요!)

        // 모달창 띄우기 (뒤에 있는 메인 화면 클릭 못하게 막음)
        modal->exec();

        // 모달창을 닫으면 메모리에서 깔끔하게 지워주기
        modal->deleteLater();
    });

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

    //디버깅용 출력
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
