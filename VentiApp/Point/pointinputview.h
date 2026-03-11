#ifndef POINTINPUTVIEW_H
#define POINTINPUTVIEW_H

#include <QWidget>

namespace Ui {
class CouponInputView;
}

class PointInputView : public QWidget
{
    Q_OBJECT

public:
    explicit PointInputView(QWidget *parent = nullptr);
    ~PointInputView();

signals:
    void phoneConfirmed(const QString &phoneNum);  // 입력 버튼 → 번호 전달
    void cancelled();                               // 취소 버튼

private:
    Ui::CouponInputView *ui;
    QString m_phoneNum;                            // 입력 번호 임시 저장

    void updateDisplay();                          // lineEdit * 마스킹 업데이트
    void connectButtons();                         // 버튼 시그널 연결
};

#endif // POINTINPUTVIEW_H
