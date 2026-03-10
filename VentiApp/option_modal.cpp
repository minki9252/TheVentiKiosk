#include "option_modal.h"
#include "ui_option_modal.h"
#include "KioskData.h" // 🛠️ [추가] 구조체 활용을 위해 포함
#include <QDebug>
#include <QPushButton> // 버튼 객체를 다루기 위해 추가
#include <QButtonGroup> // 상단에 추가 확인!

// 🛠️ [변경] 생성자에서 KioskData를 받아 멤버 변수에 보관
option_modal::option_modal(KioskData data, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::option_modal)
    , m_selectedMenu(data) // 넘어온 기본 정보(이름, 기본가 등)를 저장
{
    ui->setupUi(this);

    // 1. 사이즈 버튼들을 그룹으로 묶기 (배타적 선택 활성화)

    //사이즈 그룹
    QButtonGroup *sizeGroup = new QButtonGroup(this);
    sizeGroup->addButton(ui->btnSizeLarge);
    sizeGroup->addButton(ui->btnSizeJumbo);

    // 원두 그룹
    QButtonGroup *beanGroup = new QButtonGroup(this);
    beanGroup->addButton(ui->btnBeanSignature);
    beanGroup->addButton(ui->btnBeanDark);
    beanGroup->addButton(ui->btnBeanDecaf);

    // 샷 그룹
    QButtonGroup *shotGroup = new QButtonGroup(this);
    shotGroup->addButton(ui->btnShotBasic);
    shotGroup->addButton(ui->btnShotLight);
    shotGroup->addButton(ui->btnShotSignatureAdd);

    // 당도(시럽) 그룹
    QButtonGroup *syrupGroup = new QButtonGroup(this);
    syrupGroup->addButton(ui->btnSyrupSweet);
    syrupGroup->addButton(ui->btnSyrupVanilla);
    syrupGroup->addButton(ui->btnSyrupHazelnut);

    // 탄산수 변경 그룹 (이미지상 버튼이 하나뿐이라면 그룹화는 생략해도 되지만,
    // 나중에 '일반 물' 같은 옵션이 생길 걸 대비해 묶어두면 좋습니다)
    QButtonGroup *sparklingGroup = new QButtonGroup(this);
    sparklingGroup->addButton(ui->btnSparkling); // 탄산수(+500)
    sparklingGroup->setExclusive(true);

    // 토핑 선택 그룹 (하나만 선택 가능한 경우)
    QButtonGroup *toppingGroup = new QButtonGroup(this);
    toppingGroup->addButton(ui->btnToppingIcecream); // 아이스크림(+500)
    toppingGroup->addButton(ui->btnToppingBubble);   // 타피오카펄(+900)
    toppingGroup->addButton(ui->btnToppingWhite);    // 화이트펄(+900)
    toppingGroup->setExclusive(true);

    // // 오트팝핑펄 그룹 (단독 옵션이면 그룹화 생략 가능)
    // QButtonGroup *oatGroup = new QButtonGroup(this);
    // oatGroup->addButton(ui->btnOatPearl); // 오트팝핑펄(+900)
    // oatGroup->setExclusive(true);

    sizeGroup->setExclusive(true); // 하나를 누르면 다른 하나는 무조건 해제됨
    beanGroup->setExclusive(true);
    shotGroup->setExclusive(true);
    syrupGroup->setExclusive(true);

    // 🚩 [수정 필요] DB에서 넘어온 메뉴 이름과 기본 가격을 UI 라벨에 출력하는 코드가 빠져있습니다.
    ui->lblMenuName->setText(m_selectedMenu.menuName);
    ui->lblPrice->setText(QString::number(m_selectedMenu.basePrice) + "원");

    // 🖼️ 1. 이미지 출력 부분 (기존 코드 유지하되, 라벨 크기 관련 팁 추가)
    QPixmap pix(m_selectedMenu.imagePath);
    if (!pix.isNull()) {
        // 🚩 [팁] 창이 완전히 뜨기 전(생성자 시점)에는 라벨 사이즈가 부정확할 수 있습니다.
        // 만약 사진이 너무 작게 나오면 Qt Designer에서 lblMenuImg의 scaledContents 속성을 체크하는 게 가장 편해요.
        ui->lblMenuImg->setPixmap(pix.scaled(ui->lblMenuImg->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else {
        // 🚩 [추가 권장] 이미지가 안 나올 경우를 대비해 로그를 찍어두면 디버깅이 편합니다.
        qDebug() << "이미지 로드 실패! 경로 확인 필요:" << m_selectedMenu.imagePath;
    }

    // 1. 이 창(option_modal)에 포함된 모든 QPushButton을 리스트로 가져옵니다.
    QList<QPushButton*> allButtons = this->findChildren<QPushButton*>();

    for(QPushButton* btn : allButtons) {
        // 2. '취소'나 '주문 담기' 버튼은 제외하고, 옵션 선택용(Checkable) 버튼만 골라냅니다.
        if(btn->isCheckable()) {
            // 3. 버튼 상태가 바뀔 때마다(toggled) 가격을 다시 계산하도록 연결!
            connect(btn, &QPushButton::toggled, this, &option_modal::updatePrice);
        }
    }

    // 4. 처음 창이 열릴 때 기본 가격을 한 번 출력해줍니다.
    updatePrice();
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
    // 1. 사이즈 (Jumbo만 추가금)
    if (ui->btnSizeJumbo->isChecked()) {
        options << "jumbo(960ml)";
        extraPrice += 1000;
    }

    // 2. 샷 선택 (시그니처샷 추가 시)
    if(ui->btnSizeLarge->isChecked()){
        options << "Large(600ml)";
        extraPrice += 0;
    }

    // --- 2. 원두 선택 (0원이지만 텍스트로 남김) ---
    if (ui->btnBeanDecaf->isChecked()) options << "디카페인";
    else if (ui->btnBeanDark->isChecked()) options << "다크원두";
    else options << "시그니처";

    // --- 3. 샷 선택 ---
    // 2. 추가 옵션 체크 (예시: 샷 추가 체크박스)
    if (ui->btnShotBasic->isChecked()) {
        options << "샷추가";
        extraPrice += 500;
    }
    if (ui->btnShotSignatureAdd->isChecked()) {
        options << "시그니처샷추가";
        extraPrice += 500;
    } else if (ui->btnShotLight->isChecked()) {
        options << "연하게";
    }

    // --- 4. 시럽 선택 ---
    if (ui->btnSyrupVanilla->isChecked()) {
        options << "바닐라시럽";
        extraPrice += 500;
    }
    if (ui->btnSyrupHazelnut->isChecked()) {
        options << "헤이즐넛시럽";
        extraPrice += 500;
    }

    // --- 5. 기타 및 토핑 (중복 선택 가능하므로 if-if 구조) ---
    if (ui->btnSparkling->isChecked()) {
        options << "탄산수변경";
        extraPrice += 500;
    }
    if (ui->btnToppingIcecream->isChecked()) {
        options << "아이스크림";
        extraPrice += 500;
    }
    if (ui->btnToppingBubble->isChecked()) {
        options << "타피오카펄";
        extraPrice += 900;
    }
    if (ui->btnOatPearl->isChecked()) {
        options << "오트팝핑펄";
        extraPrice += 900;
    }

    // 3. 🌟 데이터 최종 가공 (KioskData 완성하기)

    // 여기에 추가!
    m_selectedMenu.sizeName = ui->btnSizeJumbo->isChecked() ? "jumbo(960ml)" : "Large(600ml)";

    if (ui->btnBeanDecaf->isChecked()) m_selectedMenu.beanName = "디카페인";
    else if (ui->btnBeanDark->isChecked()) m_selectedMenu.beanName = "다크원두";
    else m_selectedMenu.beanName = "시그니처";

    if (ui->btnShotSignatureAdd->isChecked()) m_selectedMenu.shotName = "시그니처샷추가";
    else if (ui->btnShotLight->isChecked()) m_selectedMenu.shotName = "연하게";
    else m_selectedMenu.shotName = "기본";

    if (ui->btnSyrupVanilla->isChecked()) m_selectedMenu.syrupName = "바닐라시럽";
    else if (ui->btnSyrupHazelnut->isChecked()) m_selectedMenu.syrupName = "헤이즐넛시럽";
    else if (ui->btnSyrupSweet->isChecked()) m_selectedMenu.syrupName = "달게";

    m_selectedMenu.sparklingName = ui->btnSparkling->isChecked() ? "탄산수변경" : "";

    if (ui->btnToppingIcecream->isChecked()) m_selectedMenu.toppingName = "아이스크림";
    else if (ui->btnToppingBubble->isChecked()) m_selectedMenu.toppingName = "타피오카펄";
    else if (ui->btnToppingWhite->isChecked()) m_selectedMenu.toppingName = "화이트펄";
    else if (ui->btnOatPearl->isChecked()) m_selectedMenu.toppingName = "오트팝핑펄";

    m_selectedMenu.optionPrice = extraPrice;

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
    QStringList selectedOptions; // 실시간 옵션 텍스트 표시용

    // 1. 사이즈 선택 (필수, 단일)
    if (ui->btnSizeJumbo->isChecked()) {
        extraPrice += 1000;
        selectedOptions << "점보(960ml)";
    } else {
        selectedOptions << "라지(600ml)";
    }

    // 2. 원두 선택 (필수, 단일)
    if (ui->btnBeanDecaf->isChecked()) {
        extraPrice += 800;
        selectedOptions << "디카페인원두";
    } else if (ui->btnBeanDark->isChecked()) {
        selectedOptions << "다크원두";
    } else {
        selectedOptions << "시그니처원두";
    }

    // 3. 샷 선택 (필수, 단일)
    if (ui->btnShotSignatureAdd->isChecked()) {
        extraPrice += 500;
        selectedOptions << "시그니처샷추가";
    } else if (ui->btnShotLight->isChecked()) {
        selectedOptions << "연하게(1샷)";
    } else {
        selectedOptions << "기본(2샷)";
    }

    // 4. 당도/시럽 선택 (선택, 단일)
    if (ui->btnSyrupVanilla->isChecked()) {
        extraPrice += 500;
        selectedOptions << "바닐라시럽";
    } else if (ui->btnSyrupHazelnut->isChecked()) {
        extraPrice += 500;
        selectedOptions << "헤이즐넛시럽";
    } /*else if (ui->btnSyrupHoney->isChecked()) { // 추가된 옵션
        extraPrice += 500;
        selectedOptions << "허니시럽";
    } else if (ui->btnSyrupBlackSugar->isChecked()) { // 추가된 옵션
        extraPrice += 500;
        selectedOptions << "흑설탕시럽";
    } */else if (ui->btnSyrupSweet->isChecked()) {
        selectedOptions << "달게";
    }

    // 5. 변경 선택 (선택, 단일)
    if (ui->btnSparkling->isChecked()) {
        extraPrice += 500;
        selectedOptions << "탄산수변경";
    }

    // 6. 토핑 선택 (선택, 단일)
    if (ui->btnToppingIcecream->isChecked()) {
        extraPrice += 500;
        selectedOptions << "아이스크림추가";
    } else if (ui->btnToppingBubble->isChecked()) {
        extraPrice += 900;
        selectedOptions << "버블추가";
    } else if (ui->btnToppingWhite->isChecked()) {
        extraPrice += 900;
        selectedOptions << "화이트펄추가";
    }

    // 7. 오트팝핑펄 추가 (선택, 단일)
    if (ui->btnOatPearl->isChecked()) {
        extraPrice += 900;
        selectedOptions << "오트팝핑펄추가";
    }
    else{
        // 🚩 만약 선택이 안 되어 있다면 아무것도 더하지 않음
    }


    // 🌟 상단 옵션 요약 텍스트 업데이트
    // "옵션 : 라지(600ml)/시그니처원두/기본(2샷) + 0원" 형태
    ui->lblOptionSummary->setText("" + selectedOptions.join("/"));

    int total = m_selectedMenu.basePrice + extraPrice;
    ui->lblPrice->setText(QString::number(total) + "원");

}

// 🧩 [추가] 취소 버튼 로직
void option_modal::on_btnCancel_clicked()
{
    this->reject(); // 아무 데이터도 안 보내고 그냥 닫기
}
