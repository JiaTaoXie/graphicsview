#include "interactiveview.h"

#include <QWheelEvent>
#include <QKeyEvent>
#include <QDebug>
#include <QMessageBox>
#include <math.h>
#include "./canvasitems/klineitem.h"
#include "viewport.h"

#define VIEW_CENTER viewport()->rect().center()
#define VIEW_WIDTH  viewport()->rect().width()
#define VIEW_HEIGHT viewport()->rect().height()

//const int m11Data[] = {0.2,0.4,0.6};

InteractiveView::InteractiveView(QWidget *parent)
    : QGraphicsView(parent),
      m_translateButton(Qt::LeftButton),
      m_scale(1.0),
      m_zoomDelta(0.5),
      m_translateSpeed(1.0),
      m_bMouseTranslate(false),
      mViewPort(new Viewport(this))
{
    // 去掉滚动条
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setCursor(Qt::PointingHandCursor);
    setRenderHint(QPainter::Antialiasing);

    setSceneRect(INT_MIN/2, INT_MIN/2, INT_MAX, INT_MAX);
    QPointF centerPos = QPointF(0,0);
    centerOn(centerPos);
    setCenterPos(centerPos);
    reCalSceneAviWidth();

    setupViewport(mViewPort);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    setStyleSheet("background: transparent;border:0px");
}

// 平移速度
void InteractiveView::setTranslateSpeed(qreal speed)
{
    // 建议速度范围
    Q_ASSERT_X(speed >= 0.0 && speed <= 2.0,
               "InteractiveView::setTranslateSpeed", "Speed should be in range [0.0, 2.0].");
    m_translateSpeed = speed;
}

qreal InteractiveView::translateSpeed() const
{
    return m_translateSpeed;
}

// 缩放的增量
void InteractiveView::setZoomDelta(qreal delta)
{
    // 建议增量范围
    Q_ASSERT_X(delta >= 0.0 && delta <= 1.0,
               "InteractiveView::setZoomDelta", "Delta should be in range [0.0, 1.0].");
    m_zoomDelta = delta;
}

qreal InteractiveView::zoomDelta() const
{
    return m_zoomDelta;
}

// 上/下/左/右键向各个方向移动、加/减键进行缩放、空格/回车键旋转
void InteractiveView::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Up:
        translate(QPointF(0, -2));  // 上移
        break;
    case Qt::Key_Down:
        translate(QPointF(0, 2));  // 下移
        break;
    case Qt::Key_Left:
        translate(QPointF(-2, 0));  // 左移
        break;
    case Qt::Key_Right:
        translate(QPointF(2, 0));  // 右移
        break;
    case Qt::Key_Plus:  // 放大
        zoomIn();
        break;
    case Qt::Key_Minus:  // 缩小
        zoomOut();
        break;
    case Qt::Key_Space:  // 逆时针旋转
        rotate(-5);
        break;
    case Qt::Key_Enter:  // 顺时针旋转
    case Qt::Key_Return:
        rotate(5);
        break;
    default:
        QGraphicsView::keyPressEvent(event);
    }
}

// 平移
void InteractiveView::mouseMoveEvent(QMouseEvent *event)
{
    if (m_bMouseTranslate){
        //        QPointF mouseDelta = mapToScene(event->pos()) - mapToScene(m_lastMousePos);
        //        translate(mouseDelta);

        QPointF delta;
        if(autoUpdateKLine){
            delta = mapToScene(event->pos().x(),0) - mapToScene(m_lastMousePos.x(),0);
            updateCenteOn2(delta,event->pos());
        }
        else{
            delta = mapToScene(event->pos()) - mapToScene(m_lastMousePos);
            updateCenteOn(delta,event->pos());
        }
    }

    m_lastMousePos = event->pos();
    mCrossPos = m_lastMousePos;
    viewport()->update();
//    mViewPort->update();
    QGraphicsView::mouseMoveEvent(event);
}

void InteractiveView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == m_translateButton) {
        // 当光标底下没有 item 时，才能移动
        QPointF point = mapToScene(event->pos());
        if (scene()->itemAt(point, transform()) == NULL)  {
            m_bMouseTranslate = true;
            m_lastMousePos = event->pos();
        }
    }
    QGraphicsView::mousePressEvent(event);
}

