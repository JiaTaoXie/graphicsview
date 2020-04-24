#ifndef KLINEITEM_H
#define KLINEITEM_H


#include <QColor>
#include <QGraphicsItem>

#include "datafile.h"

#define ITEMSPACE 4.0
#define ITEMLINEWIDTH 1.0

#define KLineBaseWidth 2.0 /*16*/
#define KLineBaseHeight 50.0/*40*/
#define REDCOLOR QColor(237,97,96)
#define GREENCOLOR QColor(0,247,172)

class KLineItem : public QGraphicsItem
{
public:
    KLineItem(const QColor &color, double openPrice, double closePrice,double highestBid, double lowestBid,int index);
    ~KLineItem();

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;

    void drawAverageLine(QPainter* painter,std::vector<KLine2> kline,double centerPrice,int day);
    void cubicToLine(QPainter* painter,QVector<QPointF>);

    void resetDefaultColor(){
        if(openPrice < closePrice){
            color = GREENCOLOR;
        }
        else {
            color = REDCOLOR;
        }
        update();
    }
    void resetColor(QColor color){
        this->color = color;
    }
    QColor getColor(){
        return this->color;
    }

    void setAverageLine5(double ave){
        averageLine5 = ave;
    }

    int getIndex(){
        return mIndex;
    }

//    void setKLineVec(std::vector<KLine> kline);
    void setDataFile(DataFile *mDataFile);

    qreal testGetHeight(){
        double lineHeight = (highestBid - lowestBid)*KLineBaseHeight;

        QPointF pos(0,-lineHeight/2.0);

        return mapToScene(pos).y();
    }
    qreal testGetLow(){
        double lineHeight = (highestBid - lowestBid)*KLineBaseHeight;

        QPointF pos(0,lineHeight/2.0);

        return mapToScene(pos).y();
    }

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
//    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    int x;
    int y;
    int width;
    int height;
    double openPrice;
    double closePrice;
    double highestBid;
    double lowestBid;
    bool mBDrawHighBid;
    bool mBDrawLowestBid;
    double averageLine5;
    QColor color;
    QVector<QPointF> stuff;

    int mIndex = 0;

    DataFile *mDataFile;

    QColor kmaColor = Qt::blue;
    bool mUseCusColor = false;

    bool mIsDrawBlue = false;
    bool mIsDrawBlack = false;
};


#endif // KLINEITEM_H
