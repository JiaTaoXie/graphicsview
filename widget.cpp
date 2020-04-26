#include "widget.h"
#include "ui_widget.h"
#include <QApplication>
#include "interactiveview.h"
#include "customscene.h"
#include <QDebug>
#include <QOpenGLWidget>
#include <QGLWidget>

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
    mView->populateScene(scene);
//    mView.show();

#ifndef QT_NO_OPENGL
//    mView->setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
//    mView->setViewport(new QOpenGLWidget());
#endif
//    mView->setViewport(new QWidget);

    ui->gridLayout->addWidget(mView);

    QPushButton* btn = new QPushButton("打开",this);
    btn->move(mView->pos());
}

void Widget::on_lineEdit_returnPressed()
{

}

void Widget::on_pushButton_2_clicked()
{
    mView->scrollY();
}

void Widget::on_pushButton_clicked()
{
    qDebug() << "sceneRect:" << mView->mapToScene(mView->rect());
}

void Widget::on_pushButton_3_clicked()
{
    mView->testCenter();
}

void Widget::on_pushButton_4_clicked()
{
    mView->selectItems();
}

void Widget::on_pushButton_5_clicked()
{

}

void Widget::on_lineEdit_2_returnPressed()
{
        mView->changeXScale(ui->lineEdit_2->text().toDouble());
}

void Widget::on_lineEdit_2_cursorPositionChanged(int arg1, int arg2)
{

}

void Widget::paintEvent(QPaintEvent *event)
{
    QPainter painter;
    painter.begin(this);
    painter.fillRect(rect(),QBrush(QColor("#2F3843")));
    painter.setPen(QPen(QColor(255,255,255,50)));

    qreal heightSpace = height()/6;
    qreal widthSpace = width()/6;
    for (int i=0;i<5;i++) {
        painter.drawLine(QPoint(0,(i+1)*heightSpace),QPoint(width(),(i+1)*heightSpace));
        painter.drawLine(QPoint((i+1)*widthSpace,0),QPoint((i+1)*widthSpace,height()));
    }

    painter.end();

    qDebug() << "widget painter event";
}
