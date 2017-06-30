#include "systemmonitor.h"
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <QDebug>

SystemMonitor::SystemMonitor(QObject *parent) : QObject(parent)
{
    initFunctional();
    initConnections();

    cpuLastSum = 0;
    cpuLast.idle = 0;
}

void SystemMonitor::initFunctional()
{
    m_timer = new QTimer(this);
    m_timer->start(1000);
}

void SystemMonitor::checkProcesses()
{
    int cpuUsage = getCPU();
    emit changeCpu( cpuUsage );

    int memUsage = getMem();
    emit changeMem( memUsage );
}

void SystemMonitor::initConnections()
{
    connect(m_timer, SIGNAL(timeout()), this, SLOT(checkProcesses()));
}

int SystemMonitor::getMem(int pid)
{
    int mem = -1;

    std::string pidString ;
    std::ostringstream convert;   // stream used for the conversion
    convert << pid;
    pidString = convert.str();
    std::string cmdPath = std::string("/proc/") + pidString + "/status";
    std::ifstream cmdFile(cmdPath.c_str());
    std::string cmdLine;
    while ( std::getline(cmdFile, cmdLine) )
    {
        std::size_t found = cmdLine.find("VmRSS");
        if (!cmdLine.empty() && found!=std::string::npos)
        {
            // Keep first cmdline item which contains the program path
//            qDebug() << QString::fromStdString( cmdLine );
            QString vmSize = QString::fromStdString( cmdLine );
            vmSize = vmSize.right( vmSize.size()-2 - vmSize.indexOf(" ") );
            vmSize = vmSize.left( vmSize.indexOf(" ") ) ;
            mem = vmSize.toInt();
        }
    }
    return mem;
}

int SystemMonitor::getCPU(int pid)
{
    // VARS
    std::string cmdPath;
    std::ifstream cmdFile;
    std::string cmdLine;
    std::string pidString ;




    //PID
    std::ostringstream convert;   // stream used for the conversion
    convert << pid;
    pidString = convert.str();

    // UPTIME
    qlonglong uptime = 0;
    cmdPath = "/proc/uptime";
    cmdFile.open( cmdPath.c_str() );
    while ( std::getline(cmdFile, cmdLine) )
    {
        QString secStr = QString::fromStdString( cmdLine );
        secStr = secStr.left( secStr.indexOf(".") );
        uptime = secStr.toLongLong();
    }
    cmdFile.close();

    // UTIME STIME CUTIME CSTIME STARTTIME
    qlonglong utime = 0;
    qlonglong stime = 0;
    qlonglong cutime = 0;
    qlonglong cstime = 0;
    qlonglong starttime = 0;

    cmdPath = std::string("/proc/") + pidString + "/stat";

    cmdFile.open( cmdPath.c_str() );
    while ( std::getline(cmdFile, cmdLine) )
    {
        QString str = QString::fromStdString( cmdLine );
        // REGEXP
        QRegExp rx("(\\S+)");
        QList<QString> list;
        int pos = 0;
        while ((pos = rx.indexIn(str, pos)) != -1)
        {
            list << rx.cap(1);
            pos += rx.matchedLength();
        }
        utime = list[13].toLongLong();
        stime = list[14].toLongLong();
        cutime = list[15].toLongLong();
        cstime = list[16].toLongLong();
        starttime = list[21].toLongLong();

    }
    cmdFile.close();

    //HERTZ
    qlonglong hertz = sysconf(_SC_CLK_TCK);

    //CALCULATION
    int totalTime = utime + stime;
    totalTime = totalTime + cutime + cstime;
    int seconds = uptime - (starttime / hertz);
    int cpuUsage = 100 * ((totalTime*1.0 / hertz) / seconds);


    return cpuUsage;
}

