#ifndef CROSS_PLATFORM_DEBUG_H
#define CROSS_PLATFORM_DEBUG_H

#ifdef Q_OS_WIN
#include <QDebug>
#define debug() qDebug()
#else
#include <iostream>
#define debug() std::cerr
#endif


#endif // DEBUG_H
