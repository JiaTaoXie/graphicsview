#include "widget.h"
#include "ui_widget.h"
#include <QApplication>
#include "interactiveview.h"
#include "customscene.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    initView();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::initView()
{
    // 创建 item
    CustomItem *pItem = new CustomItem();
    pItem->setRect(20, 20, 60, 60);

    // 将 item 添加至场景中
    CustomScene *scene = new CustomScene();
    scene->setSceneRect(0, 0, 400, 300);
//    scene.addItem(pItem);

    // 为视图设置场景
//    QGraphicsView view;
    mView = new InteractiveView();
    mView->setScene(scene);
//    view.populateScene(&scene);
//    mView.show();

    ui->gridLayout->addWidget(mView);
}

void Widget::on_lineEdit_returnPressed()
{

}
