#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QTimer>

namespace Ui {
class Widget;
}

class Score : public QGraphicsTextItem
{
    Q_OBJECT
public:
    enum
    {
        Type = UserType + 3
    };
    Score(QString text);

    // QGraphicsItem interface
public:
    virtual int type() const;
    virtual void advance(int phase);
signals:
    void scoreChanged();
};

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
class Bonus : public QGraphicsEllipseItem
{
public:
    Bonus(QPointF pos, QGraphicsItem* parent);
private:
    int yspeed;

    // QGraphicsItem interface
public:
    virtual void advance(int phase);
};

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
class Laser : public QGraphicsLineItem
{
public:
    Laser(QPointF pos, QGraphicsItem* parent);
private:
    int yspeed;

    // QGraphicsItem interface
public:
    virtual void advance(int phase);
};

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
class MovableBall : public QObject, public QGraphicsEllipseItem//Шарик
{
    Q_OBJECT
public:
    MovableBall(QPointF point);
private:
    int move = 0;
    // QGraphicsItem interface
public:
    void advance(int phase);
};

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
class MovableRect : public QGraphicsRectItem//Платформа
{
public:
    enum
    {
        Type = UserType + 2
    };
    MovableRect(QPointF point);

protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
private:
    int xMove = 0;

    // QGraphicsItem interface
public:
    int type() const;
    void advance(int phase);
};

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
class DestroyableRect : public QObject, public QGraphicsRectItem//Кирпичи
{
    Q_OBJECT
public:
    enum
    {
        Type = UserType + 1
    };
    DestroyableRect(QPointF point);


    // QGraphicsItem interface
public:
    int type() const;
    void advance(int phase);
};

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

public slots:
    void getScore();
private:
    Ui::Widget *ui;
    QGraphicsScene* _scene;
    QGraphicsItem* _item;
    MovableBall* _ball;
    Score* _score;
    QTimer* _animationTimer;
    QList<QPointF> _itemPos;
};

#endif // WIDGET_H
