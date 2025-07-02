#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMainWindow>
#include <QPixmap>

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

private slots:
    void nivel1();

private:
    Ui::MainWindow *ui;
    QGraphicsView *view;
    QGraphicsScene *scene;
};
#endif // MAINWINDOW_H
