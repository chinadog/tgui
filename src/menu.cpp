#include "menu.h"

Menu::Menu(QWidget* parent) :
    QWidget(parent)
{
    initInterface();
}

void Menu::initInterface()
{
    QWidget *w = new QWidget(this);
    QHBoxLayout* mainLay = new QHBoxLayout;
    mainLay->addWidget(w);
    mainLay->setSpacing(0);
    mainLay->setContentsMargins(0,0,0,0);

    m_timeLabel = new QLabel("TEXT",this);
    m_timeLabel->setAlignment( Qt::AlignRight );
    QHBoxLayout* lay = new QHBoxLayout;
    lay->addStretch(1);
    lay->addWidget( m_timeLabel );
    w->setLayout(lay);

    setLayout(mainLay);
    setFixedHeight(50);

    updateTime();

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateTime()));
    timer->start(1000);
}

void Menu::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    QPen pen;
    pen.setColor(QColor("#979797"));
    painter.setPen(pen);
//    painter.drawRoundedRect(0,5,width(), height(),3,3);
    painter.drawLine(0,height()-2,width(),height()-2);

    QWidget::paintEvent(e);
}

void Menu::updateTime()
{
    m_timeLabel->setText( QDateTime::currentDateTime().toString("hh:mm:ss\r\ndd.MM.yyyy") );
}

