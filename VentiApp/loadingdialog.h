#ifndef LOADINGDIALOG_H
#define LOADINGDIALOG_H

#include <QDialog>

class LoadingDialog : public QDialog {
    Q_OBJECT
public:
    explicit LoadingDialog(QWidget *parent = nullptr);
    // UI 파일이 없으므로 소멸자에서 delete ui;를 할 필요가 없습니다.
};

#endif
