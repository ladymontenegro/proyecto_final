#ifndef PERSONAJE_H
#define PERSONAJE_H
#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include <QTimer>

class Personaje : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Personaje(QPixmap _hojaSprite,
              unsigned short _x,
              unsigned short _y,
              unsigned short _anchoSprite,
              unsigned short _altoSprite,
              unsigned short _anchoSpriteEscalar,
              unsigned short _altoSpriteEscalar);

    virtual ~Personaje();

    unsigned short getCargaVida() const;
    void movimientoSpriteYajirobe();
    bool verificarVictoriaNivel1();

    short direccionXYajirobe;
    qreal x;
    qreal y;

protected:
    QPixmap hojaSprite;
    QPixmap sprite;
    QPixmap hojaBarraVida;
    QPixmap barraVida;
    unsigned short posicionX;
    unsigned short posicionY;
    unsigned int anchoSprite;
    unsigned int altoSprite;
    unsigned int anchoSpriteEscalar;
    unsigned int altoSpriteEscalar;
    unsigned int conteoSprite;
    unsigned int cargaVida;

    void movimientoSprite(int direccion, unsigned short cantidadDeSprites);
};

#endif // PERSONAJE_H
