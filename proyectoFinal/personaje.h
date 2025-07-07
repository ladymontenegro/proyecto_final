#ifndef PERSONAJE_H
#define PERSONAJE_H
#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include <QTimer>

class Personaje : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Personaje(QPixmap _hojaSprite);
    short direccionXYajirobe;

    void movimientoSpriteYajirobe();

protected:
    qreal x;
    qreal y;
    QPixmap hojaSprite;
    QPixmap sprite;
    QPixmap hojaBarraVida;
    QPixmap barraVida;
    QTimer *tempMovimiento;
    unsigned short posicionX;
    unsigned short posicionY;
    unsigned int anchoSprite;
    unsigned int altoSprite;
    unsigned int conteoSprite;
    unsigned int cargaVida;

    void movimientoSprite(int direccion);
};

#endif // PERSONAJE_H
