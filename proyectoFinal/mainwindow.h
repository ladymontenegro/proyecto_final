#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMainWindow>
#include <QPixmap>
#include <QLabel>
#include <QHBoxLayout>
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
    void nivel1();
    void nivel2();
    Ui::MainWindow *ui;
    Jugador *goku;
    Enemigo *roshi;
    QGraphicsView *view;
    QGraphicsScene *scene;
    QWidget *centralWidget;
    QVBoxLayout *mainLayout;
    QTimer *timerReaparicion;
    QList<Bonificacion*> bonificacionesRecolectadas;
    QList<Bonificacion*> bonificacionesActivas;
    QLabel *superBarLabels[5];
    QLabel *lifeBarGokuLabels[5];
    QLabel *lifeBarRoshiLabels[5];

    void actualizarBarraSuper();
    void actualizarBarraVidaRoshi();
    void actualizarBarraVidaGoku();
    void crearBloquesCuadrado();
    void crearMurosLaberinto();
    void crearPlataformas();
    void ubicarBonificaciones();
};
#endif // MAINWINDOW_H