int SystemMonitor::getCPU()
{
    qlonglong cpuUsage = 0;

    std::ostringstream convert;

    bool light = false;

    if(light)
    {
        std::string pidString ;
           // stream used for the conversion
        pidString = convert.str();
        std::string cmdPath = std::string("/proc/loadavg");
        std::ifstream cmdFile(cmdPath.c_str());
        std::string cmdLine;
        while ( std::getline(cmdFile, cmdLine) )
        {
            if (!cmdLine.empty() )
            {
                // Keep first cmdline item which contains the program path
                QString vmSize = QString::fromStdString( cmdLine );
                vmSize = vmSize.left( vmSize.indexOf(" ") ) ;
                cpuUsage = vmSize.toFloat()*100;
                qDebug() <<   vmSize.toFloat();
                break;
            }
        }
    }
    else
    {
        std::string cmdPath = std::string("/proc/stat");
        std::ifstream cmdFile(cmdPath.c_str());
        std::string cmdLine;

        while ( std::getline(cmdFile, cmdLine) )
        {
            std::size_t found = cmdLine.find("cpu");
            if (!cmdLine.empty() && found!=std::string::npos)
            {
                // Keep first cmdline item which contains the program path
                QString str = QString::fromStdString( cmdLine );
                QRegExp rx("(\\S+)");
                QList<QString> list;
                int pos = 0;
                while ((pos = rx.indexIn(str, pos)) != -1)
                {
                    list << rx.cap(1);
                    pos += rx.matchedLength();
                }
                cpuNow.usr = list[1].toLongLong();
                cpuNow.nice = list[2].toLongLong();
                cpuNow.sys = list[3].toLongLong();
                cpuNow.idle = list[4].toLongLong();
                cpuNow.iow = list[5].toLongLong();
                cpuNow.irq = list[6].toLongLong();
                cpuNow.softirq = list[7].toLongLong();


                qlonglong cpuSum = cpuNow.usr+cpuNow.nice+cpuNow.sys+cpuNow.idle+cpuNow.iow+cpuNow.irq+cpuNow.softirq;
                qlonglong cpuDelta = cpuSum - cpuLastSum;
                qlonglong cpuIdle = cpuNow.idle - cpuLast.idle;
                qlonglong cpuUsed = cpuDelta - cpuIdle;



                cpuUsage = 100.0*cpuUsed/cpuDelta;

                cpuLast = cpuNow;
                cpuLastSum = cpuSum;

                break;
            }
        }
        cmdFile.close();
    }

    return cpuUsage;
}

int SystemMonitor::getMem()
{
    qlonglong memFree = -1;


    std::string pidString ;
    std::ostringstream convert;   // stream used for the conversion
    pidString = convert.str();
    std::string cmdPath = std::string("/proc/meminfo");
    std::ifstream cmdFile(cmdPath.c_str());
    std::string cmdLine;
    while ( std::getline(cmdFile, cmdLine) )
    {
        std::size_t found = cmdLine.find("MemFree");
        if (!cmdLine.empty() && found!=std::string::npos)
        {
            // Keep first cmdline item which contains the program path
            QString vmSize = QString::fromStdString( cmdLine );
            vmSize = vmSize.left( vmSize.lastIndexOf(" ") ) ;
            vmSize = vmSize.right( vmSize.size()-1 - vmSize.lastIndexOf(" ") ) ;
            memFree = vmSize.toLongLong();

            break;
        }
    }

    int total =  getTotalSystemMemory();

    int usage = 100.0*(total-memFree) / total;

    return usage;
}


qlonglong SystemMonitor::getTotalSystemMemory()
{
    qlonglong memTotal = -1;

    std::string pidString ;
    std::ostringstream convert;   // stream used for the conversion
    pidString = convert.str();
    std::string cmdPath = std::string("/proc/meminfo");
    std::ifstream cmdFile(cmdPath.c_str());
    std::string cmdLine;
    while ( std::getline(cmdFile, cmdLine) )
    {
        std::size_t found = cmdLine.find("MemTotal");
        if (!cmdLine.empty() && found!=std::string::npos)
        {
            // Keep first cmdline item which contains the program path
            QString vmSize = QString::fromStdString( cmdLine );
            vmSize = vmSize.left( vmSize.lastIndexOf(" ") ) ;
            vmSize = vmSize.right( vmSize.size()-1 - vmSize.lastIndexOf(" ") ) ;
            memTotal = vmSize.toLongLong();

            break;
        }
    }
    return memTotal;
}
