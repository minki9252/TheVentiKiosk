#ifndef POINTMANAGERWIDGET_H
#define POINTMANAGERWIDGET_H

#include <QWidget>
#include <QList>
#include "KioskData.h"

namespace Ui
{
class PointManagerWidget;
}

class PointInputView;
class PointResultView;

class PointManagerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PointManagerWidget(const QList<KioskData> &cartList, QWidget *parent = nullptr);
    ~PointManagerWidget();

signals:
    void pointSaveCompleted();
    void cancelled();
    void stepCompleted();

private slots:
    void onDigitPressed(const QString &digit);
    void onDeletePressed();
    void onClearPressed();
    void onConfirmPressed();

private:
    Ui::PointManagerWidget *ui;
    PointInputView *m_inputView;
    PointResultView *m_resultView;
    QString m_phoneNum;
    QList<KioskData> m_cartList; // 장바구니 데이터 저장할 변수
    void connectViews();
    void updateInputDisplay();
};

#endif // POINTMANAGERWIDGET_H
