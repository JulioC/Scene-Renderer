QT += core gui opengl

SOURCES += main.cpp \
    mesh.cpp \
    meshloaderoff.cpp \
    glwidget.cpp \
    trackball.cpp \
    camera.cpp \
    mainwindow.cpp \
    meshloaderply.cpp \
    scene.cpp \
    light.cpp \
    inputstate.cpp \
    object.cpp \
    material.cpp \
    texture.cpp \
    keyvalues.cpp \
    sceneparser.cpp \
    timer.cpp \
    meshgenerator.cpp

HEADERS += \
    mesh.h \
    meshloaderoff.h \
    glwidget.h \
    trackball.h \
    camera.h \
    mainwindow.h \
    meshloaderply.h \
    scene.h \
    light.h \
    inputstate.h \
    object.h \
    material.h \
    texture.h \
    keyvalues.h \
    sceneparser.h \
    timer.h \
    meshgenerator.h

FORMS += \
    mainwindow.ui

RESOURCES += \
    resources.qrc
