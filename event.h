#ifndef EVENT_H
#define EVENT_H

#include <QObject>
#include <QStringList>
#include <QTextStream>

#include "moduleconfig.h"

class Event
{
public:
    Event(ModuleConfig *conf = 0, QString ar = "", QString ev = "", QString id = "", QString cl = "", QString met = "");

    QString getArea(){
        return area;
    }

    QString getEvent(){
        return event;
    }

    QString getIdentifier(){
        return identifier;
    }

    QString getClassName(){
        return className;
    }

    QString getMethod(){
        return method;
    }

    bool createFile();

private:
    ModuleConfig * moduleConf;

    QString area;

    QString event;

    QString identifier;

    QString className;

    QString method;
};

#endif // EVENT_H
