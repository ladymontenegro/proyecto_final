#include "mainwindow.h"
#include <QGraphicsItem>
#include <QVector>
#include "QGraphicsPixmapItem"
#include "bonificacion.h"
#include "jugador.h"
#include "obstaculo.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    resize(700, 700);
    scene = new QGraphicsScene(this);
    view = new QGraphicsView(scene, this);
    view->setFixedSize(700, 700);
    view->setScene(scene);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    nivel1();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::crearMurosLaberinto()
{
    QVector<QRectF> posicionesMuros
        = {{44, 0, 8, 277},    {44, 266, 123, 11}, {44, 86, 80, 11},    {98, 0, 8, 55},
           {150, 0, 8, 143},   {98, 132, 71, 11},  {98, 132, 8, 8},     {44, 179, 68, 11},
           {0, 312, 58, 11},   {98, 312, 65, 11},  {151, 266, 8, 50},   {98, 310, 8, 145},
           {151, 179, 8, 55},  {209, 45, 8, 130},  {151, 179, 267, 11}, {209, 86, 98, 11},
           {209, 45, 32, 8},   {333, 0, 8, 190},   {333, 45, 89, 11},   {391, 86, 8, 59},
           {448, 86, 8, 59},   {391, 128, 59, 8},  {209, 220, 8, 144},  {209, 220, 44, 8},
           {151, 353, 66, 8},  {151, 353, 8, 55},  {98, 397, 67, 8},    {0, 445, 112, 8},
           {98, 131, 8, 58},   {271, 445, 8, 54},  {209, 445, 76, 8},   {209, 397, 8, 59},
           {274, 310, 8, 98},  {274, 397, 56, 8},  {316, 397, 8, 186},  {287, 266, 171, 8},
           {312, 220, 150, 8}, {448, 220, 8, 57},  {391, 266, 8, 55},   {333, 310, 72, 8},
           {333, 310, 8, 38},  {465, 0, 8, 56},    {465, 45, 69, 8},    {482, 310, 72, 8},
           {493, 86, 8, 148},  {493, 86, 98, 8},   {493, 220, 182, 8},  {540, 266, 66, 8},
           {540, 220, 8, 55},  {44, 488, 244, 8},  {540, 128, 8, 41},   {540, 128, 114, 8},
           {649, 128, 8, 62},  {649, 179, 263, 8}, {898, 86, 8, 103},   {783, 0, 8, 100},
           {578, 45, 8, 55},   {578, 45, 219, 8},  {707, 45, 8, 97},    {707, 128, 145, 8},
           {783, 179, 8, 55},  {898, 45, 76, 8},   {151, 843, 8, 178},  {943, 179, 78, 8},
           {960, 310, 61, 8},  {960, 220, 8, 104}, {842, 220, 132, 8},  {898, 266, 8, 217},
           {810, 397, 209, 8}, {810, 397, 8, 105}, {763, 266, 163, 8},  {810, 266, 8, 58},
           {707, 310, 117, 8}, {707, 220, 8, 60},  {649, 266, 72, 8},   {649, 266, 8, 98},
           {649, 353, 72, 8},  {707, 353, 8, 103}, {707, 445, 72, 8},   {761, 397, 34, 8},
           {785, 353, 8, 55},  {785, 353, 44, 8},  {960, 488, 61, 8},   {960, 488, 8, 54},
           {759, 531, 198, 8}, {759, 488, 8, 54},  {707, 488, 57, 8},   {707, 488, 8, 95},
           {898, 572, 59, 8},  {960, 619, 61, 8},  {873, 619, 39, 8},   {873, 619, 8, 239},
           {873, 843, 44, 8},  {911, 843, 8, 54},  {810, 531, 8, 200},  {917, 663, 57, 8},
           {917, 663, 8, 58},  {917, 710, 57, 8},  {960, 710, 8, 42},   {960, 800, 61, 8},
           {759, 572, 8, 88},  {810, 755, 75, 8},  {960, 887, 61, 8},   {960, 868, 8, 79},
           {919, 936, 38, 8},  {707, 710, 114, 8}, {707, 619, 8, 106},  {669, 619, 52, 8},
           {675, 755, 8, 192}, {810, 936, 8, 85},  {810, 936, 63, 8},   {810, 800, 8, 101},
           {759, 800, 53, 8},  {759, 755, 8, 59},  {759, 887, 65, 8},   {759, 887, 8, 60},
           {729, 936, 42, 8},  {557, 755, 217, 8}, {674, 843, 59, 8},   {575, 936, 111, 8},
           {575, 663, 92, 8},  {608, 663, 8, 103}, {608, 800, 8, 54},   {557, 755, 8, 42},
           {540, 843, 73, 8},  {540, 843, 8, 55},  {493, 887, 61, 8},   {493, 887, 8, 134},
           {364, 663, 143, 8}, {274, 619, 87, 8},  {391, 663, 8, 58},   {391, 710, 56, 8},
           {493, 663, 8, 148}, {432, 800, 75, 8},  {432, 800, 8, 98},   {239, 936, 207, 8},
           {0, 936, 165, 8},   {960, 45, 8, 80},   {151, 887, 137, 8},  {391, 887, 8, 60},
           {333, 887, 72, 8},  {333, 843, 8, 55},  {232, 843, 115, 8},  {333, 755, 108, 8},
           {391, 755, 8, 85},  {274, 800, 131, 8}, {274, 755, 8, 56},   {98, 531, 158, 8},
           {44, 488, 8, 138},  {0, 619, 58, 8},    {64, 572, 101, 8},   {209, 579, 118, 8},
           {125, 572, 8, 70},  {209, 572, 8, 102}, {274, 572, 8, 54},   {161, 619, 62, 8},
           {151, 663, 72, 8},  {98, 619, 8, 106},  {44, 710, 68, 8},    {44, 710, 8, 56},
           {151, 663, 8, 103}, {98, 755, 190, 8},  {209, 710, 79, 8},   {274, 663, 8, 58},
           {274, 663, 44, 8},  {311, 663, 8, 58},  {311, 710, 42, 8},   {98, 800, 8, 98},
           {61, 887, 51, 8},   {0, 800, 220, 8},   {0, 843, 58, 8},     {56, 98, 27, 27},
           {42, 381, 27, 27},  {44, 662, 27, 27},  {221, 490, 27, 27},  {219, 946, 27, 27},
           {333, 98, 27, 27},  {256, 266, 27, 27}, {366, 804, 27, 27},  {416, 716, 27, 27},
           {579, 173, 27, 27}, {626, 89, 27, 27},  {626, 892, 27, 27},  {651, 710, 27, 27},
           {721, 221, 27, 27}, {722, 690, 27, 27}, {835, 50, 27, 27},   {854, 312, 27, 27},
           {845, 441, 27, 27}, {942, 47, 27, 27},  {960, 354, 27, 27},  {960, 870, 27, 27},
           {0, 755, 41, 8},    {0, 0, 8, 1024},    {0, 0, 1024, 8},     {1016, 0, 8, 1024},
           {0, 1016, 1024, 12}};

    QVector<QRectF> posicionesCuadrado
        = {{482, 462, 20, 20}, {502, 462, 20, 20},{522, 462, 20, 20}, {542, 462, 20, 20},
           {482, 482, 20, 20}, {482, 502, 20, 20}, {482, 522, 20, 20}, {542, 482, 20, 20},
           {542, 502, 20, 20}, {542, 522, 20, 20}, {502, 522, 20, 20}, {522, 522, 20, 20}};

    for (const QRectF &datosMuros : posicionesMuros) {
        QGraphicsRectItem *muro = new QGraphicsRectItem(datosMuros);
        muro->setScale(0.683);
        muro->setBrush(Qt::NoBrush); //relleno invisible
        muro->setPen(Qt::NoPen);     //borde invisible
        scene->addItem(muro);
    }

    for (const QRectF &datosCuadrado : posicionesCuadrado) {
        Obstaculo *cuadrado = new Obstaculo(datosCuadrado);
        cuadrado->setScale(0.683);
        cuadrado->setBrush(QColor("#053270"));
        cuadrado->setPen(QColor("#FFFFFF"));
        cuadrado->setFlag(QGraphicsItem::ItemIsMovable);
        cuadrado->setOpacity(0.5);
        scene->addItem(cuadrado);
    }
}