void InteractiveView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == m_translateButton)
        m_bMouseTranslate = false;

    QGraphicsView::mouseReleaseEvent(event);
}

void InteractiveView::mouseDoubleClickEvent(QMouseEvent *event)
{
    autoUpdateKLine = !autoUpdateKLine;
    QGraphicsView::mouseDoubleClickEvent(event);

    QPointF leftTopPos = mapToScene(QPoint(0,0));
    QPointF rightBtmPos = mapToScene(width(),height());

    QPointF rectPos = leftTopPos+QPointF(0,-3000);

    QRectF selectRect(rectPos,QSizeF(fabs(rightBtmPos.x()-leftTopPos.x()),
                                     fabs(rectPos.y())));

    QList<QGraphicsItem *> item_list =  scene()->items(selectRect);
    int count = item_list.count();

    qDebug() << "count:" << count;


}

// 放大/缩小
void InteractiveView::wheelEvent(QWheelEvent *event)
{
    // 滚轮的滚动量
    QPoint scrollAmount = event->angleDelta();
    // 正值表示滚轮远离使用者（放大），负值表示朝向使用者（缩小）
    scrollAmount.y() > 0 ? zoomIn() : zoomOut();

    reCalSceneAviWidth();
    updateKLine2();
}

void InteractiveView::paintEvent(QPaintEvent *event)
{
//    qDebug() << "InteractiveView::paintEvent";

    QGraphicsView::paintEvent(event);
    //画背景
    //drawBK();
    //画k线
    if(true){
        QPainter painter;
        painter.begin(this->viewport());        
        drawCross(painter);

        QPen pen = painter.pen();
        pen.setColor(Qt::red);
        painter.setPen(pen);

        QPointF scenePos = mapToScene(mCrossPos);
        QPointF centerPos = mapToScene(QPoint(0,0));
        painter.drawText(mCrossPos+QPointF(10,-10),QString("x=%1,y=%2,m11:%3,m12:%4")
                         .arg(scenePos.x()).arg(scenePos.y()).arg(transform().m11()).arg(transform().m21()));

        painter.drawText(mCrossPos+QPointF(10,-40),QString("centerPos:x=%1,y=%2,m_scale:%3")
                         .arg(centerPos.x()).arg(centerPos.y()).arg(QString::number(m_scale,'f',6)));

//        painter.drawRect(QRect(rect().center().x()-2, rect().center().y() - 2,4,4));
    }

}

void InteractiveView::leaveEvent(QEvent *event)
{
    m_bMouseTranslate = false;
    QGraphicsView::leaveEvent(event);
}

void InteractiveView::drawCross(QPainter& painter)
{
    drawCrossHorLine(painter);
    drawCrossVerLine(painter);
}

void InteractiveView::drawCrossVerLine(QPainter& painter)
{
    QPen pen;
    pen.setColor(mCrossColor);
    pen.setDashPattern(getDashPattern());
    pen.setWidth(1);
    painter.setPen(pen);

    double xPos = mCrossPos.x();

    QLine horline(xPos,0,xPos,VIEW_HEIGHT);
    painter.drawLine(horline);
}

void InteractiveView::drawCrossHorLine(QPainter& painter)
{
    QPen pen;
    pen.setDashPattern(getDashPattern());
    pen.setColor(mCrossColor);
    pen.setWidth(1);
    painter.setPen(pen);

    //横向不跟随
    double yPos = mCrossPos.y();

    QLine verline ( 0,yPos,VIEW_WIDTH,yPos );
    painter.drawLine(verline);

}

QVector<qreal> InteractiveView::getDashPattern()
{
    QVector<qreal> dashes;
    qreal space = 3;
    dashes << 5 << space << 5 <<space;
    return dashes;
}

// 放大
void InteractiveView::zoomIn()
{
    //    zoomIndex += 0.1;
    //    QTransform transform2;
    //    transform2.setMatrix(zoomIndex,1,1,1,1,1,1,1,1);
    //    setTransform(transform2);
    const qreal m11 = transform().m11();
    qreal newM11 = m11 + 0.5;
    qreal scaleFactor = newM11/m11;
    zoom(scaleFactor);

    //    zoom(1 + m_zoomDelta);
}

