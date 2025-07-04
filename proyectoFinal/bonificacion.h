#ifndef BONIFICACION_H
#define BONIFICACION_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QTimer>

class Bonificacion : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

private:
    QPixmap sprite;
    QTimer *timerAnimacion;
    qreal posicionYInicial;
    qreal direccionAnimacion;
    unsigned short valorCarga;

public:
    explicit Bonificacion(const QPixmap &_sprite, QGraphicsItem *parent = nullptr);
    unsigned short getValorCarga() const;
    void iniciarAnimacionFlotacion();
    void detenerAnimacion();
    void actualizarAnimacion();
};

#endif // BONIFICACION_H
