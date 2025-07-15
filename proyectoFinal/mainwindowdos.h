#ifndef MAINWINDOWDOS_H
#define MAINWINDOWDOS_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QLabel>
#include <QMainWindow>
#include "enemigo.h"
#include "jugador.h"
#include "qboxlayout.h"
#include "ui_mainwindow.h"

class MainWindowDos : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindowDos(QWidget *parent = nullptr);

private:
    QGraphicsScene *scene;
    QGraphicsView *view;
    QWidget *centralWidget;
    QVBoxLayout *mainLayout;
    Jugador *goku;
    Enemigo *roshi;

    void nivel2();
    void crearPlataformas();
    void detenerJuego();
    bool juegoTerminado;
    QLabel *lifeBarGokuLabels[5];
    QLabel *lifeBarRoshiLabels[5];
    QVector<QRectF> plataformasDerecha;

private slots:
    void actualizarBarraVidaRoshi();
    void actualizarBarraVidaGoku();
    void removerJugador();
    void removerEnemigo();
};

#endif // MAINWINDOWDO_H
