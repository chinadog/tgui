#include "tgui.h"
#include <QApplication>
#include <QTranslator>
#include <TLogger/TLogger>
#include <Common/PreLoad>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString configPath = PreLoad::getConfigPath(argc,argv,CONFIG_PATH);
    TGUI* w;
    if( PreLoad::configIsValid(configPath+"settings.conf") )
    {
        TLogger::get().init(configPath.toStdString()+"settings.conf");
        // Загрузка переводов
        QTranslator myTranslator;
        QString translationPath = RESOURCES_PATH;
        translationPath += "translation/";
        myTranslator.load("trifecta_ru",translationPath);
        a.installTranslator(&myTranslator);
        TINFO() << "Start the server with the" << configPath <<
                   "configuration path";
        w = new TGUI(configPath);
        w->show();
    }
    else
    {
        TERROR() << "Configuration file doesn't exist in folder - " <<
                    configPath;
        TERROR() << "Closing application";
        return 1;
    }
    int r = a.exec();
    delete w;
    return r;
}
