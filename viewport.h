#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <QOpenGLWidget>

class Viewport : public QOpenGLWidget
{
public:
    explicit Viewport(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());

protected:
    virtual void paintEvent(QPaintEvent *e) override;
};

#endif // VIEWPORT_H
