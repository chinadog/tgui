#include "circleprogressbar.h"
#include <QDebug>
#include <QTime>
#include <qmath.h>

CircleProgressBar::CircleProgressBar(QWidget* parent) : QWidget(parent)
{
    animateProgress = 0;
    curProgress = 0.0;
    setFixedSize(100,110);
    startAnimate();

    QTime midnight(0,0,0);
    qsrand(midnight.secsTo(QTime::currentTime()));

}

void CircleProgressBar::startAnimate()
{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateAnimation()));
    timer->start(10);
}

void CircleProgressBar::updateAnimation()
{
    animateProgress++;
    update();
    if(animateProgress == 100)
    {
        timer->stop();
        disconnect(timer,SIGNAL(timeout()), this, SLOT(updateAnimation()));

        setValWithAnimation();
    }
}

void CircleProgressBar::updateValAnimation()
{

    if(fabs(curProgress - progress) <= 0.001 )
    {
        timer->stop();
        disconnect(timer,SIGNAL(timeout()), this, SLOT(updateValAnimation()));
        return;
    }

    if(curProgress < progress)
    {
        curProgress += 0.01;
    }
    else if(curProgress > progress)
    {
        curProgress -= 0.01;
    }
    update();

}

void CircleProgressBar::setProgress(int val)
{
    progress = (double)val/100;

    //yes, it is not very good, the best approach is to
    //create something similar to QProgressBar
    if(animateProgress == 100 && !timer->isActive())
    {
//        this->update();
        setValWithAnimation();
    }
}


void CircleProgressBar::paintEvent(QPaintEvent*)
{
    QPainter p(this);

    QPen pen;
    pen.setWidth(6);
    pen.setColor(QColor("#3275AE")); //#355A88

    QPen penBack;
    penBack.setWidth(6);
    penBack.setColor(QColor("#454545"));

    p.setPen(pen);

    p.setRenderHint(QPainter::Antialiasing);

    QRectF rectangle(10.0, 20.0, 80.0, 80.0);
    //to understand these magic numbers, look drawArc method in Qt doc
    int startAngle = -90 * 16;
    int spanAngle = curProgress * 360 * 16;

    p.setPen( penBack );
    p.drawArc(rectangle, startAngle, -(double)animateProgress/100 * 360 * 16);
    p.setPen( pen );
    if(animateProgress == 100)
    {
        p.drawArc(rectangle, startAngle, -spanAngle);
    }

    p.drawText(rectangle,Qt::AlignCenter,QString::number(curProgress*100)+" %");
}


void CircleProgressBar::setValWithAnimation()
{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateValAnimation()));
    timer->start(10);
}