// 缩小
void InteractiveView::zoomOut()
{
    //    zoomIndex -= 0.1;
    //    QTransform transform2;
    //    transform2.setMatrix(zoomIndex,1,1,1,1,1,1,1,1);
    //    setTransform(transform2);
    //    qDebug() << "transform m11:" << transform().m11();

    const qreal m11 = transform().m11();
    qreal newM11 = m11 - 0.5;
    qreal scaleFactor = newM11/m11;
    zoom(scaleFactor);

    //    zoom(1 - m_zoomDelta);
}

// 缩放 - scaleFactor：缩放的比例因子
void InteractiveView::zoom(float scaleFactor)
{
    // 防止过小或过大
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 1 || factor > 100){
        return ;
    }

    qDebug() << "====>scaleFactor:" << scaleFactor;

    scale(scaleFactor, /*scaleFactor*/1);
    m_scale *= scaleFactor;
}

// 平移
void InteractiveView::translate(QPointF delta)
{
    //    // 根据当前 zoom 缩放平移数
    //   delta *= transform().m11();

    //    // view 根据鼠标下的点作为锚点来定位 scene
    //    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    ////    QPoint newCenter(VIEW_WIDTH / 2 - delta.x(),  VIEW_HEIGHT / 2 - delta.y());
    //    QPointF oldScenePos = mapToScene(rect().center());
    //    QPointF newCenter(oldScenePos - delta);
    //    centerOn(newCenter);

    //    // scene 在 view 的中心点作为锚点
    //    setTransformationAnchor(QGraphicsView::AnchorViewCenter);


    // 根据当前 zoom 缩放平移数
    qreal m11 = transform().m11();
    delta *= mTestScaleX;

//    delta *= transform().m11();


    qDebug() << "mouse move delta:" << delta;

    QPointF oldCenterPos = getCenterPos();

    // view 根据鼠标下的点作为锚点来定位 scene
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
//    QPoint newCenter(VIEW_WIDTH / 2 - delta.x(),  VIEW_HEIGHT / 2 - delta.y());
//    centerOn(mapToScene(newCenter));

    QPointF newCenter = getCenterPos() - delta;
    centerOn(newCenter);
    setCenterPos(newCenter);
    qDebug() << "scene center delta:" << oldCenterPos - newCenter;

    // scene 在 view 的中心点作为锚点
    setTransformationAnchor(QGraphicsView::AnchorViewCenter);
}

// 平移
void InteractiveView::translate_old(QPointF delta)
{
    // 根据当前 zoom 缩放平移数
    delta *= m_scale;
    delta *= m_translateSpeed;

    // view 根据鼠标下的点作为锚点来定位 scene
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    QPoint newCenter(VIEW_WIDTH / 2 - delta.x(),  VIEW_HEIGHT / 2 - delta.y());
    centerOn(mapToScene(newCenter));

    // scene 在 view 的中心点作为锚点
    setTransformationAnchor(QGraphicsView::AnchorViewCenter);
}


void InteractiveView::populateScene(QGraphicsScene* scene)
{
    QString file = "dataKLine.txt";
    if( !mDataFile.readData(file) )
    {
        QMessageBox::about(0,"数据文件读取失败","确定");
        return;
    }

    //    scene->setBackgroundBrush(QPixmap(":/image/background2.png"));

    int count = mDataFile.kline.size();
    for(int i=0;i<count;i++){

        double openPrice = mDataFile.kline[i].openingPrice;
        double closePrice = mDataFile.kline[i].closeingPrice;
        double highestBid = mDataFile.kline[i].highestBid;
        double lowestBid = mDataFile.kline[i].lowestBid;

        QColor color;
        double starY;
        if(openPrice < closePrice){
            color = GREENCOLOR;
            starY = closePrice;
        }
        else {
            color = REDCOLOR;
            starY = openPrice;
        }
        /*
         * 1.item的x=(KLineBaseWidth + ITEMLINEWIDTH) ITEMLINEWIDTH两条边,ITEMLINEWIDTH为item的边线
         * 2.item的y=最低价格+价差/2
        */
        KLineItem *item = new KLineItem(color, openPrice, closePrice,highestBid, lowestBid,i);

        item->setPos(i*(KLineBaseWidth + ITEMLINEWIDTH*2)+ITEMSPACE,
                     -(lowestBid + (highestBid - lowestBid)/2.0)*KLineBaseHeight);
        item->setDataFile(&mDataFile);
        scene->addItem(item);
    }

    //    MovingAverageItem *item = new MovingAverageItem(&mDataFile);
    //    item->setPos(0,0);
    //    scene->addItem(item);
}

