#include "option_modal.h"
#include "ui_option_modal.h"
#include "KioskData.h"
#include "databasemanager.h"
#include <QDebug>
#include <QLabel>
#include <QPushButton>
#include <QButtonGroup>
#include <QVBoxLayout>
#include <QHBoxLayout>

// 생성자에서 KioskData를 받아 멤버 변수에 보관
option_modal::option_modal(KioskData data, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::option_modal)
    , m_selectedMenu(data) // 넘어온 기본 정보(이름, 기본가 등)를 저장
{
    ui->setupUi(this);

    ui->lblMenuName->setText(m_selectedMenu.menuName);
    ui->lblPrice->setText(QString::number(m_selectedMenu.basePrice) + "원");


    // 이미지 출력 부분
    QPixmap pix(m_selectedMenu.imagePath);
    if (!pix.isNull()) {
        ui->lblMenuImg->setPixmap(pix.scaled(ui->lblMenuImg->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else {
        qDebug() << "이미지 로드 실패! 경로 확인 필요:" << m_selectedMenu.imagePath;
    }

    // 이 창에 포함된 모든 QPushButton을 리스트로 가져오기
    QList<QPushButton*> allButtons = this->findChildren<QPushButton*>();

    for(QPushButton* btn : allButtons) {
        // 취소나 담기 버튼은 제외하고, 옵션 선택용 버튼만 골라내기
        if(btn->isCheckable()) {
            // 버튼 상태가 바뀔 때마다 가격을 다시 계산하도록 연결
            connect(btn, &QPushButton::toggled, this, &option_modal::updatePrice);
        }
    }

    setupDynamicOptions();

    // 처음 창이 열릴 때 기본 가격을 한 번 출력해줍니다.
    updatePrice();
}

option_modal::~option_modal()
{
    delete ui;
}

void option_modal::setupDynamicOptions() {
    // 1. 기존 레이아웃 및 위젯 청소 (함수 호출 시 중복 생성 방지)
    if (ui->scrollAreaWidgetContents->layout()) {
        QLayoutItem* item;
        while ((item = ui->scrollAreaWidgetContents->layout()->takeAt(0)) != nullptr) {
            if (item->widget()) delete item->widget();
            delete item;
        }
        delete ui->scrollAreaWidgetContents->layout();
    }

    // 2. 격자 레이아웃(QGridLayout) 생성 및 설정
    QGridLayout* layout = new QGridLayout(ui->scrollAreaWidgetContents);
    ui->scrollAreaWidgetContents->setLayout(layout);

    // DB에서 모든 옵션 그룹 가져오기
    QList<OptionGroup> allGroups = DatabaseManager::instance().getAllOptions();

    if (allGroups.isEmpty()) {
        qDebug() << "⚠️ DB에 옵션 데이터가 없습니다!";
        return;
    }

    int row = 0;
    for (const auto& group : allGroups) {
        // 3. 그룹 제목 생성 (예: 사이즈, 원두...)
        QLabel* label = new QLabel(group.groupName);
        label->setStyleSheet("font-weight: bold; color: #f15a24; margin-top: 10px; font-size: 14px;");
        layout->addWidget(label, row++, 0, 1, 3); // 3열을 합쳐서 제목 표시

        QButtonGroup* btnGroup = new QButtonGroup(this);
        btnGroup->setExclusive(group.isExclusive);

        int col = 0;
        for (const auto& opt : group.options) {
            // 한 줄에 3개씩 배치
            if (col > 2) {
                col = 0;
                row++;
            }

            // 4. 가격 정보가 포함된 버튼 텍스트 생성
            QString buttonText = opt.name;
            if (opt.extraPrice > 0) {
                buttonText += QString(" (+%1원)").arg(opt.extraPrice);
            }

            QPushButton* btn = new QPushButton(buttonText);
            btn->setCheckable(true);
            btn->setMinimumHeight(45);

            // 디자인 입히기
            btn->setStyleSheet("QPushButton { background: white; border: 1px solid #ccc; font-size: 12px; border-radius: 5px; }"
                               "QPushButton:checked { border: 2px solid #f15a24; font-weight: bold; background-color: #fffaf9; }");

            // 나중에 결제 시 사용할 속성 저장
            btn->setProperty("extraPrice", opt.extraPrice);
            btn->setProperty("optionName", opt.name);
            btn->setProperty("groupName", group.groupName);

            connect(btn, &QPushButton::toggled, this, &option_modal::updatePrice);

            btnGroup->addButton(btn);
            layout->addWidget(btn, row, col++);
        }
        row++; // 다음 그룹과의 간격
    }

    // 마지막에 여백(Stretch)을 주어 위젯들이 위로 밀착되게 함
    layout->setRowStretch(row, 1);
}

// 확인 버튼 클릭 시 옵션을 확정하고 데이터를 카트로
void option_modal::on_btnConfirm_clicked() {
    QStringList options;
    int totalExtraPrice = 0;

    QList<QPushButton*> allButtons = this->findChildren<QPushButton*>();
    for (QPushButton* btn : allButtons) {
        if (btn->isCheckable() && btn->isChecked()) {
            QString gName = btn->property("groupName").toString();
            QString oName = btn->property("optionName").toString();
            int price = btn->property("extraPrice").toInt();

            options << oName;
            totalExtraPrice += price;

            if (gName == "사이즈") m_selectedMenu.sizeName = oName;
            else if (gName == "원두") m_selectedMenu.beanName = oName;
            else if (gName == "샷") m_selectedMenu.shotName = oName;
            else if (gName == "시럽") m_selectedMenu.syrupName = oName;
            else if (gName == "탄산수변경") m_selectedMenu.sparklingName = oName;
            else if (gName == "토핑") m_selectedMenu.toppingName = oName;
        }
    }

    m_selectedMenu.optionPrice = totalExtraPrice;
    m_selectedMenu.summaryText = options.join("/");
    m_selectedMenu.totalPrice = m_selectedMenu.basePrice + totalExtraPrice;
    m_selectedMenu.quantity = 1;

    QList<KioskData> list;
    list.append(m_selectedMenu);
    emit sendToCart(list);
    this->accept();
}

// 옵션 상세 가격
void option_modal::updatePrice()
{
    int extraPrice = 0;
    QStringList selectedOptions;

    // 1. 기본 메뉴 이름 추가 (예: 아메리카노)
    selectedOptions << m_selectedMenu.menuName; //

    // 2. 동적으로 생성된 버튼들 중 체크된 버튼들만 순회하며 정보 수집
    // option_modal 내의 모든 QPushButton을 찾아 체크 상태를 확인합니다.
    QList<QPushButton*> allButtons = this->findChildren<QPushButton*>();

    for (QPushButton* btn : allButtons) {
        // 옵션 선택용 버튼(Checkable)이면서 현재 체크된 상태인 경우
        if (btn->isCheckable() && btn->isChecked()) {
            // 버튼에 표시된 텍스트(예: Jumbo(960ml)(+1000원))를 리스트에 추가
            selectedOptions << btn->text();

            // 버튼 생성 시 저장해둔 'extraPrice' 속성값을 가져와 합산
            extraPrice += btn->property("extraPrice").toInt();
        }
    }

    // 3. UI 업데이트
    // 옵션 요약 라벨 업데이트 (슬래시로 구분하여 연결)
    // horizontalLayout_2 내부에 있는 요약용 TextLabel의 이름을 확인하여 적용하세요.
    // 예시에서는 질문하신 의도대로 lblOptionSummary(또는 해당 위치의 라벨)에 출력합니다.
    ui->lblOptionSummary->setText(selectedOptions.join(" / ")); //

    // 4. 추가 금액 표시 (+1500원 형태)
    ui->label->setText(QString(" + %1원").arg(extraPrice));

    // 5. 전체 합계 금액 업데이트 (기본가 + 추가가)
    int total = m_selectedMenu.basePrice + extraPrice;
    ui->lblPrice->setText(QString::number(total) + "원");
}

// 취소 버튼 로직
void option_modal::on_btnCancel_clicked()
{
    this->reject(); // 아무 데이터도 안 보내고 그냥 닫기
}
