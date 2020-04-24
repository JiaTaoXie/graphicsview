#ifndef CGRAPHICSVIEW_H
#define CGRAPHICSVIEW_H

#include <QGraphicsView>

class QWheelEvent;
class QKeyEvent;

class CGraphicsView : public QGraphicsView
{
public:
    explicit CGraphicsView(QWidget *parent = 0);

protected:
//    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void wheelEvent(QWheelEvent *event) override;

public Q_SLOTS:
    void zoomIn();
    void zoomOut();
    void zoom(float scaleFactor);
    void translate(QPointF delta);

private:
    Qt::MouseButton m_translateButton = Qt::LeftButton;
    bool m_bMouseTranslate = false;
    QPoint m_lastMousePos;

    qreal m_translateSpeed;  // 平移速度
    qreal m_zoomDelta;  // 缩放的增量
    qreal m_scale;  // 缩放值

private:

};

#endif // CGRAPHICSVIEW_H
