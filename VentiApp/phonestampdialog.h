#ifndef PHONESTAMPDIALOG_H
#define PHONESTAMPDIALOG_H

#include <QDialog>

namespace Ui { class PhoneStampDialog; }

class PhoneStampDialog : public QDialog {
    Q_OBJECT
public:
    explicit PhoneStampDialog(QWidget *parent = nullptr);
    ~PhoneStampDialog();

private slots:
    void onKeyClicked();      // 숫자 키패드 클릭
    void on_btnSave_clicked(); // 적립 버튼
    void on_btnSkip_clicked(); // 건너뛰기 버튼

private:
    Ui::PhoneStampDialog *ui;
    QString phoneNumber;      // 입력된 휴대폰 번호 저장
};

#endif
