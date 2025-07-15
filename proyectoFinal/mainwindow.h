#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMainWindow>
#include <QPixmap>
#include <QLabel>
#include <QHBoxLayout>
#include <QVideoWidget>
#include <QMediaPlayer>
#include "jugador.h"
#include "enemigo.h"
#include "bonificacion.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void reaparecerBonificaciones();
    void manejarBonificacionRecolectada(Bonificacion* bonificacion);
    void resetCargaSuperYActualizarBarra();

private:
    //general
    Ui::MainWindow *ui;
    Jugador *goku;
    Enemigo *roshi;
    Personaje *yajirobe;
    QGraphicsView *view;
    QGraphicsScene *scene;
    QWidget *centralWidget;
    QVBoxLayout *mainLayout;

    //cargar nivel uno
    void nivel1();
    void crearBloquesCuadrado();
    void crearMurosLaberinto();
    void ubicarBonificaciones();
    QTimer *timerReaparicion;
    QTimer *timerVictoria;
    QTimer *timerYajirobe;
    QList<Bonificacion*> bonificacionesRecolectadas;
    QList<Bonificacion*> bonificacionesActivas;
    QLabel *superBarLabels[5];

    //barras
    void actualizarBarraSuper();
    void iniciarTransicionYNivel2();
};
#endif // MAINWINDOW_H
