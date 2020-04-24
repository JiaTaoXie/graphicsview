#ifndef MOVINGAVERAGEITEM_H
#define MOVINGAVERAGEITEM_H

#include <QColor>
#include <QGraphicsItem>

#define ITEMSPACE 4.0
#define ITEMLINEWIDTH 1.0

#define KLineBaseWidth 40.0 /*16*/
#define KLineBaseHeight 200.0/*40*/
#define REDCOLOR QColor(237,97,96)
#define GREENCOLOR QColor(0,247,172)

class DataFile;


class MovingAverageItem : public QGraphicsItem
{
public:
    MovingAverageItem(DataFile*);
    ~MovingAverageItem();

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;

    void resetColor(QColor color){
        this->color = color;
    }
    QColor getColor(){
        return this->color;
    }

    void setAverageLine5(double ave){
        averageLine5 = ave;
    }

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
//    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
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
    DataFile *mDataFile;
    QPolygon mLastPath;

private:
    void drawAverageLine(QPainter* painter,int day);
};


#endif // MOVINGAVERAGEITEM_H
