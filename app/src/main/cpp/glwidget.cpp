/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "glwidget.h"
//#include "helper.h"

#include <QPainter>
#include <QTimer>
#include <QDebug>
#include <QPoint>
#include <QTime>

//! [0]
GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent), mode(Lines), scale(1)
{
    elapsed = 0;
    setFixedSize(1000, 800);
    setAutoFillBackground(false);
    setMouseTracking(true);
}
//! [0]

//! [1]
void GLWidget::animate()
{
    elapsed = (elapsed + qobject_cast<QTimer*>(sender())->interval()) % 1000;
    update();
}

void GLWidget::newLines()
{
    model.lines.clear();
    beginpoint = true;
    model.otherCircles.clear();
    mode = Lines;
}

void GLWidget::endLines()
{
    beginpoint = true;
}

void GLWidget::newPoint(const Point &p)
{
    if (mode == Lines){
        if (beginpoint){
            beginpoint = false;
        }
        else {
            model.lines.push_back(Line(beginPoint, p));
        }
        beginPoint = p;
    }
    else if (mode == Circles){
        model.otherCircles.push_back(Circle(p.x/scale, p.y/scale, double(rand()%5)+3.+10./double(rand())));
    }
    else if (mode == Play){
        mousePressed = true;
    }
}

void GLWidget::modeLines()
{
    mode = Lines;
}

void GLWidget::modeCircles()
{
    mode = Circles;
}

void GLWidget::modePlay()
{
    mode = Play;
}
//! [1]

//! [2]
void GLWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter;
    if (event->rect().height() && model.mainCircle.r*scale*7. > event->rect().height()) scale /= 2.;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(event->rect(), QBrush(QColor(255, 255, 255)));
//    painter.translate(event->rect().width()/2, event->rect().height()/2);
    painter.save();
    painter.setBrush(QBrush(QColor(32, 64, 32)));
    painter.setPen(QPen(Qt::black));
    foreach (Circle c, model.otherCircles) {
        double r = c.r * scale;
        double dx = c.pos.x*scale;
        double dy = c.pos.y*scale;
        painter.drawEllipse(dx-r, dy-r, r * 2., r * 2.);
        painter.drawLine(dx,dy,dx+c.dir.x*5,dy+c.dir.y*5);
    }
    foreach (Line l, model.lines) {
        double x1 = l.begin.x*scale;
        double y1 = l.begin.y*scale;
        double x2 = l.end.x*scale;
        double y2 = l.end.y*scale;
        painter.drawLine(x1,y1,x2,y2);
    }
    int r = model.mainCircle.r * scale;
    painter.setBrush(QBrush(QColor(64, 32, 64)));
    painter.drawEllipse(QRectF(model.mainCircle.pos.x*scale-r, model.mainCircle.pos.y*scale-r, r * 2, r * 2));

    if (mode == Play){
        for(int i=0; i < 1; i++){
            QTime t;
            t.start();
            if (mousePressed) model.moveMainToPoint(mousePos.x()/scale, mousePos.y()/scale);
            model.nextStep();
        }
//        qDebug() << "t = " << t.elapsed();
    }
    else if (mode == Lines){
        if (!beginpoint) painter.drawLine(beginPoint.x, beginPoint.y, mousePos.x(), mousePos.y());
        const double r = 3.0;
        catchPoint = false;
        foreach (Line l, model.lines) {
            if (dist(Point(mousePos.x()/scale, mousePos.y()/scale), l.begin) < r){
                painter.drawEllipse(QRectF(l.begin.x*scale-r, l.begin.y*scale-r, r * 2, r * 2));
                catchPoint = true;
                catchedPoint = QPoint(l.begin.x*scale, l.begin.y*scale);
            }
            if (dist(Point(mousePos.x()/scale, mousePos.y()/scale), l.end) < r){
                painter.drawEllipse(QRectF(l.end.x*scale-r, l.end.y*scale-r, r * 2, r * 2));
                catchPoint = true;
                catchedPoint = QPoint(l.end.x*scale, l.end.y*scale);
            }
        }
    }
    painter.restore();
    painter.end();

}

void GLWidget::mousePressEvent(QMouseEvent *e)
{
    if (e->button()==Qt::LeftButton)
        newPoint(catchPoint ? Point(catchedPoint.x()/scale, catchedPoint.y()/scale) : Point(mousePos.x()/scale, mousePos.y()/scale));
    else if (e->button()==Qt::RightButton && mode == Lines)
        endLines();
}

void GLWidget::mouseReleaseEvent(QMouseEvent *e)
{
    Q_UNUSED(e);
    mousePressed = false;
}

void GLWidget::mouseMoveEvent(QMouseEvent *e)
{
    mousePos = e->pos();
    if ((mode == Lines) && (e->modifiers() == Qt::ShiftModifier) && !beginpoint)
    {
        int dx = abs(beginPoint.x - e->pos().x());
        int dy = abs(beginPoint.y - e->pos().y());
        if (dy > dx) mousePos.setX(beginPoint.x);
        else mousePos.setY(beginPoint.y);
    }
}

void GLWidget::keyPressEvent(QKeyEvent *event)
{
    double s = 2.5;
    double dx=0, dy=0;
    switch (event->key()) {
    case Qt::Key_Up:
        dy = -s;
        break;
    case Qt::Key_Down:
        dy = s;
        break;
    case Qt::Key_Left:
        dx = -s;
        break;
    case Qt::Key_Right:
        dx = s;
        break;
    default:
        break;
    }

    if (dx!=0 || dy!=0) model.moveMain(dx, dy);
    QOpenGLWidget::keyPressEvent(event);
}

void GLWidget::saveToFile(const QString &filename)
{
    model.save(filename.toStdString());
}

void GLWidget::loadFromFile(const QString &filename)
{
    model.load(filename.toStdString());
}
//! [2]