void InteractiveView::scrollY()
{
    //    fitInView()
    updateKLine2();
}

void InteractiveView::changeXScale(double xScale)
{
//    const qreal m11 = transform().m11();
//    qreal newM11 = xScale;
//    qreal scaleFactor = newM11/m11;
//    zoom(scaleFactor);
//    updateKLine2();
    mTestScaleX = xScale;
}

void InteractiveView::testCenter()
{
    //    qDebug() << "center posF:" <<

    qDebug() << "get center then map to scene:" << mapToScene(rect().center());
    QPolygonF mapRect = mapToScene(rect());
    qDebug() << "mapRect :" << mapRect;
    qDebug() << "get rect to scene then get center:" << QPointF(mapRect.at(0).x() + fabs(QPointF(mapRect.at(1)-mapRect.at(0)).x())/2.0,
                                                                mapRect.at(0).y() + fabs(QPointF(mapRect.at(3) - mapRect.at(0)).y())/2.0);
}

void InteractiveView::updateCenteOn(const QPointF &delta,const QPoint& lastpos)
{
    //    translate(delta);
    translate_old(delta);
    //    updateKLine();
    m_lastMousePos = lastpos;
}

void InteractiveView::updateCenteOn2(const QPointF &delta,const QPoint& lastpos)
{
    //    translate(delta);
    translate(delta);
    updateKLine2();
    m_lastMousePos = lastpos;
}

void InteractiveView::updateKLine()
{
    if(!autoUpdateKLine)
        return;

    double centerYPos = selectItems();
    if(centerYPos == 0.0)
        return ;

    //移动中心点,只改变y轴，不改变x轴
    QPointF tmpPos = mapFromScene(QPointF(0,centerYPos));

    //    translate(QPointF(0, VIEW_HEIGHT/2 - tmpPos.y()));  // 下移
    translate(QPointF(0, VIEW_HEIGHT/2 - tmpPos.y()));

    //    makeHeightChanged();
    //    qDebug() << "StockCanvas::updateKLine mScaleY:" << mScaleY;
}

//// 根据当前 zoom 缩放平移数
//delta *= transform().m11();

//// view 根据鼠标下的点作为锚点来定位 scene
//setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
//QPoint newCenter(VIEW_WIDTH / 2 - delta.x(),  VIEW_HEIGHT / 2 - delta.y());
//centerOn(mapToScene(newCenter));

//// scene 在 view 的中心点作为锚点
//setTransformationAnchor(QGraphicsView::AnchorViewCenter);

void InteractiveView::updateKLine2(/*const QPointF &delta*/)
{
    //    if(!autoUpdateKLine)
    //        return;

    double centerYPos = selectItems();
    if(centerYPos == 0.0)
        return ;

    //    //移动中心点,只改变y轴，不改变x轴
    //    QPointF tmpPos = mapFromScene(QPointF(0,centerYPos));

    ////    translate(QPointF(0, VIEW_HEIGHT/2 - tmpPos.y()));  // 下移
    //    translate(QPointF(0, VIEW_HEIGHT/2 - tmpPos.y()));

    //    makeHeightChanged();
    //    qDebug() << "StockCanvas::updateKLine mScaleY:" << mScaleY;

    QPolygonF viewRectToScene = mapToScene(rect());
    QPointF ltPos = viewRectToScene.at(0); //QPointF(mapToScene(0,0));
    QPointF rbPos = viewRectToScene.at(2); //QPointF(mapToScene(width(),height()));
    //左上，右上，左下，右下
    //QPolygonF(QPointF(1251.76,-423.653)QPointF(1632.56,-423.653)QPointF(1632.56,-324.537)QPointF(1251.76,-324.537))

    QPointF newLTPos = QPointF(ltPos.x(),mHeightItem->testGetHeight());

    qreal newHeight = fabs(mLowItem->testGetLow()-mHeightItem->testGetHeight());
    //    qreal newWidth = width()*transform().m11();



    QRectF newRectView(newLTPos,QSizeF(mSceneAviWidth,newHeight));

    fitInView(newRectView,Qt::IgnoreAspectRatio);

//    qDebug() << "updateKLine2 fitInView new rectView:" << newRectView;
    //    QRectF fitRectF(QPointF(mapToScene(0,0).x(),mHeightItem->testGetHeight()),
    //                    QPointF)
    //    fitInView()
}

