#ifndef SYSTEMMONITOR_H
#define SYSTEMMONITOR_H

#include <QObject>
#include <QTimer>

struct CpuStruct
{
    qlonglong usr;
    qlonglong nice;
    qlonglong sys;
    qlonglong idle;
    qlonglong iow;
    qlonglong irq;
    qlonglong softirq;
};

class SystemMonitor : public QObject
{
    Q_OBJECT
public:
    explicit SystemMonitor(QObject *parent = 0);
private:
    QTimer* m_timer;
    CpuStruct cpuLast;
    CpuStruct cpuNow;
    qlonglong cpuLastSum;

    int getMem(int pid);
    int getCPU(int pid);
    int getCPU();
    int getMem();
    qlonglong getTotalSystemMemory();
    void initFunctional();
    void initConnections();
signals:
    void changeCpu( int val );
    void changeMem( int val );
private slots:
    void checkProcesses();
};

#endif // SYSTEMMONITOR_H
