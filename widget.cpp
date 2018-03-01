#include "widget.h"
#include "ui_widget.h"
#include <QPainter>
#include "math.h"
#include "qdebug.h"
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>

int score = 0;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    _scene = new QGraphicsScene(0, 0, 800, 600, this);
    _scene->setStickyFocus(true);
    ui->graphicsView->setScene(_scene);
    _ball = new MovableBall(QPointF(475,500) );
    _score = new Score("0");
    _scene->addItem( _ball );
    _scene->addItem( new MovableRect(QPointF(400,600) ) );
    _scene->addItem( _score );

    for(int i = 150; i < 900; i+=50)
    {
        for(int j = 100; j < 350; j+=50)
        {
            _scene->addItem( new DestroyableRect(QPointF(i,j) ) );
        }
    }

    _animationTimer = new QTimer(this);
    connect(_animationTimer, SIGNAL( timeout() ), _scene, SLOT( advance() ) );
    connect(_score, SIGNAL( scoreChanged() ), this, SLOT( getScore() ) );

    _animationTimer->start(1000/60);//60 fps
}

Widget::~Widget()
{
    delete _scene;
    delete _animationTimer;
    delete ui;
}

void Widget::getScore()
{
    ui->lcdNumber->display(score);
}

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
MovableBall::MovableBall(QPointF point) : QGraphicsEllipseItem(0)//Создание шарика
{
    QColor color = QColor( Qt::darkRed );
    setBrush( color );
    setRect( 0, 0, 50, 50 );//задание размера шарика
    setPos(point-QPointF(125,50));
}

void MovableBall::advance(int phase)
{
    if(phase)
    {
        foreach(QGraphicsItem * item, collidingItems() )
        {
            if(item->type() == DestroyableRect::Type)//шарик столкнулся с кирпичем
            {
                //qDebug()<<"Brick pos="<<item->pos()<<"Ball pos="<<pos();

                if(item->pos().y() == pos().y()-50)//нижняя стенка
                {
                    //qDebug()<<"TEST111";
                    if(move == 3)
                        move = 2;
                    if(move == 0)
                        move = 1;
                    item->setData(0,true);
                }
                if(item->pos().y() == pos().y()+50)//верхняя стенка
                {
                    //qDebug()<<"TEST222";
                    if(move == 1)
                        move = 0;
                    if(move == 2)
                        move = 3;
                    item->setData(0,true);
                }
                if(item->pos().x() == pos().x()+49)//левая стенка
                {
                    //qDebug()<<"TEST333";
                    if(move == 0)
                        move = 3;
                    if(move == 1)
                        move = 2;
                    item->setData(0,true);
                }
                if(item->pos().x() == pos().x()-49)//правая стенка
                {
                    //qDebug()<<"TEST444";
                    if(move == 2)
                        move = 1;
                    if(move == 3)
                        move = 0;
                    item->setData(0,true);
                }
                if(item->pos().y() != pos().y()-50 && item->pos().y() != pos().y()+50 && item->pos().x() != pos().x()-49 && item->pos().x() != pos().x()+49)//угол кирпича
                {
                    //qDebug()<<"TEST555";
                    if(move == 0)
                    {
                        //qDebug()<<"TEST1";
                        move = 2;
                    }
                    else if(move == 1)
                    {
                        //qDebug()<<"TEST2";
                        move = 3;
                    }
                    else if(move == 2)
                    {
                        //qDebug()<<"TEST3";
                        move = 0;
                    }
                    else if(move == 3)
                    {
                        //qDebug()<<"TEST4";
                        move = 1;
                    }
                    item->setData(0,true);
                }
                if(item->pos() == QPointF(575,200))
                {
                    scene()->addItem(new Bonus(mapToScene(QPointF( 25, 0 ) ), 0 ));
                }
            }
            else if(item->type() == MovableRect::Type)//шарик столкнулся с ракеткой
            {
                if(move == 2)
                    move = 3;
                if(move == 1)
                    move = 0;
            }
        }
        if(pos().y()==-50)//шарик столкнулся с потолком
        {
            if(move == 3)
                move = 2;
            if(move == 0)
                move = 1;
        }
        if(pos().x()==800)//шарик столкнулся с правой стенкой
        {
            if(move == 0)
                move = 3;
            if(move == 1)
                move = 2;
        }
        if(pos().x()==-50)//шарик столкнулся с левой стенкой
        {
            if(move == 3)
                move = 0;
            if(move == 2)
                move = 1;
        }
        if(pos().y()==600)//шарик столкнулся с дном
        {
            foreach(QGraphicsItem * item, scene()->items() )
            {
                item->setData(0,true);
            }
            scene()->addText("Game Over")->setScale(4);
            delete this;
        }
        if(move == 0)
            moveBy(2,-2);
        if(move == 1)
            moveBy(2,2);
        if(move == 2)
            moveBy(-2,2);
        if(move == 3)
            moveBy(-2,-2);
    }
}

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
MovableRect::MovableRect(QPointF point) : QGraphicsRectItem(0)
{
    QColor color = QColor( rand()%255, rand()%255, rand()%255 );
    setBrush( color );
    setRect( 0, 0, 200, 50 );
    setPos(point-QPointF(125,100));
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();
}

