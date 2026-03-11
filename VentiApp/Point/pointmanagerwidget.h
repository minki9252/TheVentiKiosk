#ifndef POINTMANAGERWIDGET_H
#define POINTMANAGERWIDGET_H

#include <QWidget>

namespace Ui {
class PointManagerWidget;
}

class PointManagerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PointManagerWidget(QWidget *parent = nullptr);
    ~PointManagerWidget();

private:
    Ui::PointManagerWidget *ui;
};

#endif // POINTMANAGERWIDGET_H
