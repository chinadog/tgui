#include "tgui.h"
#include <QSplitter>
#include <QGraphicsDropShadowEffect>
#include <QBoxLayout>
#include <Common/Resources>
#include <TDB/TDB>
#include <TLogger/TLogger>
#include <QThread>
#include "syslog.h"

//--------------------------------------------------------------------------------------
//                                     Конструкторы
//--------------------------------------------------------------------------------------

TGUI::TGUI(const QString& configPath, QWidget *parent)
    : QWidget(parent)
{
    // Загрузка конфигурации
    m_config.load(configPath);
    initInterface();
    initStyle();
    initConnections();
    initFunctional();
    getData();
}

TGUI::~TGUI()
{
    delete m_tdb;
}

//--------------------------------------------------------------------------------------
//                                     Методы
//--------------------------------------------------------------------------------------

void TGUI::initInterface()
{
    // Инициализация
    m_systemMonitor = new SystemMonitor(this);
    m_tdb = new TDB();
    QVBoxLayout* mainLay = new QVBoxLayout;
    QHBoxLayout* leftLay = new QHBoxLayout;
    QVBoxLayout* circleLay = new QVBoxLayout;
    Menu* menu = new Menu(this);
    QSplitter *splitter = new QSplitter(this);
    QWidget* leftWidget = new QWidget(this);
    QGraphicsDropShadowEffect *bodyShadow = new QGraphicsDropShadowEffect;
    logListWidget = new LogListWidget(this);
    m_button = new QPushButton("OK",this);

    m_circleCpu = new CircleProgressBar(this);
    m_circleCpu->setProgress(41);

    m_circleMem = new CircleProgressBar(this);
    m_circleMem->setProgress(80);

    QLabel* labelCpu = new QLabel("CPU",this);
    labelCpu->setAlignment(Qt::AlignHCenter);
    QLabel* labelMem = new QLabel("MEM",this);
    labelMem->setAlignment(Qt::AlignHCenter);


    // Установка на слой
    circleLay->addWidget(labelCpu);
    circleLay->addWidget(m_circleCpu);
    circleLay->addSpacing(50);
    circleLay->addWidget(labelMem);
    circleLay->addWidget(m_circleMem);
    circleLay->addStretch(1);
    circleLay->setSpacing(0);
    leftLay->addWidget(m_button);
    leftLay->addStretch(1);
    leftLay->addLayout(circleLay);
    leftWidget->setLayout(leftLay);
    splitter->addWidget(leftWidget);
    splitter->addWidget(logListWidget);
    splitter->setStretchFactor(0,4);
    splitter->setStretchFactor(1,1);
    mainLay->addWidget(menu);
    mainLay->addWidget(splitter);
    mainLay->setContentsMargins(0,0,0,0);
    mainLay->setSpacing(0);
    setLayout(mainLay);

    // Кастомизация
    splitter->setHandleWidth(0);
    logListWidget->setSelectionMode(QAbstractItemView::NoSelection);
    logListWidget->setWordWrap(true);
    logListWidget->setGraphicsEffect(bodyShadow);
//    logListWidget->setTextElideMode(Qt::ElideNone); // возможно нужно для переноса слов
//    logListWidget->setResizeMode(QListView::Adjust);
    bodyShadow->setBlurRadius(9.0);
    bodyShadow->setColor(QColor(0, 0, 0,40));
    bodyShadow->setXOffset(-2.0);
    setMinimumSize(700,500);
    resize(900,500);
}

void TGUI::initStyle()
{
    setStyleSheet(Resources::style("dark"));
}

void TGUI::initConnections()
{
//    connect(m_sql,SIGNAL(finishedExec(int)),this,SLOT(checkResult(int)));
    connect(m_button,SIGNAL(clicked(bool)),this,SLOT(emitLog()));
//    connect(m_button,SIGNAL(clicked(bool)),this,SLOT(getData()));
    connect(m_tdb,SIGNAL(eventUpdated(int)),this,SLOT(updateEvent(int)));
    connect(m_systemMonitor,SIGNAL(changeCpu(int)),m_circleCpu,SLOT(setProgress(int)));
    connect(m_systemMonitor,SIGNAL(changeMem(int)),m_circleMem,SLOT(setProgress(int)));

}

void TGUI::setStartValues()
{
    QString connectionName = "syslog";

    m_tdb->setConnectionName(connectionName,
                             m_config.db.databaseName(),
                             m_config.db.hostName(),
                             m_config.db.userName(),
                             m_config.db.password(),
                             m_config.db.port()
                             );
    m_tdb->subscribeToNotificationEvents();
}

void TGUI::getData()
{
    qDebug() << "[GUI] Start get data";

    ResultAsync<QList<Event> > *res = m_tdb->lastMessageAsync(10);
    connect(res,SIGNAL(ready(QList<Event>)),this,SLOT(addLog(QList<Event>)));


//    QList<Event> events = m_tdb->lastMessage(100);
//    addLog(events);
}

void TGUI::addLog(const QList<Event>& events)
{
    for(int i=0;i<events.size();i++)
    {
        logListWidget->addLog(events[i].time, QString::number(events[i].id) + events[i].msg);
    }
}

void TGUI::addLog(const Event &event)
{
    qDebug() << "redy = " << event.id;
    logListWidget->addLog(event.time, QString::number(event.id) + event.msg);
}

void TGUI::emitLog()
{
    openlog("myapp", LOG_PID|LOG_CONS, LOG_USER);
    QString qtStrData = "abc"+ QString::number( qrand()%100 );
    syslog(LOG_INFO, "%s", qtStrData.toStdString().c_str() );
    closelog();
}

void TGUI::updateEvent(int id)
{
    qDebug() << "update event = " << id;
    ResultAsync<Event> *res = m_tdb->messageAsync(id);
    connect(res,SIGNAL(ready(Event)),this,SLOT(addLog(Event)));

//    Event event = m_tdb->message(id);
//    addLog(event);
}



void TGUI::initFunctional()
{
    setStartValues();
}


