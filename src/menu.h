#ifndef MENU_H
#define MENU_H

#include <QWidget>
#include <QBoxLayout>
#include <QPushButton>
#include <QGraphicsEffect>
#include <QPainter>
#include <QLabel>
#include <QTimer>
#include <QDateTime>

class Menu : public QWidget
{
    Q_OBJECT
public:
    Menu(QWidget* parent = 0);
private:
    QLabel* m_timeLabel;

    void initInterface();
    void paintEvent(QPaintEvent* e);
private slots:
    void updateTime();
};

#endif // MENU_H
