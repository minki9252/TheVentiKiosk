#ifndef POINTRESULTVIEW_H
#define POINTRESULTVIEW_H

#include <QWidget>

namespace Ui { class PointResultView; }

class PointResultView : public QWidget
{
    Q_OBJECT

public:
    explicit PointResultView(QWidget *parent = nullptr);
    ~PointResultView();
    void setResult(const QString &phoneNum, int earnedPoints); // ← 추가

signals:
    void confirmed(); // ← 추가

private:
    Ui::PointResultView *ui;
};

#endif // POINTRESULTVIEW_H
