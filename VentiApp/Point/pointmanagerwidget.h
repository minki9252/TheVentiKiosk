#ifndef POINTMANAGERWIDGET_H
#define POINTMANAGERWIDGET_H

#include <QWidget>

namespace Ui { class PointManagerWidget; }

class PointInputView;
class PointResultView;

class PointManagerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PointManagerWidget(QWidget *parent = nullptr);
    ~PointManagerWidget();

signals:
    void pointSaveCompleted();
    void cancelled();

private slots:
    void onDigitPressed(const QString &digit);
    void onDeletePressed();
    void onClearPressed();
    void onConfirmPressed();

private:
    Ui::PointManagerWidget *ui;
    PointInputView  *m_inputView;
    PointResultView *m_resultView;
    QString m_phoneNum;
    void connectViews();
    void updateInputDisplay();
};

#endif // POINTMANAGERWIDGET_H
