#ifndef PALETTE_H
#define PALETTE_H

#include <QWidget>
#include"qopengl.h"
#include"Game/Game.h"
#include"header.h"
#include"Dependency/include.h"
#include"Dependency/timer.h"
#include"QMouseEvent"
#include"windows.h"
#include"Game/controller.h"
#include"qmessagebox.h"
#include"Game/rolecontroller.h"
class Palette : public  QOpenGLWidget, public OPENGLAPI
{
    Q_OBJECT
public:
    explicit Palette(QWidget *parent = nullptr);
protected:
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void keyReleaseEvent(QKeyEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void wheelEvent(QWheelEvent* event);
signals:

public slots:
};
///////////////////////////////////
void UpdateAllTimers();
void ExChangeTheWorld();
void DrawBag(bool isdraw);
#endif // PALETTE_H
