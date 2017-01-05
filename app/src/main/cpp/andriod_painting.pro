QT          += widgets
CONFIG      += c++11
HEADERS     = glwidget.h \
              window.h \
    model.h \
    ai.h \
    physics.h \
    data.h \
    debug.h
SOURCES     = glwidget.cpp \
              main.cpp \
              window.cpp \
    model.cpp \
    ai.cpp \
    physics.cpp \
    data.cpp

# install
INSTALLS += target

DEFINES += Q_OS_WIN
