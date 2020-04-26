#ifndef INTERACTIVEVIEW_H
#define INTERACTIVEVIEW_H

#include <QGraphicsView>
#include "datafile.h"

class QWheelEvent;
class QKeyEvent;
class KLineItem;
class Viewport;

class InteractiveView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit InteractiveView(QWidget *parent = 0);

    // 平移速度
    void setTranslateSpeed(qreal speed);
    qreal translateSpeed() const;
    void populateScene(QGraphicsScene* scene);

    void changeXScale(double xScale);

    void testCenter();

    void scrollY();

    // 缩放的增量
    void setZoomDelta(qreal delta);
    qreal zoomDelta() const;

    double selectItems();

    void addCrossItem();

protected:
    // 上/下/左/右键向各个方向移动、加/减键进行缩放、空格/回车键旋转
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
    // 平移
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    // 放大/缩小
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *event) override;
    void leaveEvent(QEvent *event) override;

public Q_SLOTS:
    void zoomIn();  // 放大
    void zoomOut();  // 缩小
    void zoom(float scaleFactor); // 缩放 - scaleFactor：缩放的比例因子
    void translate(QPointF delta);  // 平移
    void translate_old(QPointF delta);

private:
    Qt::MouseButton m_translateButton;  // 平移按钮
    qreal m_translateSpeed;  // 平移速度
    qreal m_zoomDelta;  // 缩放的增量
    bool m_bMouseTranslate;  // 平移标识
    QPoint m_lastMousePos;  // 鼠标最后按下的位置
    qreal m_scale;  // 缩放值
    QPoint mCrossPos;
    QColor mCrossColor = Qt::white;

    DataFile mDataFile;
private:
    QVector<qreal> getDashPattern();
    void drawCrossHorLine(QPainter& painter);
    void drawCrossVerLine(QPainter& painter);
    void drawCross(QPainter& painter);
    void updateCenteOn(const QPointF &delta,const QPoint& lastpos);
    void updateCenteOn2(const QPointF &delta,const QPoint& lastpos);
    void updateKLine();
    void updateKLine2();
    QPointF getCenterPos(){ return mCenterPos; }
    void setCenterPos(QPointF pos) { mCenterPos = pos;}

    void reCalSceneAviWidth();

    bool autoUpdateKLine = true;
    void makeHeightChanged();

    KLineItem *mHeightItem,*mLowItem;
    qreal mSceneAviWidth;

    Viewport* mViewPort;

    QPointF mCenterPos;


};


#endif // INTERACTIVEVIEW_H
