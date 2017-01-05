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


#include "window.h"

#include <QGridLayout>
#include <QLabel>
#include <QTimer>
#include <QWidget>
#include <QPushButton>
#include <QFileDialog>

//! [0]
Window::Window()
{
    setWindowTitle(tr("2D Painting on Native and OpenGL Widgets"));

    openGL = new GLWidget(this);

    QPushButton *beginbtn = new QPushButton("Новые");
    connect(beginbtn, SIGNAL(pressed()), openGL, SLOT(newLines()));
    QPushButton *endbtn = new QPushButton("Готово");
    connect(endbtn, SIGNAL(pressed()), openGL, SLOT(endLines()));
    QPushButton *linesbtn = new QPushButton("Линии");
    connect(linesbtn, SIGNAL(pressed()), openGL, SLOT(modeLines()));
    QPushButton *circlesbtn = new QPushButton("Круги");
    connect(circlesbtn, SIGNAL(pressed()), openGL, SLOT(modeCircles()));
    QPushButton *playsbtn = new QPushButton("Play");
    connect(playsbtn, SIGNAL(pressed()), openGL, SLOT(modePlay()));
    QPushButton *savebtn = new QPushButton("Сохранить");
    connect(savebtn, &QPushButton::pressed, this, &Window::saveToFilePressed);
    QPushButton *loadbtn = new QPushButton("Загрузить");
    connect(loadbtn, &QPushButton::pressed, this, &Window::loadFromFilePressed);

    QBoxLayout *thlayout = new QHBoxLayout;
    thlayout->addWidget(beginbtn);
    thlayout->addWidget(endbtn);
    thlayout->addWidget(linesbtn);
    thlayout->addWidget(circlesbtn);
    thlayout->addWidget(playsbtn);
    QWidget *twidget = new QWidget;
    twidget->setLayout(thlayout);
    QBoxLayout *bhlayout = new QHBoxLayout;
    bhlayout->addWidget(savebtn);
    bhlayout->addWidget(loadbtn);
    QWidget *bwidget = new QWidget;
    bwidget->setLayout(bhlayout);
    QBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(twidget);
    layout->addWidget(openGL);
    layout->addWidget(bwidget);
    setLayout(layout);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), openGL, SLOT(animate()));
    timer->start(1);
}

void Window::keyPressEvent(QKeyEvent *event)
{
    openGL->keyPressEvent(event);
}

void Window::saveToFilePressed()
{
    openGL->saveToFile("test.mp123");
}

void Window::loadFromFilePressed()
{
    openGL->loadFromFile("test.mp123");
}
//! [0]
