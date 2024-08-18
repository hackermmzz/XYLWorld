#-------------------------------------------------
#
# Project created by QtCreator 2024-07-16T22:31:41
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = XYLWorld
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    palette.cpp \
    header.cpp \
    Game/Game.cpp \
    Game/globalsource.cpp \
    Dependency/include.cpp \
    Dependency/timer.cpp \
    Dependency/random.cpp \
    Dependency/vao.cpp \
    Dependency/vbo.cpp \
    Dependency/ebo.cpp \
    Dependency/program.cpp \
    Dependency/perlinnoise.cpp \
    Dependency/ubo.cpp \
    Dependency/ssbo.cpp \
    Dependency/threadpool.cpp \
    Dependency/texture2d.cpp \
    Dependency/camera.cpp \
    Game/controller.cpp \
    Game/role.cpp \
    Dependency/model.cpp \
    Game/chunk.cpp \
    Game/other.cpp \
    Game/section.cpp \
    Game/face.cpp \
    Game/block.cpp \
    Game/generateterrain.cpp \
    Game/generateblock.cpp \
    Game/generatebiomemap.cpp \
    Game/generateface.cpp \
    Game/generatedecoration.cpp \
    Dependency/aabb.cpp \
    Game/frustumcull.cpp \
    Dependency/texture2dgeneral.cpp \
    Game/biomebase.cpp \
    Game/desert.cpp \
    Game/forest.cpp \
    Game/grassland.cpp \
    Game/ocean.cpp \
    Game/plainland.cpp \
    Game/rainforest.cpp \
    Game/snowfield.cpp \
    Game/rolecontroller.cpp \
    Game/rolecontrollerglobalsource.cpp \
    Game/roledestroyblock.cpp \
    Game/roledrop.cpp \
    Game/rolefunctionblock.cpp \
    Game/rolejump.cpp \
    Game/rolemove.cpp \
    Game/rolestop.cpp \
    Dependency/textur1d.cpp \
    Dependency/particlesystem.cpp \
    Dependency/fbo.cpp \
    Dependency/rbo.cpp \
    Dependency/deferredrendering.cpp \
    itemboard.cpp \
    workspaceboard.cpp \
    bag.cpp \
    bagbutton.cpp

HEADERS += \
        mainwindow.h \
    palette.h \
    header.h \
    Game/Game.h \
    Game/globalsource.h \
    Dependency/include.h \
    Dependency/timer.h \
    Dependency/random.h \
    Dependency/bufferbase.hpp \
    Dependency/vao.h \
    Dependency/vbo.h \
    Dependency/ebo.h \
    Dependency/program.h \
    Dependency/perlinnoise.h \
    Dependency/ubo.h \
    Dependency/ssbo.h \
    Dependency/threadpool.h \
    Dependency/texture2d.h \
    Dependency/camera.h \
    Game/controller.h \
    Game/role.h \
    Dependency/model.h \
    Game/chunk.h \
    Game/other.h \
    Game/section.h \
    Game/face.h \
    Game/block.h \
    Game/generateterrain.h \
    Game/generateblock.h \
    Game/generatebiomemap.h \
    Game/generateface.h \
    Game/generatedecoration.h \
    Dependency/aabb.h \
    Game/frustumcull.h \
    Dependency/texture2dgeneral.h \
    Game/biomebase.h \
    Game/desert.h \
    Game/forest.h \
    Game/grassland.h \
    Game/ocean.h \
    Game/plainland.h \
    Game/rainforest.h \
    Game/snowfield.h \
    Dependency/FSM.hpp \
    Game/rolecontroller.h \
    Game/rolecontrollerglobalsource.h \
    Game/roledestroyblock.h \
    Game/roledrop.h \
    Game/rolefunctionblock.h \
    Game/rolejump.h \
    Game/rolemove.h \
    Game/rolestop.h \
    Game/bagglobalsource.h \
    Dependency/textur1d.h \
    Dependency/particlesystem.h \
    Dependency/fbo.h \
    Dependency/rbo.h \
    Dependency/deferredrendering.h \
    Game/bagbutton.h \
    itemboard.h \
    workspaceboard.h \
    bag.h \
    bagbutton.h

FORMS += \
        mainwindow.ui \
    itemboard.ui \
    workspaceboard.ui \
    bag.ui

DISTFILES += \
    src/Icon/Icon.png \
    src/Shader/test.frag \
    src/Shader/test.vert
INCLUDEPATH+= Packaged/


win32: LIBS += -L$$PWD/Packaged/tinyObj/ -ltinyobjloader

INCLUDEPATH += $$PWD/Packaged/tinyObj
DEPENDPATH += $$PWD/Packaged/tinyObj