void MovableRect::advance(int phase)
{
    if(phase)
    {
        if(pos().x() > 650)//правый ограничитель ракетки
            setPos( 650, pos().y() );
        if(pos().x() < -50)//левый ограничитель ракетки
            setPos( -50, pos().y() );
        moveBy( xMove, 0 );//перемещение ракетки по оси х
        if(data(0).toBool())//удаление ракетки
            delete this;
    }
}

void MovableRect::keyPressEvent(QKeyEvent *event)//событие нажатия на кнопку
{
    switch (event->key())
    {
    case Qt::Key_Left:
        xMove = -3;
        break;
    case Qt::Key_Right:
        xMove = 3;
        break;
    case Qt::Key_Space:
        if(data(1).toString() == "laser")
        {
            scene()->addItem(new Laser(mapToScene(QPointF( rect().width()/2, 0 ) ), 0 ));
        }
    default:
        break;
    }
}

void MovableRect::keyReleaseEvent(QKeyEvent *event)//событие отпускания кнопки
{
    switch (event->key())
    {
    case Qt::Key_Left:
        xMove = 0;
        break;
    case Qt::Key_Right:
        xMove = 0;
        break;
    default:
        break;
    }
}

int MovableRect::type() const
{
    return Type;
}

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
DestroyableRect::DestroyableRect(QPointF point) : QGraphicsRectItem(0)//построение кирпича
{
    QColor color;
    if(point.y() == 100)
        color = QColor( Qt::red );
    else if(point.y() == 150)
        color = QColor( Qt::green );
    else if(point.y() == 200)
        color = QColor( Qt::blue );
    else if(point.y() == 250)
        color = QColor( Qt::darkCyan );
    else if(point.y() == 300)
        color = QColor( Qt::darkYellow );
    setBrush( color );
    setRect( 0, 0, 50, 50 );
    setPos(point-QPointF(125,100));
}

int DestroyableRect::type() const
{
    return Type;
}

void DestroyableRect::advance(int phase)//удаление кирпича и запись очков
{
    if(phase)
    {
        if(data(0).toBool())
        {
            score+=10;
            foreach(QGraphicsItem* item, scene()->items() )
            {
                if(item->type() == Score::Type)
                {
                    item->setData(2,true);
                }
            }
            delete this;
        }
    }
}

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
Laser::Laser(QPointF pos, QGraphicsItem *parent): QGraphicsLineItem(parent)//создание лазера
{
    yspeed = 4;
    setPen( QPen( QBrush( Qt::red ), 3) );
    setLine( 0, 0, 0, -10 );
    setPos( pos );
}

void Laser::advance(int phase)//движение, столкновение, удаление лазера
{
    if(!phase)
    {
        foreach(QGraphicsItem* item, collidingItems() )
        {
            if(item->type() == DestroyableRect::Type)
            {
                item->setData(0, true);
                setData(0, true);
            }
        }
    }
    else
    {
        moveBy(0, -yspeed);
        if(data(0).toBool())
            delete this;
    }
}

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
Bonus::Bonus(QPointF pos, QGraphicsItem *parent): QGraphicsEllipseItem(parent)//создание бонуса
{
    yspeed = 2;
    QColor color = QColor( Qt::red );
    setBrush( color );
    setRect( 0, 0, 50, 25 );//задание размера бонуса
    setPos( pos );
}

void Bonus::advance(int phase)//движение, столкновение, удаление бонуса
{
    if(!phase)
    {
        foreach(QGraphicsItem* item, collidingItems() )
        {
            if(item->type() == MovableRect::Type)
            {
                item->setData( 1, "laser" );
                setData(0, true);
            }
        }
    }
    else
    {
        moveBy(0, yspeed);
        if(data(0).toBool())
            delete this;
    }
}


Score::Score(QString text):QGraphicsTextItem(0)//класс для передачи очков
{
    setHtml(text);
    setPos(850,-50);
}

int Score::type() const
{
    return Type;
}

void Score::advance(int phase)
{
    if(phase)
    {
        if(data(2).toBool())
        {
            emit scoreChanged();
            setData(2,false);
        }
    }
}
