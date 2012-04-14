QT += core gui opengl

SOURCES += main.cpp \
    mesh.cpp \
    meshloaderoff.cpp \
    glwidget.cpp \
    trackball.cpp \
    camera.cpp \
    mainwindow.cpp \
    meshloaderply.cpp

HEADERS += \
    mesh.h \
    meshloaderoff.h \
    glwidget.h \
    trackball.h \
    camera.h \
    mainwindow.h \
    meshloaderply.h

FORMS += \
    mainwindow.ui

RESOURCES += \
    resources.qrc

OTHER_FILES += \
    vshader.glsl \
    shaders/blinn/vshader.glsl \
    shaders/blinn/fshader.glsl \
    shaders/gouraud/vshader.glsl \
    shaders/gouraud/fshader.glsl \
    shaders/normals/vshader.glsl \
    shaders/normals/fshader.glsl \
    shaders/phong/vshader.glsl \
    shaders/phong/fshader.glsl \
    shaders/toon/vshader.glsl \
    shaders/toon/fshader.glsl \
    shaders/fshader.glsl \
    shaders/vshader.glsl

