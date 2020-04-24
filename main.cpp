#include "widget.h"

#include <QApplication>
#include "interactiveview.h"
#include "customscene.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    Widget w;
//    w.show();

    // 创建 item
    CustomItem *pItem = new CustomItem();
    pItem->setRect(20, 20, 60, 60);

    // 将 item 添加至场景中
    CustomScene scene;
    scene.setSceneRect(0, 0, 400, 300);
//    scene.addItem(pItem);

    // 为视图设置场景
//    QGraphicsView view;
    InteractiveView view;
    view.setScene(&scene);
//    view.populateScene(&scene);
    view.show();





    return a.exec();
}
