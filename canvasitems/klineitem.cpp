#include "klineitem.h"
#include <QtWidgets>
#include <QPen>
#include <QDebug>
#include <QPolygon>
//#include "../util/util.h"

KLineItem::KLineItem(const QColor &color, double openPrice, double closePrice,double highestBid, double lowestBid,int index)
{
//    this->x = x;
//    this->y = y;

    this->openPrice = openPrice;
    this->closePrice = closePrice;
    this->highestBid = highestBid;
    this->lowestBid = lowestBid;

    this->color = color;
    setZValue(-1);

//    setFlags(ItemIsSelectable);
//    setAcceptHoverEvents(true);
    mIndex = index;

    this->setAcceptedMouseButtons(Qt::LeftButton);
    setFlag(QGraphicsItem::ItemIsSelectable);//必须加上这句，否则item无法获取到鼠标事件

    setFlag(QGraphicsItem::ItemIsMovable, true);
    setAcceptHoverEvents(true);
    setAcceptDrops(true);
}

KLineItem::~KLineItem()
{

}

QRectF KLineItem::boundingRect() const
{

    double lineHeight = fabs(highestBid - lowestBid)*KLineBaseHeight;

    return QRectF(-KLineBaseWidth/2, -lineHeight/2 , KLineBaseWidth,lineHeight);


//    painter->drawLine(0,-lineHeight/2.0,0,lineHeight/2.0);
//    painter->drawLine(-KLineBaseWidth/2,0,KLineBaseWidth/2,0);


}

QPainterPath KLineItem::shape() const
{
    QPainterPath path;
    path.addRect(14, 14, 82, 42);
    return path;
}

void KLineItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    painter->setRenderHint(QPainter::Antialiasing, true);

    QPen pen;
    double scaleFactor = painter->matrix().m11();
    pen.setWidth(pen.width()/scaleFactor); //线段保持原来的线宽

    QColor drawColor = color;

    pen.setColor(drawColor);
    painter->setBrush(drawColor);
    painter->setPen(pen);

//    double priceHeight = fabs(closePrice - openPrice)*KLineBaseHeight;
    double lineHeight = (highestBid - lowestBid)*KLineBaseHeight;

    double centerPrice = (lowestBid + (highestBid - lowestBid)/2.0);

    //   18 22  center 20    open 21- close 21.5 涨    center上移1
    //   15 25  center 20    open 17-close 19
    //   15 25  center 20    open 19 close 17
    double priceHeight = fabs(closePrice - openPrice)*KLineBaseHeight;
    double yPos = 0.0;
    if(openPrice >= closePrice){
        yPos = (centerPrice - openPrice)*KLineBaseHeight;
    }
    else{
        yPos = (centerPrice - closePrice)*KLineBaseHeight;
    }

    painter->drawRect(-KLineBaseWidth/2.0,yPos,KLineBaseWidth,priceHeight);

//    pen.setColor(Qt::black);
//    painter->setPen(pen);

    painter->drawLine(0,-lineHeight/2.0,0,lineHeight/2.0);


//    painter->drawLine(-KLineBaseWidth/2,0,KLineBaseWidth/2,0);

//    painter->drawText(0,0,QString::number(mIndex));

//    painter->drawText(0,-lineHeight/2.0,QString::number(testGetHeight()));
//    painter->drawText(0,lineHeight/2.0,QString::number(testGetLow()));
}

