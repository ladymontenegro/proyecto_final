#include "mainwindow.h"
#include <QGraphicsItem>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QVector>
#include <QGraphicsProxyWidget>
#include "QGraphicsPixmapItem"
#include "bonificacion.h"
#include "jugador.h"
#include "enemigo.h"
#include "obstaculo.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , goku(nullptr)
    , roshi(nullptr)
{
    ui->setupUi(this);

    //los widgets son como los contenedores y lo layouts como delimitadores para organizar

    //el espcio central, va a contener a todo
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget); //se pone como area principal

    //es un layout vertical que va en el centraWidget, controla el espacio de arriba a abajo
    mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    //se crea la escena
    scene = new QGraphicsScene(this);

    //se crea la vista y le damos color
    view = new QGraphicsView(scene, this);

    //quitar barras de desplazamiento
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //nivel1();
    nivel2();
}

MainWindow::~MainWindow()
{
    // Limpiar listas de bonificaciones
    for(Bonificacion* b : bonificacionesActivas) {
        scene->removeItem(b);
        delete b;
    }
    bonificacionesActivas.clear();

    for(Bonificacion* b : bonificacionesRecolectadas) {
        delete b;
    }
    bonificacionesRecolectadas.clear();
    delete ui;
}

//----------------- METODOS PARA LA CARGA DEL NIVEL UNO -----------------
void MainWindow::nivel1()
{
    //CONFIGURAR LA VENTANA

    resize(700, 700);

    //este sera el contenedor donde ira lo de la barra
    QWidget *superBarContainer = new QWidget(this);
    superBarContainer->setFixedHeight(50); //le damos altura
    superBarContainer->setStyleSheet("background-color: #FADA7A;"); //le damos color

    //es un layout vertical para organizar la barra y el texto
    QHBoxLayout *superBarLayout = new QHBoxLayout(superBarContainer);
    superBarLayout->setSpacing(350);
    superBarLayout->setContentsMargins(10, 5, 40, 5);

    //este es un layout para poner solo la barra
    QHBoxLayout *barraLayout = new QHBoxLayout();
    barraLayout->setSpacing(0);
    barraLayout->setContentsMargins(0, 0, 0, 0);

    //al contenedor principal le agregamos el widget de la barra y la vista
    mainLayout->addWidget(superBarContainer);
    mainLayout->addWidget(view);

    //cargar titulo
    QPixmap title(":/multimedia/title.png");
    QPixmap titleScaled = title.scaled(128, 50);
    QLabel *titleLabel = new QLabel(superBarContainer);
    titleLabel->setPixmap(titleScaled);

    //cargar la hoja de sprites
    QPixmap hojaSpriteSuper(":/multimedia/barraSuper.png");

    // Crear los labels para cada estado
    for(int i = 0; i < 5; i++) {
        superBarLabels[i] = new QLabel(superBarContainer);
        superBarLabels[i]->setFixedSize(200, 40); // Tamaño fijo

        // Obtener el sprite correspondiente
        int y = i * 100; // Ajusta según tu hoja de sprites
        QPixmap sprite = hojaSpriteSuper.copy(0, y, 500, 100);
        QPixmap scaled = sprite.scaled(200, 40, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

        superBarLabels[i]->setPixmap(scaled);
        superBarLabels[i]->setVisible(false); // Inicialmente ocultos
        barraLayout->addWidget(superBarLabels[i]);
    }

    superBarLayout->addStretch();
    superBarLayout->addLayout(barraLayout);
    superBarLayout->addWidget(titleLabel);
    superBarLayout->addStretch();

    //ELEMENTOS DE LA ESCENA
    //crear el laberinto
    QPixmap imagenLaberinto(":/multimedia/laberinto.png");
    QPixmap imagenEscaladaLaberinto = imagenLaberinto.scaled(700, 700);
    QGraphicsPixmapItem *laberinto = new QGraphicsPixmapItem(imagenEscaladaLaberinto);
    laberinto->setZValue(-100);
    scene->addItem(laberinto);
    scene->setSceneRect(laberinto->boundingRect());
    crearMurosLaberinto();
    crearBloquesCuadrado();

    //AGREGAR PERSONAJES
    //agregar a Goku
    QPixmap spriteGoku(":/multimedia/goku.png");
    goku = new Jugador(spriteGoku, 267, 271, 64, 64, 22, 22);
    goku->nivelDosActivo = false;
    goku->setFocus();
    goku->setPos(goku->x, goku->y);
    scene->addItem(goku);

    //inicializar barra
    actualizarBarraSuper();

    //agregar a Yajirobe
    QPixmap spriteYajirobe(":/multimedia/yajirobe.png");
    Personaje *yajirobe = new Personaje(spriteYajirobe, 350, 330, 63, 48, 25, 25);
    yajirobe->direccionXYajirobe = -3;
    scene->addItem(yajirobe);
    yajirobe->setPos(yajirobe->x, yajirobe->y);

    //Timer para que se mueva solo
    QTimer *timerYajirobe = new QTimer(yajirobe);
    QObject::connect(timerYajirobe, &QTimer::timeout, [=]() {yajirobe->movimientoSpriteYajirobe();});
    timerYajirobe->start(100);

    //cargar las bonificaciones y señales
    timerReaparicion = new QTimer(this);
    timerReaparicion->setInterval(5000);
    connect(timerReaparicion, &QTimer::timeout, this, &MainWindow::reaparecerBonificaciones);
    connect(goku, &Jugador::bonificacionRecolectada, this, &MainWindow::manejarBonificacionRecolectada);
    connect(goku, &Jugador::poderLanzado, this, &MainWindow::resetCargaSuperYActualizarBarra);
    ubicarBonificaciones();

    // Timer para que verificar la victoria del nivel 1
    QTimer *timerVictoria = new QTimer(this);
    connect(timerVictoria, &QTimer::timeout, [=]() {
        if (yajirobe->verificarVictoriaNivel1()) {
            timerVictoria->stop(); // Detiene el timer al ganar
            timerYajirobe->stop();
            QMessageBox::information(this, "¡¡VICTORIA!!", "¡Has rescatado a Yajirobe!");
        }
    });
    timerVictoria->start(100); // chequear cada 100 ms

    QMessageBox::information(nullptr, "NIVEL 1", "!!Yajirobe esta atrapado!!\nAumenta tu super con comida y utiliza tu "
                                                 "poder para rescatar a Yajirobe");
}

void MainWindow::crearMurosLaberinto()
{
    QVector<QRectF> posicionesMuros
        = {{44, 0, 8, 274}, {44, 266, 120, 8}, {44, 90, 80, 8}, {98, 0, 8, 55},
            {156, 0, 8, 140}, {100, 132, 64, 8}, {100, 132, 8, 54}, {44, 179, 64, 8},
            {0, 312, 58, 8}, {100, 312, 63, 8}, {156, 266, 8, 50}, {100, 312, 8, 143},
            {155, 179, 8, 51}, {209, 45, 8, 140}, {155, 179, 263, 8}, {209, 90, 98, 8},
            {209, 45, 32, 8}, {333, 0, 8, 187}, {333, 45, 89, 8}, {395, 87, 8, 57},
            {448, 87, 8, 57}, {395, 136, 59, 8}, {210, 221, 8, 144}, {210, 221, 44, 8},
            {154, 356, 65, 8}, {154, 356, 8, 53},  {100, 400, 63, 8}, {0, 449, 109, 8},
            {271, 447, 8, 53}, {211, 447, 68, 8}, {211, 401, 8, 52},
            {272, 311, 8, 97}, {274, 403, 52, 8}, {316, 403, 8, 180}, {287, 268, 168, 8},
            {312, 220, 145, 8}, {448, 220, 8, 57}, {395, 270, 8, 53}, {333, 314, 68, 8},
            {333, 314, 8, 34}, {465, 0, 8, 54}, {465, 45, 69, 8}, {476, 314, 83, 8},
            {493, 90, 8, 144}, {493, 90, 94, 8}, {493, 224, 176, 8}, {543, 270, 63, 8},
            {544, 223, 8, 54}, {44, 492, 236, 8}, {546, 132, 8, 33}, {546, 132, 113, 8},
            {649, 132, 8, 56}, {649, 179, 259, 8}, {902, 90, 8, 97}, {788, 0, 8, 100},
            {578, 45, 8, 53}, {578, 45, 219, 8}, {714, 45, 8, 97}, {714, 136, 141, 8},
            {787, 179, 8, 55}, {912, 47, 64, 8}, {151, 843, 8, 178}, {954, 179, 68, 8},
            {964, 314, 57, 8}, {964, 222, 8, 100}, {842, 222, 132, 8}, {902, 268, 8, 215},
            {818, 403, 199, 8}, {818, 403, 8, 98}, {765, 268, 161, 8}, {816, 268, 8, 56},
            {713, 314, 113, 8}, {717, 220, 8, 56}, {652, 268, 72, 8}, {652, 268, 8, 96},
            {652, 357, 71, 8}, {715, 357, 8, 99}, {715, 450, 64, 8}, {763, 403, 32, 8},
            {789, 357, 8, 51}, {789, 357, 41, 8}, {964, 492, 57, 8}, {964, 492, 8, 50},
            {761, 535, 210, 8}, {761, 492, 8, 51}, {711, 492, 53, 8}, {711, 492, 8, 91},
            {898, 576, 59, 8}, {960, 623, 61, 8}, {873, 623, 39, 8}, {873, 623, 8, 228},
            {873, 843, 44, 8}, {911, 843, 8, 54}, {816, 535, 8, 190}, {925, 667, 50, 8},
            {925, 667, 8, 54}, {925, 714, 47, 8}, {962, 714, 8, 38}, {962, 806, 61, 8},
            {763, 578, 8, 88},  {812, 760, 70, 8},  {960, 895, 61, 8},   {960, 868, 8, 79},
            {922, 940, 44, 8},  {713, 716, 111, 8}, {713, 623, 8, 102},  {674, 623, 48, 8},
            {683, 760, 8, 187}, {816, 936, 8, 83},  {816, 936, 60, 8},   {816, 806, 8, 95},
            {763, 806, 58, 8},  {763, 760, 8, 55},  {763, 891, 61, 8},   {763, 891, 8, 56},
            {733, 940, 39, 8},  {557, 760, 215, 8}, {683, 849, 55, 8},   {578, 939, 109, 8},
            {580, 667, 87, 8},  {610, 667, 8, 99}, {610, 802, 8, 52},   {557, 760, 8, 42},
            {546, 847, 67, 8},  {546, 847, 8, 51},  {493, 891, 61, 8},   {493, 891, 8, 130},
            {365, 669, 138, 8}, {274, 625, 84, 8},  {391, 669, 8, 50},   {391, 714, 56, 8},
            {495, 669, 8, 143}, {438, 806, 66, 8},  {438, 806, 8, 92},   {241, 940, 205, 8},
            {0, 940, 159, 8},   {966, 48, 8, 77},   {151, 893, 130, 8},  {391, 893, 8, 54},
            {333, 893, 58, 8},  {333, 849, 8, 49},  {240, 849, 101, 8},  {336, 761, 105, 8},
            {391, 761, 8, 83},  {271, 806, 129, 8}, {271, 761, 8, 52},   {102, 536, 150, 8},
            {44, 492, 8, 140},  {0, 625, 54, 8},    {66, 578, 99, 8},   {212, 578, 112, 8},
            {125, 578, 8, 64},  {209, 578, 8, 102}, {274, 578, 8, 54},   {167, 625, 51, 8},
            {151, 671, 68, 8},  {98, 619, 8, 106},  {44, 716, 64, 8},    {44, 716, 8, 50},
            {151, 671, 8, 99}, {102, 761, 178, 8},  {209, 716, 73, 8},   {274, 671, 8, 54},
            {274, 671, 44, 8},  {311, 671, 8, 50},  {311, 716, 42, 8},   {100, 807, 8, 94},
            {57, 893, 51, 8},   {0, 806, 220, 8},   {0, 849, 54, 8},     {56, 98, 27, 27},
            {42, 381, 27, 27},  {44, 662, 27, 27},  {221, 490, 27, 27},  {219, 946, 27, 27},
            {333, 98, 27, 27},  {256, 266, 27, 27}, {366, 804, 27, 27},  {416, 716, 27, 27},
            {579, 173, 27, 27}, {626, 89, 27, 27},  {626, 892, 27, 27},  {651, 710, 27, 27},
            {721, 221, 27, 27}, {722, 690, 27, 27}, {835, 50, 27, 27},   {854, 312, 27, 27},
            {845, 441, 27, 27}, {942, 47, 27, 27},  {960, 354, 27, 27},  {960, 870, 27, 27},
            {0, 755, 41, 8},    {0, 0, 8, 1024},    {0, 0, 1024, 8},     {1016, 0, 8, 1024},
            {0, 1016, 1024, 12}
        };

    for (const QRectF &datosMuros : posicionesMuros) {
        QGraphicsRectItem *muro = new QGraphicsRectItem(datosMuros);
        muro->setScale(0.68359375);
        muro->setBrush(Qt::NoBrush); //relleno invisible
        muro->setPen(Qt::NoPen);     //borde invisible
        //muro->setPen(QColor("#FFFFFF"));
        scene->addItem(muro);
    }
}

void MainWindow::crearBloquesCuadrado(){
    QVector<QRectF> posicionesCuadrado
        = {{482, 462, 20, 20}, {502, 462, 20, 20},{522, 462, 20, 20}, {542, 462, 20, 20},
           {482, 482, 20, 20}, {482, 502, 20, 20}, {482, 522, 20, 20}, {542, 482, 20, 20},
           {542, 502, 20, 20}, {542, 522, 20, 20}, {502, 522, 20, 20}, {522, 522, 20, 20}};

    for (const QRectF& datosCuadrado : posicionesCuadrado) {
        Obstaculo *cuadrado = new Obstaculo(datosCuadrado);
        cuadrado->setScale(0.68359375);
        cuadrado->setBrush(QColor("#053270"));
        cuadrado->setPen(QColor("#FFFFFF"));
        cuadrado->setFlag(QGraphicsItem::ItemIsMovable);
        cuadrado->setOpacity(0.5);
        scene->addItem(cuadrado);
    }
}

void MainWindow::ubicarBonificaciones(){
    QVector<QPoint> posicionesBonificaciones = {
        {47, 363}, {133, 576}, {522, 576}, {665, 385},
        {510, 59}, {337, 182}, {67, 150}, {630, 241}
    };

    //limpiar cualquier bonificación existente
    for(Bonificacion* b : bonificacionesActivas) {
        scene->removeItem(b);
        delete b;
    }
    bonificacionesActivas.clear();

    for(Bonificacion* b : bonificacionesRecolectadas) {
        delete b;
    }
    bonificacionesRecolectadas.clear();

    QPixmap plato(":/multimedia/plato.png");
    plato = plato.scaled(18, 18);

    for(QPoint &punto : posicionesBonificaciones){
        Bonificacion *bonificacion = new Bonificacion(plato);
        bonificacion->setPos(punto.x(), punto.y());
        bonificacion->iniciarAnimacionFlotacion();
        //bonificacion->setParent(this);
        bonificacionesActivas.append(bonificacion);
        scene->addItem(bonificacion);
    }
}

void MainWindow::reaparecerBonificaciones() {
    //eliminar bonificaciones recolectadas
    for(Bonificacion* bonificacion : bonificacionesRecolectadas) {
        delete bonificacion;
    }
    bonificacionesRecolectadas.clear();

    //crear nuevas bonificaciones
    ubicarBonificaciones();
}

void MainWindow::manejarBonificacionRecolectada(Bonificacion* bonificacion) {
    //eliminar de la escena
    if(!bonificacion || !bonificacionesActivas.contains(bonificacion)) {
        return;
    }

    scene->removeItem(bonificacion);

    if(goku->getCargaSuper() >= 100) {
        goku->setValidoCargarSuper(true);
    }

    actualizarBarraSuper();

    //mover a lista de recolectadas
    bonificacionesActivas.removeOne(bonificacion);
    bonificacionesRecolectadas.append(bonificacion);

    //ocultar en lugar de eliminar inmediatamente
    bonificacion->setVisible(false);
    bonificacion->detenerAnimacion();

    //verificar si se recolectaron todas
    if (bonificacionesActivas.isEmpty()) {
        QTimer::singleShot(1000, this, &MainWindow::reaparecerBonificaciones);
    }
}

void MainWindow::actualizarBarraSuper() {
    //ocultar todos los labels primero
    for(int i = 0; i < 5; i++) {
        superBarLabels[i]->setVisible(false);
    }

    int estado = 0;
    if(goku->getCargaSuper() >= 100) estado = 4;
    else if(goku->getCargaSuper() >= 75) estado = 3;
    else if(goku->getCargaSuper() >= 50) estado = 2;
    else if(goku->getCargaSuper() >= 25) estado = 1;
    else estado = 0;

    //mostrar solo el label correspondiente
    superBarLabels[estado]->setVisible(true);
}

void MainWindow::resetCargaSuperYActualizarBarra() {
    goku->setCargaSuper(0);
    actualizarBarraSuper();
}

//----------------- METODOS PARA LA CARGA DEL NIVEL DOS -----------------
void MainWindow::nivel2(){
    //CONFIGURAR LA VISTA
    resize(1000, 600);

    QWidget *lifeBarContainer = new QWidget();
    lifeBarContainer->setFixedHeight(50); //le damos altura
    lifeBarContainer->setStyleSheet("background-color: transparent;");

    QHBoxLayout *lifeBarLayout = new QHBoxLayout(lifeBarContainer);
    lifeBarLayout->setSpacing(450);
    lifeBarLayout->setContentsMargins(10, 5, 40, 5);

    QHBoxLayout *barraGokuLayout = new QHBoxLayout();
    barraGokuLayout->setSpacing(0);
    barraGokuLayout->setContentsMargins(0, 0, 0, 0);

    QHBoxLayout *barraRoshiLayout = new QHBoxLayout();
    barraRoshiLayout->setSpacing(0);
    barraRoshiLayout->setContentsMargins(0, 0, 0, 0);


    QPixmap imagenRoshi(":/multimedia/barraRoshi.png");
    for(int i = 0; i < 5; i++) {
        lifeBarRoshiLabels[i] = new QLabel(lifeBarContainer);
        lifeBarRoshiLabels[i]->setFixedSize(250, 50);

        // Obtener el sprite correspondiente
        int y = i * 100;
        QPixmap sprite = imagenRoshi.copy(0, y, 500, 100);
        QPixmap scaled = sprite.scaled(250, 50, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

        lifeBarRoshiLabels[i]->setPixmap(scaled);
        lifeBarRoshiLabels[i]->setVisible(false); //inicialmente ocultos
        barraRoshiLayout->addWidget(lifeBarRoshiLabels[i]);
    }

    QPixmap imagenGoku(":/multimedia/barraGoku.png");
    for(int i = 0; i < 5; i++) {
        lifeBarGokuLabels[i] = new QLabel(lifeBarContainer);
        lifeBarGokuLabels[i]->setFixedSize(250, 50);

        // Obtener el sprite correspondiente
        int y = i * 100;
        QPixmap sprite = imagenGoku.copy(0, y, 500, 100);
        QPixmap scaled = sprite.scaled(250, 50, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

        lifeBarGokuLabels[i]->setPixmap(scaled);
        lifeBarGokuLabels[i]->setVisible(false); //inicialmente ocultos
        barraGokuLayout->addWidget(lifeBarGokuLabels[i]);
    }

    lifeBarLayout->addStretch();
    lifeBarLayout->addLayout(barraGokuLayout);
    lifeBarLayout->addLayout(barraRoshiLayout);
    lifeBarLayout->addStretch();

    QGraphicsProxyWidget *proxyWidget = new QGraphicsProxyWidget();
    proxyWidget->setWidget(lifeBarContainer);
    scene->addItem(proxyWidget);
    proxyWidget->setPos(0, 10);

    QPixmap imagenCueva(":/multimedia/cueva.png");
    QGraphicsPixmapItem *cueva = new QGraphicsPixmapItem(imagenCueva);
    cueva->setZValue(-100);
    scene->addItem(cueva);
    scene->setSceneRect(cueva->boundingRect());
    mainLayout->addWidget(view);
    crearPlataformas();

    //AGREGAR PERSONAJES
    //agregar a Goku
    QPixmap spriteGoku(":/multimedia/goku.png");
    goku = new Jugador(spriteGoku, 36, 248, 64, 64, 50, 50);
    scene->addItem(goku);
    goku->setFocus();
    goku->setPuntoReinicio(36, 336);
    goku->setPos(goku->x, goku->y);

    connect(goku, &Jugador::vidaCambiada, this, &MainWindow::actualizarBarraVidaGoku);

    //agregar a Roshi
    QPixmap spriteRoshi(":/multimedia/roshi.png");
    roshi = new Enemigo(spriteRoshi, 848, 248, 64, 64, 50, 50);
    scene->addItem(roshi);
    roshi->setPos(roshi->x, roshi->y);
    roshi->setPlataformas(plataformasDerecha);
    roshi->setJugadorObjetivo(goku);

    //inicializar barras
    actualizarBarraVidaGoku();
    actualizarBarraVidaRoshi();
}

void MainWindow::crearPlataformas(){
    QVector<QRectF> posicionesPlataformas = {
        {0, 296, 312, 32}, {0, 384, 236, 32}, {0, 468, 400, 32},
        {768, 296, 228, 32}, {608, 388, 388, 32}, {500, 468, 496, 32}
    };

    for(auto &datosPlataforma : posicionesPlataformas){
        QGraphicsRectItem *plataforma = new QGraphicsRectItem(datosPlataforma);
        plataforma->setBrush(Qt::NoBrush);
        plataforma->setPen(Qt::NoPen);
        //plataforma->setBrush(QColor("#6d2045"));
        //plataforma->setPen(QColor("#FFFFFF"));
        //plataforma->setFlag(QGraphicsItem::ItemIsMovable);
        //plataforma->setOpacity(0.5);
        scene->addItem(plataforma);
        if (plataforma && plataforma->rect().x() > 495) {
            plataformasDerecha.append(plataforma->rect());
        }
    }
}

void MainWindow::actualizarBarraVidaGoku() {
    //ocultar todos los labels primero
    for(int i = 0; i < 5; i++) {
        lifeBarGokuLabels[i]->setVisible(false);
    }

    int estado = 0;

    if(goku->getCargaVida() >= 4) estado = 4;
    else if(goku->getCargaVida() >= 3) estado = 3;
    else if(goku->getCargaVida() >= 2) estado = 2;
    else if(goku->getCargaVida() >= 1) estado = 1;
    else estado = 0;

    //mostrar solo el label correspondiente
    lifeBarGokuLabels[estado]->setVisible(true);
}

void MainWindow::actualizarBarraVidaRoshi() {
    //ocultar todos los labels primero
    for(int i = 0; i < 5; i++) {
        lifeBarRoshiLabels[i]->setVisible(false);
    }

    int estado = 0;

    if(roshi->getCargaVida() >= 4) estado = 4;
    else if(roshi->getCargaVida() >= 3) estado = 3;
    else if(roshi->getCargaVida() >= 2) estado = 2;
    else if(roshi->getCargaVida() >= 1) estado = 1;
    else estado = 0;

    //mostrar solo el label correspondiente
    lifeBarRoshiLabels[estado]->setVisible(true);
}
