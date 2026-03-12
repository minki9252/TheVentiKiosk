#ifndef POINTINPUTVIEW_H
#define POINTINPUTVIEW_H

#include <QWidget>

namespace Ui {
class PointInputView;
}

class PointInputView : public QWidget
{
    Q_OBJECT

public:
    // ✅ cartList, totalAmount 인자 추가
    // ✅ 교체
    explicit PointInputView(QWidget *parent = nullptr);
    ~PointInputView();
    void setDisplay(const QString &maskedText);  // Manager가 마스킹 후 호출

signals:
    void digitPressed(const QString &digit);  // 숫자/010 버튼
    void deletePressed();                     // 부분삭제 버튼
    void clearPressed();                      // 전체삭제 버튼
    void confirmPressed();                    // 입력(확인) 버튼
    void cancelled();                         // 취소 버튼

private:
    Ui::PointInputView *ui;
    void connectButtons();
};

#endif // POINTINPUTVIEW_H
