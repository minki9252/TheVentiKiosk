#include "pointresultview.h"
#include "ui_pointresultview.h"
#include <QPushButton>

PointResultView::PointResultView(QWidget *parent)
    : QWidget(parent), ui(new Ui::PointResultView)
{
    ui->setupUi(this);

    // 확인 버튼 → "눌렸다"는 사실만 emit, 로직은 없음
    connect(ui->pushButton_confirm, &QPushButton::clicked,
            this, &PointResultView::confirmed);
}

PointResultView::~PointResultView()
{
    delete ui;
}

// Manager가 데이터를 넘겨주면 표시만 함, 계산 없음
void PointResultView::setResult(const QString &phoneNum, int earnedStamps)
{
    ui->label_phone->setText(phoneNum);
    ui->label_points->setText(QString("%1 개").arg(earnedStamps));
}