void InteractiveView::reCalSceneAviWidth()
{
    QPolygonF viewRectToScene = mapToScene(rect());
    QPointF ltPos = viewRectToScene.at(0); //QPointF(mapToScene(0,0));
    QPointF rbPos = viewRectToScene.at(2); //QPointF(mapToScene(width(),height()));
    mSceneAviWidth = fabs(rbPos.x() - ltPos.x());
}

double InteractiveView::selectItems()
{
    QPointF leftTopPos = mapToScene(QPoint(0,0));
    QPointF rightBtmPos = mapToScene(width(),height());

    QPointF rectPos = leftTopPos+QPointF(0,-3000);

    QRectF selectRect(rectPos,QSizeF(fabs(rightBtmPos.x()-leftTopPos.x()),
                                     fabs(rectPos.y())));

    QList<QGraphicsItem *> item_list =  scene()->items(selectRect);
    int count = item_list.count();

    if(count <= 10){
        mHeightItem = nullptr;
        mLowItem = nullptr;
        return 0.0;
    }

    mHeightItem = static_cast<KLineItem*>(item_list.at(0));
    mLowItem = mHeightItem;

//    qDebug() << "item count:" << count;
    for (int i=1;i<count;i++) {
        KLineItem* kitem = static_cast<KLineItem*>(item_list.at(i));
        if(fabs(kitem->testGetHeight())>= fabs(mHeightItem->testGetHeight())){
            mHeightItem = kitem;
        }
        if(fabs(kitem->testGetLow()) <= fabs(mLowItem->testGetLow())){
            mLowItem = kitem;
        }
    }


    return (mHeightItem->pos().y() - mLowItem->pos().y())/2 + mLowItem->pos().y();
}

void InteractiveView::makeHeightChanged()
{
    if(mHeightItem == nullptr || mLowItem == nullptr){
        return ;
    }

    //    qDebug() << "mHeightItem map from scene pos:" << mapFromScene(mHeightItem->pos());
    //    qDebug() << "mLowItem map from scene pos:" << mapFromScene(mLowItem->pos());

    QPointF hItemScenePos = mapFromScene(mHeightItem->pos());
    QPointF lItemScenePos = mapFromScene(mLowItem->pos());

    double sceneHLength = lItemScenePos.y() - hItemScenePos.y();
    sceneHLength = sceneHLength + mHeightItem->boundingRect().height()/2 + mLowItem->boundingRect().height()/2;

    if(sceneHLength == 0)
        return ;

    //    //检测抖动，缩放比例大于0.005时进行缩放
    //    qreal scaleY = VIEW_HEIGHT*0.7/sceneHLength;
    //    if(fabs(mScaleY - scaleY) > 0.02)
    //        mScaleY = scaleY;

    //    //拖拉的时候需要刷新，否则背景阑珊格刷新有问题
    //    scale(/*mScaleX*/1,mScaleY);
}

void InteractiveView::addCrossItem()
{

}

QGraphicsItem* InteractiveView::getCurItem(QPointF crossPos)
{
    QRectF selectRect = QRectF(crossPos.x(),-2000,1,2000);
    QList<QGraphicsItem(*)> itemList = scene()->items(selectRect);

//    qDebug() <<

}
