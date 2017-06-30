#ifndef CIRCLEPROGRESSBAR_H
#define CIRCLEPROGRESSBAR_H

#include <QWidget>
#include <QPaintEvent>
#include <QPainter>
#include <QTimer>

class CircleProgressBar : public QWidget
{
    Q_OBJECT
public:
    CircleProgressBar(QWidget *parent = 0);

    void startAnimate();
signals:
private slots:
    void updateAnimation();
    void updateValAnimation();

public slots:
    void setProgress(int val);

protected:
    void paintEvent(QPaintEvent *);

private:
    double progress;
    double curProgress;
    int animateProgress;
    QTimer *timer;

    void setValWithAnimation();
    double myrand(int accuracy);
};

#endif // CIRCLEPROGRESSBAR_H
