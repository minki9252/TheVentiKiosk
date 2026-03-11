#ifndef BEVERAGE_H
#define BEVERAGE_H

#include <QWidget>
#include <QList>
#include "KioskData.h" // [추가] 공통 구조체 정보 공유를 위해 포함

namespace Ui
{
    class beverage;
}

class beverage : public QWidget
{
    Q_OBJECT

public:
    // [변경] 생성자에서 선택된 메뉴의 기본 데이터(KioskEvent)를 받음
    explicit beverage(QWidget *parent = nullptr);
    ~beverage();

    // 🛠️ MainWindow에서 카테고리 이름을 넘겨받아 메뉴를 그릴 함수
    void loadMenus(const QString &categoryName);

signals:
    // [추가] 장바구니 데이터를 MainWindow로 전달하는 핵심 신호
    void sendToCart(QList<KioskData> list);

private slots:
    // [추가] 실시간 옵션 요약 및 가격 갱신 로직 (지훈님 기존 로직 복구용)
    void updateRealTimeInfo();
    // [추가] 확인 버튼 클릭 시 데이터를 리스트에 담아 발송하는 로직
    void on_btnConfirm_clicked();

private:
    Ui::beverage *ui;
    // ⬇️ [여기에 이 줄이 반드시 추가되어야 합니다!]
    KioskData m_currentOrder;
};

#endif // BEVERAGE_H
