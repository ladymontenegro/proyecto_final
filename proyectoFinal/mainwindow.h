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
    void crearMurosLaberinto();
    void ubicarBonificaciones();
    void mostrarBarraSuper(QPixmap &hojaSriteSuper);

private slots:
    void reaparecerBonificaciones();
    void manejarBonificacionRecolectada(Bonificacion* bonificacion);

private:
    void nivel1();
    Ui::MainWindow *ui;
    Jugador *goku;
    QGraphicsView *view;
    QGraphicsScene *scene;
    QTimer *timerReaparicion;
    QList<Bonificacion*> bonificacionesRecolectadas;
    QList<Bonificacion*> bonificacionesActivas;
    QLabel *superBarLabels[5];
    int cargaSuper;

    void actualizarBarraSuper();
};
#endif // MAINWINDOW_H
