# Progetto di programmazione a oggetti (2019)
# Developed by Mariano Sciacco
# qCloud Manager

QT += widgets

TEMPLATE = app
CONFIG += c++11

SOURCES += main.cpp \
    model/model.cpp \
    model/core/server.cpp \
    model/core/vps.cpp \
    model/core/hosting.cpp \
    model/core/dedicated.cpp \
    model/core/node.cpp \
    model/xmlio.cpp \
    view/mainwindow.cpp \
    view/splashwindow.cpp \
    model/qtablenodesadapter.cpp \
    model/qtableserveradapter.cpp \
    view/serverview.cpp \
    view/extra/qlabeltime.cpp \
    model/qfiltersearchserver.cpp \
    view/extra/qchungusbar.cpp

HEADERS += \
    model/template/plist.hpp \
    model/template/deepptr.hpp \
    model/model.h \
    model/core/server.h \
    model/core/vps.h \
    model/core/hosting.h \
    model/core/dedicated.h \
    model/core/node.h \
    model/xmlio.h \
    view/mainwindow.h \
    view/splashwindow.h \
    model/qtablenodesadapter.h \
    model/qtableserveradapter.h \
    view/serverview.h \
    view/extra/qlabeltime.h \
    model/qfiltersearchserver.h \
    model/template/statictypebuilder.hpp \
    view/extra/qchungusbar.h

RESOURCES = \
    resources.qrc
