#include "movingaverageitem.h"
#include <QPainter>
#include "datafile.h"
#include "klineitem.h"
#include <QDebug>

MovingAverageItem::MovingAverageItem(DataFile* dataFile)
{
    mDataFile = dataFile;
    setZValue(1);
//    setFlags(ItemIsSelectable);
}

MovingAverageItem::~MovingAverageItem()
{

}

QRectF MovingAverageItem::boundingRect() const
{
//    return QRectF(0, 0, 110, 70);
//    double priceHeight = fabs(closePrice - openPrice)*KLineBaseHeight;
//    double lineHeight = fabs(highestBid - lowestBid)*KLineBaseHeight;

//    return QRectF(-KLineBaseWidth/2, -lineHeight/2 , KLineBaseWidth,lineHeight);
    return QRectF(0,-25*KLineBaseHeight,mDataFile->kline.size()*KLineBaseWidth,25*200);
//    return QRectF(0,0,0,0);
}

QPainterPath MovingAverageItem::shape() const
{
    QPainterPath path;
//    path.addRect(14, 14, 82, 42);
//    path
    path.addPolygon(mLastPath);
    return path;
}

void MovingAverageItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    drawAverageLine(painter,5);
    drawAverageLine(painter,10);
    drawAverageLine(painter,20);
    drawAverageLine(painter,30);
    drawAverageLine(painter,60);


//    static bool firstDrawImage = true;

//    static QPixmap pixmap(mDataFile->kline.size()*KLineBaseWidth,25*KLineBaseHeight);

//    if(firstDrawImage){

//        pixmap.fill(Qt::transparent);
//        QPainter pixPainter(&pixmap);

//        pixPainter.setRenderHint(QPainter::Antialiasing, true);
//        drawAverageLine(&pixPainter,5);
//        drawAverageLine(&pixPainter,10);
//        drawAverageLine(&pixPainter,20);
//        drawAverageLine(&pixPainter,30);
//        drawAverageLine(&pixPainter,60);
//        firstDrawImage = false;
//        pixmap.save("111.png");
//    }

//    painter->drawPixmap(0,0,pixmap);

}

void MovingAverageItem::drawAverageLine(QPainter* painter,int day)
{
    //画线要连接的点
    QVector<QPoint> point;

    QPoint temp;

//    mDataFile.kline
    int count = mDataFile->kline.size();
    switch(day)
    {
    case 5:
        for( int i= 0;i<count;++i)
        {
            if( mDataFile->kline[i].averageLine5 == 0)
                continue;
            temp.setX(i*KLineBaseWidth + KLineBaseWidth/2);
            temp.setY(-KLineBaseHeight*mDataFile->kline[i].averageLine5);
//            temp.setX(getMarginLeft() + xstep *(i - beginDay) + 0.5*lineWidth);
//            temp.setY(getWidgetHeight() - (mDataFile.kline[i].averageLine5 - lowestBid) *yscale - getMarginBottom());
            point.push_back(temp);
        }
        break;
    case 10:
        for( int i= 0;i<count;++i)
        {
            if( mDataFile->kline[i].averageLine10 == 0)
                continue;
            temp.setX(i*KLineBaseWidth + KLineBaseWidth/2);
            temp.setY(-KLineBaseHeight*mDataFile->kline[i].averageLine10);

//            temp.setX(getMarginLeft() + xstep *(i - beginDay) + 0.5*lineWidth);
//            temp.setY(getWidgetHeight() - (mDataFile.kline[i].averageLine10 - lowestBid) *yscale - getMarginBottom());
            point.push_back(temp);
        }
        break;
    case 20:
        for( int i= 0;i<count;++i)
        {
            if( mDataFile->kline[i].averageLine20 == 0)
                continue;
            temp.setX(i*KLineBaseWidth + KLineBaseWidth/2);
            temp.setY(-KLineBaseHeight*mDataFile->kline[i].averageLine20);
//            temp.setX(getMarginLeft() + xstep *(i - beginDay) + 0.5*lineWidth);
//            temp.setY(getWidgetHeight() - (mDataFile.kline[i].averageLine20 - lowestBid) *yscale - getMarginBottom());
            point.push_back(temp);
        }
        break;
    case 30:
        for( int i= 0;i<count;++i)
        {
            if( mDataFile->kline[i].averageLine30 == 0)
                continue;
            temp.setX(i*KLineBaseWidth + KLineBaseWidth/2);
            temp.setY(-KLineBaseHeight*mDataFile->kline[i].averageLine30);
//            temp.setX(getMarginLeft() + xstep *(i - beginDay) + 0.5*lineWidth);
//            temp.setY(getWidgetHeight() - (mDataFile.kline[i].averageLine30 - lowestBid) *yscale - getMarginBottom());
            point.push_back(temp);
        }
        break;
    case 60:
        for( int i= 0;i<count;++i)
        {
            if( mDataFile->kline[i].averageLine60 == 0)
                continue;
            temp.setX(i*KLineBaseWidth + KLineBaseWidth/2);
            temp.setY(-KLineBaseHeight*mDataFile->kline[i].averageLine60);
//            temp.setX(getMarginLeft() + xstep *(i - beginDay) + 0.5*lineWidth);
//            temp.setY(getWidgetHeight() - (mDataFile.kline[i].averageLine60 - lowestBid) *yscale - getMarginBottom());
            point.push_back(temp);
        }
        break;
    default:
        break;
    }

    QPen     pen;
    pen.setWidth(2);
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

    painter->setPen(pen);
    QPolygon polykline(point);
    painter->drawPolyline(polykline);
    mLastPath = polykline;

//    painter->drawRect(0,0,100,100);
}

void MovingAverageItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
    update();
}