void KLineItem::drawAverageLine(QPainter* painter,std::vector<KLine2> kline,double centerPrice,int day)
{
    //画线要连接的点
    QVector<QPointF> point;
    QPointF temp;

    int count = kline.size();
    switch(day)
    {
    case 5:
        for( int i= 0;i<count;++i)
        {
            if( kline[i].averageLine5 == 0)
                continue;
            temp.setX(-3*KLineBaseWidth + i*KLineBaseWidth);
            temp.setY((centerPrice - kline[i].averageLine5)*KLineBaseHeight);
            point.push_back(temp);
        }
        break;
    case 10:
        for( int i= 0;i<count;++i)
        {
            if( kline[i].averageLine10 == 0)
                continue;
            temp.setX(-KLineBaseWidth + i*KLineBaseWidth);
            temp.setY((centerPrice - kline[i].averageLine10)*KLineBaseHeight);
            point.push_back(temp);
        }
        break;
    case 20:
        for( int i= 0;i<count;++i)
        {
            if( kline[i].averageLine20 == 0)
                continue;
            temp.setX(-KLineBaseWidth + i*KLineBaseWidth);
            temp.setY((centerPrice - kline[i].averageLine20)*KLineBaseHeight);
            point.push_back(temp);
        }
        break;
    case 30:
        for( int i= 0;i<count;++i)
        {
            if( kline[i].averageLine30 == 0)
                continue;
            temp.setX(-KLineBaseWidth + i*KLineBaseWidth);
            temp.setY((centerPrice - kline[i].averageLine30)*KLineBaseHeight);
            point.push_back(temp);
        }
        break;
    case 60:
        for( int i= 0;i<count;++i)
        {
            if( kline[i].averageLine60 == 0)
                continue;
            temp.setX(-KLineBaseWidth + i*KLineBaseWidth);
            temp.setY((centerPrice - kline[i].averageLine60)*KLineBaseHeight);
            point.push_back(temp);
        }
        break;
    default:
        break;
    }

    QPen pen = painter->pen();
//    pen.setWidthF(1.5/Util::getInstance()->scaleY());

//    qDebug() << "kline item pen widthF:" << pen.widthF() <<" scaleY:" << Util::getInstance()->scaleY();

    switch(day)
    {
    case 5:
        pen.setColor(Qt::white);
        break;
    case 10:
        pen.setColor(Qt::yellow);
        break;
    case 20:
        pen.setColor(Qt::magenta);
        break;
    case 30:
        pen.setColor(Qt::green);
        break;
    case 60:
        pen.setColor(Qt::cyan);
        break;
    default:
        pen.setColor(Qt::white);
        break;
    }

//    if(mIndex%10 == 0){

//        pen.setColor(kmaColor);
//        qDebug() << "index :" << mIndex << " drawAverageLine point:" << point;
//    }


    painter->setPen(pen);
    if(point.size() > 0)
        cubicToLine(painter,point);


//    QPolygonF polykline(point);
//    painter->drawPolyline(polykline);
}

void KLineItem::cubicToLine(QPainter* painter,QVector<QPointF> points)
{
    // 曲线上的点
//    static QList<QPointF> points = QList<QPointF>() << QPointF(0, 0) << QPointF(100, 100) << QPointF(200, -100)
//                                                    << QPointF(300, 100) << QPointF(330, -80) << QPointF(350, -70);
    QPainterPath path(points[0]);
    //计算
    for (int i = 0; i < points.size() - 1; ++i) {
        // 控制点的 x 坐标为 sp 与 ep 的 x 坐标和的一半
        // 第一个控制点 c1 的 y 坐标为起始点 sp 的 y 坐标
        // 第二个控制点 c2 的 y 坐标为结束点 ep 的 y 坐标
        QPointF sp = points[i];
        QPointF ep = points[i+1];
        QPointF c1 = QPointF((sp.x() + ep.x()) / 2, sp.y());
        QPointF c2 = QPointF((sp.x() + ep.x()) / 2, ep.y());
        path.cubicTo(c1, c2, ep);
    }
//    QPainter painter(this);
//    //设置渲染提示为消除锯齿
//    painter->setRenderHint(QPainter::Antialiasing, true);
//    //设置画笔颜色和宽度
//    painter->setPen(QPen(Qt::black, 2));
//    //将坐标系转换为矢量
//    painter->translate(40, 130);
//    //绘制path
    painter->drawPath(path);
//    // 绘制曲线上的点
//    painter->setBrush(Qt::gray);

    //绘制曲线上的点
//    for (int i = 0; i < points.size(); ++i) {
//        painter->drawEllipse(points[i], 4, 4);
//    }
}

void KLineItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
    update();
    qDebug() << "kline item mousePressEvent:";
}

void KLineItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "kline item pos:" << event->pos();
}

void KLineItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    mUseCusColor = !mUseCusColor;
    qDebug() << "KLineItem::mouseDoubleClickEvent mUseCusColor:" << mUseCusColor;

}

void KLineItem::setDataFile(DataFile *datafile)
{
    mDataFile = datafile;
}
