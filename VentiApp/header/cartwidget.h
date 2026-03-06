#ifndef CARTWIDGET_H
#define CARTWIDGET_H

#include <QWidget>
#include <QStandardItemModel>
#include "ui_cartwidget.h"  // UI 파일에서 생성된 헤더
#include "kioskevent.h"
#include "cartmanager.h"

class CartWidget : public QWidget {
    Q_OBJECT

public:
    explicit CartWidget(QWidget *parent = nullptr);
    ~CartWidget();

    // 외부(메인 윈도우 등)에서 이벤트를 전달받는 함수
    void handleKioskEvent(const KioskEvent& event);

private slots:
    void onClearButtonClicked();    // 전체 삭제 버튼
    void onCheckoutButtonClicked(); // 결제하기 버튼
    void onCartItemClicked(const QModelIndex &index); // 아이템 클릭 시 삭제/수정 등

private:
    Ui::cartwidget *ui;
    CartManager m_cartManager;
    QStandardItemModel *m_model;

    void refreshCartUI(); // UI 갱신 핵심 로직
};

#endif // CARTWIDGET_H