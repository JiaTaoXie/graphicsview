#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

class InteractiveView;

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    void initView();

private slots:
    void on_lineEdit_returnPressed();

private:
    Ui::Widget *ui;

    InteractiveView *mView;
};
#endif // WIDGET_H
