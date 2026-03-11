#ifndef OPTION_MODAL_H
#define OPTION_MODAL_H

#include <QDialog>
#include <QList>
#include "KioskData.h" // [추가] 공통 구조체 정보를 읽어오기 위해 포함

namespace Ui {
class option_modal;
}

class option_modal : public QDialog
{
    Q_OBJECT

public:
    // [변경] 생성자에서 KioskEvent 데이터를 받도록 수정 (beverage에서 던져준 데이터를 받음)
    explicit option_modal(KioskData data,QWidget *parent = nullptr);
    ~option_modal();

signals:
    // [추가] 옵션 선택 완료 후 데이터를 다시 밖(beverage -> mainwindow)으로 보낼 신호
    void sendToCart(QList<KioskData> list);

private slots:
    // [추가] 확인 버튼 클릭 시 동작할 로직 선언
    void on_btnConfirm_clicked(); //담기 버튼 눌렀을때
    void on_btnCancel_clicked();  // 🌟 이 줄이 없어서 에러가 났을 거예요! (이름 확인 필수)
    void updatePrice();  // ✅ 추가

private:
    Ui::option_modal *ui;
    // [추가] 넘어온 메뉴 정보를 이 창이 열려있는 동안 저장해둘 보관함
    KioskData m_selectedMenu;
    void setupDynamicOptions(); // [추가] DB에서 버튼을 만드는 함수 선언
};

#endif // OPTION_MODAL_H
