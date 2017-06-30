#ifndef LOGLISTWIDGET_H
#define LOGLISTWIDGET_H

#include <QListWidget>

class LogListWidget : public QListWidget
{
    Q_OBJECT
public:
    LogListWidget(QWidget* parent = 0);
    QString logPath() const;
    void setLogPath(const QString &logPath);

    bool writeToFile() const;
    void setWriteToFile(bool writeToFile);

private:
    void writeLog(const QDateTime& time, const QString& log);

    QString m_logPath;
    bool m_writeToFile;
public slots:
    void addLog(const QDateTime& date, const QString& log);
    void addNewProcStatus(QString name, bool status);
    void excessLimitMem(QString name);
    void excessLimitCpu(QString name);
    void setAutostartToggle(QString name, bool val);
};

#endif // LOGLISTWIDGET_H
