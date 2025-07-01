#include "jugador.h"
#include <QGraphicsItem>
#include "bonificacion.h"
#include "qgraphicsscene.h"

Jugador::Jugador(QPixmap _hojaSprite)
    : Personaje(_hojaSprite)
{
    x = 187;
    y = 87;
    superCargado = false;
    setFlag(QGraphicsItem::ItemIsFocusable);
    hojaPoderGoku.load(":/multimedia/poderGoku.png");
    timerPoderGoku = new QTimer(this);
    connect(timerPoderGoku, &QTimer::timeout, this, &Jugador::poderGoku);
    timerPoderGoku->setInterval(100);
}

void Jugador::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_A:
        movimiento(-5, 0);
        movimientoSprite(2496);
        break;
    case Qt::Key_W:
        movimiento(0, -5);
        movimientoSprite(2432);
        break;
    case Qt::Key_S:
        movimiento(0, 5);
        movimientoSprite(2560);
        break;
    case Qt::Key_D:
        movimiento(5, 0);
        movimientoSprite(2624);
        break;
    case Qt::Key_Space:
        iniciarPoderGoku();
        break;
    default:
        QGraphicsItem::keyPressEvent(event);
        break;
    }
}

void Jugador::movimiento(int dx, int dy)
{
    // Posicion actual antes de intentar el movimiento
    qreal oldX = x;
    qreal oldY = y;

    // Intenta mover al jugador
    setPos(x + dx, y + dy);

    // Actualiza la nueva posicion
    x = pos().x();
    y = pos().y();

    QList<QGraphicsItem *> itemsChocados
        = collidingItems(); // Obtiene los elementos con los que colisiona

    for (QGraphicsItem *item : itemsChocados) {        // Itera sobre los elementos colisionados
        if (item->type() == QGraphicsRectItem::Type) { // Si colisiona con un rectangulo
            // Deshace el movimiento y pone la posicion anterior
            setPos(oldX, oldY);
            x = oldX; //Actualizar variables miembro
            y = oldY;
            break;
        } /*else if (item->type() == QGraphicsPixmapItem::Type) {
            Bonificacion *bonificacion = dynamic_cast<Bonificacion *>(item); 
            if (bonificacion) {                                            
                scene()->removeItem(bonificacion); 
                delete bonificacion;
            }
        }*/
    }
}

void Jugador::iniciarPoderGoku()
{
    contadorSpritePoderGoku = 0; // Reinicia la animacion al inicio
    poderGoku();                 // Muestra el primer frame inmediatamente
    timerPoderGoku->start();     // Inicia el timer para el resto de la animacion
}

void Jugador::poderGoku()
{
    if (contadorSpritePoderGoku <= 7) {
        posicionXPoderGoku = 0;
        anchoSpritePoderGoku = 66; // Ancho por defecto
        posicionXPoderGoku = contadorSpritePoderGoku * anchoSpritePoderGoku;
        spritePoderGoku = hojaPoderGoku.copy(posicionXPoderGoku, 0, anchoSpritePoderGoku, 128);
        QPixmap spriteEscalado = spritePoderGoku.scaled(22, 32);
        setPixmap(spriteEscalado);

        if (contadorSpritePoderGoku == 6) {
            anchoSpritePoderGoku = 283;
            posicionXPoderGoku = 410;
            spritePoderGoku = hojaPoderGoku.copy(posicionXPoderGoku, 0, anchoSpritePoderGoku, 128);
            QPixmap spriteEscalado = spritePoderGoku.scaled(70, 32);
            setPixmap(spriteEscalado);
        } else if (contadorSpritePoderGoku == 7) {
            anchoSpritePoderGoku = 283;
            posicionXPoderGoku = 693;
            spritePoderGoku = hojaPoderGoku.copy(posicionXPoderGoku, 0, anchoSpritePoderGoku, 128);
            QPixmap spriteEscalado = spritePoderGoku.scaled(70, 32);
            setPixmap(spriteEscalado);
        }

        contadorSpritePoderGoku++;

    } else {
        // La animacion termino, se para el temporizador
        timerPoderGoku->stop();
    }
}
