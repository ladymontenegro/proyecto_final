#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMainWindow>
#include <QPixmap>
#include <QLabel>
#include <QHBoxLayout>
#include "jugador.h"
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
    QGraphicsView *view;
    QGraphicsScene *scene;
    QWidget *centralWidget;
    QVBoxLayout *mainLayout;
    QTimer *timerReaparicion;
    QList<Bonificacion*> bonificacionesRecolectadas;
    QList<Bonificacion*> bonificacionesActivas;
    QLabel *superBarLabels[5];
    int cargaSuper;

    void actualizarBarraSuper();
    void crearBloquesCuadrado();
    void crearMurosLaberinto();
    void ubicarBonificaciones();
    void mostrarBarraSuper(QPixmap &hojaSriteSuper);
};
#endif // MAINWINDOW_H