void MainWindow::ubicarBonificaciones(){
    QVector<QPoint> posicionesBonificaciones = {
        {100, 200}, {300, 400}, {500, 600}, {430, 430},
        {100, 200}, {300, 400}, {500, 600}, {350, 350}
};
    QPixmap plato(":/multimedia/plato.png");
    plato = plato.scaled(20, 20);

    for(QPoint &punto : posicionesBonificaciones){
        Bonificacion *bonificacion = new Bonificacion(plato);
        bonificacion->setPos(punto.x(), punto.y());
        scene->addItem(bonificacion);
    }
}

void MainWindow::nivel1()
{
    //crear el laberinto
    QPixmap imagenLaberinto(":/multimedia/laberinto.png");
    QPixmap imagenEscaladaLaberinto = imagenLaberinto.scaled(700, 700);
    QGraphicsPixmapItem *laberinto = new QGraphicsPixmapItem(imagenEscaladaLaberinto);
    laberinto->setZValue(-100);
    scene->addItem(laberinto);
    scene->setSceneRect(laberinto->boundingRect());
    crearMurosLaberinto();
    ubicarBonificaciones();

    //agregar al jugador
    QPixmap spriteGoku(":/multimedia/goku.png");
    Jugador *goku = new Jugador(spriteGoku);
    scene->addItem(goku);
    goku->setFocus();
    goku->setPos(267, 271);
}

