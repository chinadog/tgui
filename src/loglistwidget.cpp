#include "loglistwidget.h"
#include <QDateTime>
#include <QFile>
#include <QDebug>

LogListWidget::LogListWidget(QWidget* parent) :
    QListWidget(parent)
{
    setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    setLogPath( "logPath" );
    setWriteToFile( true );
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
}

void LogListWidget::writeLog(const QDateTime& time, const QString &log)
{
//    QListWidgetItem* sep = new QListWidgetItem;
//    sep->setSizeHint(QSize(50,1));
//    sep->setFlags(Qt::ItemFlag::NoItemFlags);
//    sep->setBackgroundColor(QColor("#ececec"));

//    qDebug() << logPath();

    QString listMsg = time.toString("dd.MM.yyyy hh:mm:ss")+"\r\n"+log;
    QString logMsg = time.toString("[dd.MM.yyyy hh:mm:ss]")+" "+log+"\r\n";

//    addItem( listMsg );
//    insertItem( 0,sep );
    insertItem( 0,listMsg );

//    scrollToBottom();

    if(writeToFile() == true)
    {
        QFile file( logPath() );
        if (file.open(QIODevice::Append))
        {
           file.write(logMsg.toLocal8Bit().constData());
        }
        file.close();
    }
}

void LogListWidget::addLog(const QDateTime& date, const QString &log)
{
    writeLog(date,log);
}

bool LogListWidget::writeToFile() const
{
    return m_writeToFile;
}

void LogListWidget::setWriteToFile(bool writeToFile)
{
    m_writeToFile = writeToFile;
}

QString LogListWidget::logPath() const
{
    return m_logPath;
}

void LogListWidget::setLogPath(const QString &logPath)
{
    m_logPath = logPath;
    if(logPath.isEmpty())
    {
        m_logPath = "tmcs.log";
    }
}

void LogListWidget::addNewProcStatus(QString name, bool status)
{
    QString msg = "Процесс "+name+" остановился";
    if(status == true)
    {
        msg = "Процесс "+name+" запустился";
    }

    writeLog(QDateTime::currentDateTime(),msg);
}

void LogListWidget::excessLimitMem(QString name)
{
    QString msg = "Процесс "+name+" превысил установленный предел загрузки памяти";
    writeLog(QDateTime::currentDateTime(),msg);
}

void LogListWidget::excessLimitCpu(QString name)
{
    QString msg = "Процесс "+name+" превысил установленный предел загрузки процессора";
    writeLog(QDateTime::currentDateTime(),msg);
}

void LogListWidget::setAutostartToggle(QString name, bool status)
{
    QString msg = "Процесс "+name+" превысил число остановок за один сеанс. Функция AUTOSTART отключена.";
    if(status == true)
    {
        msg = "AUTOSTART для процесса "+name+" включена";
    }
    writeLog(QDateTime::currentDateTime(),msg);
}
