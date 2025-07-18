QT       += core gui multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ataque.cpp \
    bonificacion.cpp \
    enemigo.cpp \
    jugador.cpp \
    main.cpp \
    mainwindow.cpp \
    mainwindowdos.cpp \
    obstaculo.cpp \
    personaje.cpp

HEADERS += \
    ataque.h \
    bonificacion.h \
    enemigo.h \
    jugador.h \
    mainwindow.h \
    mainwindowdos.h \
    obstaculo.h \
    personaje.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    multimedia.qrc

DISTFILES +=
