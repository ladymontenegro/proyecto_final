#include "enemigo.h"
#include "ataque.h"

Enemigo::Enemigo(QPixmap _hojaSprite,
                 unsigned short _x,
                 unsigned short _y,
                 unsigned short _anchoSprite,
                 unsigned short _altoSprite,
                 unsigned short _anchoSpriteEscalar,
                 unsigned short _altoSpriteEscalar)
    : Personaje(_hojaSprite,
                _x,
                _y,
                _anchoSprite,
                _altoSprite,
                _anchoSpriteEscalar,
                _altoSpriteEscalar)
{
    timerMovimientoAtaque = new QTimer(this);
}

Enemigo::~Enemigo()
{
    if (timerMovimientoAtaque) {
        timerMovimientoAtaque->stop();
        delete timerMovimientoAtaque;
        timerMovimientoAtaque = nullptr;
    }

    qDebug() << "Destructor de Enemigo";
}

void Enemigo::movimientoAtaque(){
    QList<QGraphicsItem *> itemsChocados = collidingItems(); //obtiene los elementos con los que colisiona

    for (QGraphicsItem *item : itemsChocados) {
        if (item->type() == QGraphicsPixmapItem::Type) {
            Ataque *ataque = dynamic_cast<Ataque *>(item);
            if(ataque){
                emit golpeRecibido();
            }
        }
    }
}
