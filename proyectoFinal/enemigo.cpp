#include "enemigo.h"
#include "ataque.h"
#include <QGraphicsScene>

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
    , timerMovimiento(nullptr)
{
    timerDisparo = new QTimer(this);
    connect(timerDisparo, &QTimer::timeout, this, &Enemigo::disparar);
    timerDisparo->start(800); // cada 3s dispara

    timerMovimientoGolpe = new QTimer(this);
    connect(timerMovimientoGolpe, &QTimer::timeout, this, &Enemigo::movimientoGolpe);
    timerMovimientoGolpe->setInterval(100);

}

Enemigo::~Enemigo()
{
    if (timerMovimiento) {
        timerMovimiento->stop();
        delete timerMovimiento;
        timerMovimiento = nullptr;
    }

    if (timerMovimientoGolpe) {
        timerMovimientoGolpe->stop();
        delete timerMovimientoGolpe;
        timerMovimientoGolpe = nullptr;
    }

    if (timerDisparo) {
        timerDisparo->stop();
        delete timerDisparo;
        timerDisparo = nullptr;
    }

    if (timerSalto) {
        timerSalto->stop();
        delete timerSalto;
        timerSalto = nullptr;
    }

    qDebug() << "Destructor de Enemigo";
}

void Enemigo::setJugadorObjetivo(Jugador *jugador) {
    jugadorObjetivo = jugador;
}

void Enemigo::setPlataformas(const QVector<QRectF> &listaPlataformas) {
    plataformas = listaPlataformas;
    if (!plataformas.isEmpty()) {
        setPos(plataformas[0].x() + 20, plataformas[0].y() - pixmap().height());
        plataformaActual = 0;

        puedeSaltarEntrePlataformas = true;

        if (!timerMovimiento) {
            timerMovimiento = new QTimer(this);
            connect(timerMovimiento, &QTimer::timeout, this, &Enemigo::moverEntrePlataformas);
        }
        timerMovimiento->start(1000);
    }
}

void Enemigo::moverEntrePlataformas() {
    if (plataformas.isEmpty() || !puedeSaltarEntrePlataformas) return;

    plataformaActual = (plataformaActual + 1) % plataformas.size();
    destinoSalto = QPointF(plataformas[plataformaActual].x() + 20,
                           plataformas[plataformaActual].y() - pixmap().height());
    origenSalto = pos();
    tiempoSalto = 0;

    if (!timerSalto) {
        timerSalto = new QTimer(this);
        connect(timerSalto, &QTimer::timeout, this, &Enemigo::animarSalto);
    }

    timerSalto->start(40);
}

void Enemigo::disparar() {
    if (piedraActiva || !jugadorObjetivo) return;

    piedraActiva = true;
    puedeSaltarEntrePlataformas = false;

    iniciarMovimientoGolpe();

    QPointF posicionJugador = jugadorObjetivo->sceneBoundingRect().center();
    QPointF posicionEnemigo = pos();
    unsigned short direccion = (posicionJugador.x() < posicionEnemigo.x()) ? 1 : 0;

    Ataque *piedra = new Ataque(QPixmap(":/multimedia/piedra.png"), posicionEnemigo, posicionJugador, direccion);
    piedra->usarMovimientoParabolico();

    connect(piedra, &Ataque::destruido, this, [this]() {
        piedraActiva = false;
        puedeSaltarEntrePlataformas = true;
    });

    if (scene()) scene()->addItem(piedra);
}

void Enemigo::iniciarMovimientoGolpe(){
    contadorspriteMovimientoGolpe = 0;
    movimientoGolpe();
    timerMovimientoGolpe->start();
}

void Enemigo::movimientoGolpe(){
    if (contadorspriteMovimientoGolpe <= 5) {
        posicionGolpeX = 0;
        posicionGolpeX = contadorspriteMovimientoGolpe * anchoSprite;
        QPixmap spriteMovimientoAgacho = hojaSprite.copy(posicionGolpeX, 3264, anchoSprite, altoSprite);
        //posicionX, posicionY, anchoSprite, altoSprite
        QPixmap spriteEscalado = spriteMovimientoAgacho.scaled(50, 50);

        setPixmap(spriteEscalado);
        contadorspriteMovimientoGolpe++;

    } else {
        timerMovimientoGolpe->stop();
    }
}

void Enemigo::animarSalto() {
    const float duracionTotal = 20; //para velocidad

    tiempoSalto++;

    float t = tiempoSalto / duracionTotal;
    if (t > 1.0f) {
        setPos(destinoSalto);
        timerSalto->stop();
        return;
    }

    float nuevoX = origenSalto.x() + t * (destinoSalto.x() - origenSalto.x());
    float altura = -50 * sin(M_PI * t); //pico en la mitad

    float nuevoY = origenSalto.y() + t * (destinoSalto.y() - origenSalto.y()) + altura;

    setPos(nuevoX, nuevoY);

    movimientoSprite(1728, 5);
}
