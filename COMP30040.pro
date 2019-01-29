#-------------------------------------------------
#
# Project created by QtCreator 2018-10-06T16:44:31
#
#-------------------------------------------------

QT += core gui opengl charts concurrent 3dcore 3drender 3dinput 3dextras multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = COMP30040
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS GL_SILENCE_DEPRECATION

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11 sdk_no_version_check

unix:!macx {
    LIBS += -lGLEW -lglfw -lGL -lX11 -lXi -lXrandr -lXxf86vm -lXinerama -lXcursor -lrt -lm -pthread -lGL -lGLU -lglut
}

macx: {
    INCLUDEPATH += /usr/local/Cellar/eigen/3.3.5/include/
    unix: LIBS += -L$$PWD/../../../../../usr/local/Cellar/glew/2.1.0/lib/ -lGLEW

    INCLUDEPATH += $$PWD/../../../../../usr/local/Cellar/glew/2.1.0/include
    DEPENDPATH += $$PWD/../../../../../usr/local/Cellar/glew/2.1.0/include

    unix: PRE_TARGETDEPS += $$PWD/../../../../../usr/local/Cellar/glew/2.1.0/lib/libGLEW.a
    LIBS += -framework Cocoa -framework CoreVideo -framework IOKit -framework GLUT
}



SOURCES += \
        main.cpp \
        mokelasersim.cpp \
    polarisationwindow.cpp \
    sampleobject.cpp \
    collideableobject.cpp \
    ray.cpp \
    polarisingfilter.cpp \
    pem.cpp \
    simulationthread.cpp \
    kerrrotationgraph.cpp \
    randomnoisecalculator.cpp \
    randomnoisechartview.cpp \
    threedimentionalvisualisation.cpp

HEADERS += \
        mokelasersim.h \
    collideableobject.h \
    ray.h \
    polarisationwindow.h \
    sampleobject.h \
    polarisingfilter.h \
    pem.h \
    simulationthread.h \
    kerrrotationgraph.h \
    randomnoisecalculator.h \
    randomnoisechartview.h \
    threedimentionalvisualisation.h


FORMS += \
        mokelasersim.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


RESOURCES += \
    obj.qrc


