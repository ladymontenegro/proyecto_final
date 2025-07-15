#include "jugador.h"
#include <QDebug>
#include <QGraphicsItem>
#include "ataque.h"
#include "bonificacion.h"
#include "qgraphicsscene.h"
#include <typeinfo>

Jugador::Jugador(QPixmap _hojaSprite,
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
    , cargaSuper(0)
    , enPlataforma(false)
    , validoCargarSuper(false)
    , nivelDosActivo(true)
    , puntoReinicioX(0)
    , puntoReinicioY(0)
    , porCaida(false)
{
    //validoCargarSuper = false;
    setFlag(QGraphicsItem::ItemIsFocusable);
    hojaMovimientoPoderGoku.load(":/multimedia/movimientoPoderGoku.png");

    timerMovimientoPoderGoku = new QTimer(this);
    connect(timerMovimientoPoderGoku, &QTimer::timeout, this, &Jugador::movimientoPoderGoku);
    timerMovimientoPoderGoku->setInterval(100);

    timerMovimientoSalto = new QTimer(this);
    connect(timerMovimientoSalto, &QTimer::timeout, this, &Jugador::movimientoSalto);
    timerMovimientoSalto->setInterval(30);

    timerMovimientoAgacho = new QTimer(this);
    connect(timerMovimientoAgacho, &QTimer::timeout, this, &Jugador::movimientoAgacho);
    timerMovimientoAgacho->setInterval(100);

    timerGravedad = new QTimer(this);
    connect(timerGravedad, &QTimer::timeout, this, &Jugador::aplicarGravedad);

}

Jugador::~Jugador()
{
    if (timerMovimientoPoderGoku) {
        timerMovimientoPoderGoku->stop();
        delete timerMovimientoPoderGoku;
        timerMovimientoPoderGoku = nullptr;
    }

    if (timerMovimientoSalto) {
        timerMovimientoSalto->stop();
        delete timerMovimientoSalto;
        timerMovimientoSalto = nullptr;
    }

    if (timerMovimientoAgacho) {
        timerMovimientoAgacho->stop();
        delete timerMovimientoAgacho;
        timerMovimientoAgacho = nullptr;
    }

    if (timerGravedad) {
        timerGravedad->stop();
        delete timerGravedad;
        timerGravedad = nullptr;
    }

    qDebug() << "Destructor de Jugador";
}

unsigned short Jugador::getCargaSuper(){return cargaSuper;}

void Jugador::setValidoCargarSuper(bool estado){validoCargarSuper = estado;}

void Jugador::setCargaSuper(unsigned short nuevaCarga){cargaSuper = nuevaCarga;}

void Jugador::setPuntoReinicio(qreal x, qreal y) {
    puntoReinicioX = x;
    puntoReinicioY = y;
}

//----------------- MOVIMIENTO GENERAL -----------------


void Jugador::keyPressEvent(QKeyEvent *event)
{
    if (!nivelDosActivo) {
        if (!teclasPresionadas.contains(event->key())) {
            teclasPresionadas.append(event->key()); //para que guarde la tecla, si no esta repetida
        }

        switch (event->key()) {
        case Qt::Key_A:
            movimiento(-5, 0);
            movimientoSprite(2496, 8);
            ultimaDireccion = 1;
            break;
        case Qt::Key_W:
            movimiento(0, -5);
            movimientoSprite(2432, 8);
            ultimaDireccion = 2;
            break;
        case Qt::Key_S:
            movimiento(0, 5);
            movimientoSprite(2560, 8);
            ultimaDireccion = 3;
            break;
        case Qt::Key_D:
            movimiento(5, 0);
            movimientoSprite(2624, 8);
            ultimaDireccion = 0;
            break;
        case Qt::Key_Space:
            if (validoCargarSuper) {
                iniciarMovimientoPoderGoku();
                validoCargarSuper = false;
                emit poderLanzado();
                break;
            }
            break;
        default:
            QGraphicsItem::keyPressEvent(event);
            break;
        }
    } else if (nivelDosActivo) {
        if (!teclasPresionadas.contains(event->key())) {
            teclasPresionadas.append(event->key()); //para que guarde la tecla, si no esta repetida
        }

        switch (event->key()) {
        case Qt::Key_A:
            movimientoNivelDos(-5, 0);
            movimientoSprite(2496, 8);
            ultimaDireccion = 1;
            break;
        case Qt::Key_W:
            if (enPlataforma) {
                iniciarMovimientoSalto();
            }
            break;
        case Qt::Key_S:
            iniciarMovimientoAgacho();
            break;
        case Qt::Key_D:
            movimientoNivelDos(5, 0);
            movimientoSprite(2624, 8);
            ultimaDireccion = 0;
            break;
        case Qt::Key_Space:
            iniciarMovimientoPoderGoku();
            break;
        default:
            QGraphicsItem::keyPressEvent(event);
            break;
        }
    }
}

void Jugador::keyReleaseEvent(QKeyEvent *event)
{
    teclasPresionadas.removeAll(event->key());

    //solo procesar teclas de movimiento
    int key = event->key();
    if (key != Qt::Key_A && key != Qt::Key_D && key != Qt::Key_W && key != Qt::Key_S) {
        return;
    }

    if (teclasPresionadas.isEmpty()) {
        mostrarSpriteQuieto();
    }
}

void Jugador::mostrarSpriteQuieto()
{
    //guardar el conteo actual de sprites
    int conteoAnterior = conteoSprite;

    switch(ultimaDireccion) {
    case 0:
        movimientoSprite(2368, 0);
        break;
    case 1:
        movimientoSprite(2240, 0);
        break;
    case 2:
        movimientoSprite(2176, 0);
        break;
    case 3:
        movimientoSprite(2304, 0);
        break;
    }
    //restaurar el conteo original y mantenerlo fijo
    conteoSprite = conteoAnterior;

    //crear el sprite estatico (primer frame)
    sprite = hojaSprite.copy(0, posicionY, anchoSprite, altoSprite);
    QPixmap spriteEscalado = sprite.scaled(anchoSpriteEscalar, altoSpriteEscalar);
    setPixmap(spriteEscalado);
}


void Jugador::movimiento(int dx, int dy)
{
    //posicion actual antes de intentar el movimiento
    qreal oldX = x;
    qreal oldY = y;

    //intenta mover al jugador
    setPos(x + dx, y + dy);

    //actualiza la nueva posicion
    x = pos().x();
    y = pos().y();

    QList<QGraphicsItem *> itemsChocados = collidingItems(); //obtiene los elementos con los que colisiona

    for (QGraphicsItem *item : itemsChocados) {
        if (item->type() == QGraphicsRectItem::Type) {
            //deshace el movimiento y pone la posicion anterior
            setPos(oldX, oldY);
            x = oldX; //actualiza variables miembro
            y = oldY;
            break;
        } else if (item->type() == QGraphicsPixmapItem::Type) {
            Bonificacion *bonificacion = dynamic_cast<Bonificacion *>(item);
            if (bonificacion) {
                cargaSuper += 25;
                emit bonificacionRecolectada(bonificacion);
            }
        }
    }
}

void Jugador::desactivarControles() {
    // Apagar timers
    if (timerMovimientoPoderGoku && timerMovimientoPoderGoku->isActive())
        timerMovimientoPoderGoku->stop();

    if (timerMovimientoSalto && timerMovimientoSalto->isActive())
        timerMovimientoSalto->stop();

    if (timerMovimientoAgacho && timerMovimientoAgacho->isActive())
        timerMovimientoAgacho->stop();

    if (timerGravedad && timerGravedad->isActive())
        timerGravedad->stop();

    // Borrar teclas presionadas
    teclasPresionadas.clear();

    // Mostrar sprite quieto para no dejar un frame congelado
    mostrarSpriteQuieto();

    // Desactivar respuesta a teclas
    setFlag(QGraphicsItem::ItemIsFocusable, false);
    clearFocus();

    qDebug() << "Controles de jugador desactivados.";
}

//----------------- ANIMACIONES DE MOVIMIENTO -----------------

void Jugador::iniciarMovimientoPoderGoku()
{
    cargaSuper = 0;
    contadorspriteMovimientoPoderGoku = 0; // Reinicia la animacion al inicio
    movimientoPoderGoku();                 // Muestra el primer frame inmediatamente
    timerMovimientoPoderGoku->start();     // Inicia el timer para el resto de la animacion
}

void Jugador::movimientoPoderGoku()
{
    if (contadorspriteMovimientoPoderGoku <= 6) {
        posicionXPoderGoku = 0;
        anchoSpriteMovimientoPoderGoku = 66; // Ancho por defecto
        posicionXPoderGoku = contadorspriteMovimientoPoderGoku * anchoSpriteMovimientoPoderGoku;
        spriteMovimientoPoderGoku = hojaMovimientoPoderGoku.copy(posicionXPoderGoku, 0, anchoSpriteMovimientoPoderGoku, 128);

        QPixmap spriteEscalado;
        if(nivelDosActivo){
            spriteEscalado = spriteMovimientoPoderGoku.scaled(50, 72);
        } else {
            spriteEscalado = spriteMovimientoPoderGoku.scaled(22, 32);
        }

        //espejar el sprite
        if (ultimaDireccion == 1) {
            spriteEscalado = spriteEscalado.transformed(QTransform().scale(-1, 1));
        }

        setPixmap(spriteEscalado);
        contadorspriteMovimientoPoderGoku++;

    } else {
        timerMovimientoPoderGoku->stop();
        lanzarPoderGoku();
        mostrarSpriteQuieto();
    }
}

void Jugador::lanzarPoderGoku()
{
    QPointF inicio = {ultimaDireccion != 1 ? x + 20 : x - 20, y + 4};
    QPixmap sprite(":/multimedia/poderGoku.png");

    QPixmap spriteEscalado;
    if(nivelDosActivo){
        spriteEscalado = sprite.scaled(40, 30);
    } else {
        spriteEscalado = sprite.scaled(30, 20);
    }

    Ataque *ataqueGoku = new Ataque(this, spriteEscalado, inicio, ultimaDireccion);
    scene()->addItem(ataqueGoku);
}

void Jugador::iniciarMovimientoSalto(){
    if (!enPlataforma) return;

    contadorspriteMovimientoSalto = 0;

    velocidadVertical = -15;  //valor negativo para subirs
    movimientoAgacho();

    timerMovimientoSalto->start();
}

void Jugador::movimientoSalto(){
    //actualizar poicion
    movimientoNivelDos(0, velocidadVertical);

    //reducir impulso vertical (gravedad)
    velocidadVertical += 1.0;  //gravedad suave

    QPixmap spriteEscalado;

    if (contadorspriteMovimientoSalto <= 4) {
        posicionSaltoX = 0;
        posicionSaltoX = contadorspriteMovimientoSalto * anchoSprite;
        QPixmap spriteMovimientoSalto = hojaSprite.copy(posicionSaltoX, 1856, anchoSprite, altoSprite);
        //posicionX, posicionY, anchoSprite, altoSprite
        spriteEscalado = spriteMovimientoSalto.scaled(50, 50);

        //espejar el sprite
        if (ultimaDireccion == 1) {
            spriteEscalado = spriteEscalado.transformed(QTransform().scale(-1, 1));
        }

        setPixmap(spriteEscalado);
        contadorspriteMovimientoSalto++;

    } else if (enPlataforma){
        timerMovimientoSalto->stop();
        mostrarSpriteQuieto();
    }
    else {
        //para que cuando caiga quede un sprite especifico
        QPixmap spriteCaida = hojaSprite.copy(256, 1856, anchoSprite, altoSprite);
        spriteEscalado = spriteCaida.scaled(50, 50);
        //espejar el sprite
        if (ultimaDireccion == 1) {
            spriteEscalado = spriteEscalado.transformed(QTransform().scale(-1, 1));
        }
        setPixmap(spriteEscalado);
    }
}

void Jugador::iniciarMovimientoAgacho(){
    contadorspriteMovimientoAgacho = 0;
    movimientoAgacho();
    timerMovimientoAgacho->start();
}

void Jugador::movimientoAgacho(){
    if (contadorspriteMovimientoAgacho <= 3) {
        posicionAgachoX = 0;
        posicionAgachoX = contadorspriteMovimientoAgacho * anchoSprite;
        QPixmap spriteMovimientoAgacho = hojaSprite.copy(posicionAgachoX, 1280, anchoSprite, altoSprite);
        //posicionX, posicionY, anchoSprite, altoSprite
        QPixmap spriteEscalado = spriteMovimientoAgacho.scaled(50, 50);

        //espejar el sprite
        if (ultimaDireccion == 1) {
            spriteEscalado = spriteEscalado.transformed(QTransform().scale(-1, 1));
        }

        setPixmap(spriteEscalado);
        contadorspriteMovimientoAgacho++;

    } else {
        timerMovimientoAgacho->stop();
    }
}


//----------------- MOVIMIENTO NIVEL II -----------------

void Jugador::movimientoNivelDos(int dx, int dy) {

    timerGravedad->start(16);

    //definir limites de la ventana
    const qreal limiteIzquierdo = 0;
    const qreal limiteDerecho = 1000;
    const qreal limiteSuperior = 0;
    const qreal limiteInferior = 600;

    qreal newX = x;
    qreal newY = y;

    if (dx != 0) newX = x + dx;
    if (dy != 0) newY = y + dy;

    QRectF bordeJugador = boundingRect();
    QRectF jugadorRect = QRectF(
        newX + bordeJugador.left(),
        newY + bordeJugador.top(),
        bordeJugador.width(),
        bordeJugador.height()
        );

    //verificar colision con los limites
    if (jugadorRect.left() < limiteIzquierdo) {
        newX = limiteIzquierdo - bordeJugador.left();
    }
    else if (jugadorRect.right() > limiteDerecho) {
        newX = limiteDerecho - bordeJugador.right();
    }

    if (jugadorRect.top() < limiteSuperior) {
        newY = limiteSuperior - bordeJugador.top();
    }
    else if (jugadorRect.bottom() > limiteInferior) {
        porCaida = true;
        perderVida();
        return;
    }

    if (dx != 0 || dy != 0) {
        setPos(newX, newY);
        x = newX;
        y = newY;
    }

    //actualizar estado de plataforma
    actualizarEnPlataforma();
}

void Jugador::actualizarEnPlataforma() {
    enPlataforma = false;

    //area de los pies
    QRectF pieRect(
        boundingRect().left() + 5,
        boundingRect().bottom() - 1,
        boundingRect().width() - 10,
        1
        );
    pieRect.translate(pos());

    for (QGraphicsItem* item : collidingItems()) {
        if (item->type() == QGraphicsRectItem::Type) {
            QRectF plataformaRect = item->boundingRect().translated(item->pos());

            if (pieRect.intersects(plataformaRect)) {
                if ((velocidadVertical >= 0 || enPlataforma) &&
                    pieRect.bottom() >= plataformaRect.top() &&
                    pieRect.bottom() <= plataformaRect.top() + 10) {

                    enPlataforma = true;

                    //ajuste de posicion
                    qreal nuevaY = plataformaRect.top() - boundingRect().height();
                    setPos(x, nuevaY);
                    break;
                }
            }
        }
    }

    if (enPlataforma) {
        if (timerMovimientoSalto->isActive() && velocidadVertical > 0) {
            timerMovimientoSalto->stop();
            mostrarSpriteQuieto();
        }
        velocidadVertical = 0;
    } else {
        //solo si no esta en plataforma
        if (!timerMovimientoSalto->isActive()) {
            velocidadVertical += 1.0;
        }
    }
}

void Jugador::aplicarGravedad() {
    //solo aplicar si no esta saltando activamente
    if (!timerMovimientoSalto->isActive() && !enPlataforma) {
        movimientoNivelDos(0, 5);  //gravedad normal
    }
}

void Jugador::perderVida() {
    cargaVida--;
    emit vidaCambiada(cargaVida);

    if(cargaVida > 0) {
        if(porCaida) {
            setPos(puntoReinicioX, puntoReinicioY);
            x = pos().x();
            y = pos().y();
            velocidadVertical = 0;
            enPlataforma = true;
        } else {porCaida = false;}
    } else {
        emit jugadorMurio();
    }
}
