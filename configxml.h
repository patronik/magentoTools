#ifndef CONFIGXML_H
#define CONFIGXML_H

#include <QObject>
#include <QMessageBox>

#include <QDomDocument>
#include <QDomElement>

#include <QFile>
#include <QDir>
#include <QTextStream>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QStringList>


#include "moduleconfig.h"
#include "rewrite.h"
#include "event.h"

class ConfigXML : public QObject
{
    Q_OBJECT
public:
    explicit ConfigXML(QObject *parent = 0, ModuleConfig *conf = 0);    

    bool createXML();

    bool deleteDeclFile();        

    QJsonObject * getClassData() {
        return &classData;
    }

    QJsonObject * getEventData() {
        return &eventData;
    }

    // list with applied rewrites
    QList<Rewrite> rewrites;

    // list with applied events
    QList<Event> events;

private:    

    ModuleConfig * moduleConf;

    // parsed json with class data
    QJsonObject classData;


    // parsed json with event data
    QJsonObject eventData;

    // ConmanyName_ModuleName
    QString CnMn;

    // for rewrite
    QDomDocument * configXMLDoc;

    bool createDeclFile();

    bool createConfigFile();    

    void applyRewrite(Rewrite & r, QDomElement &);

    void applyEvent(Event & e, QDomElement &);

    QString getClassFrom(QString classFrom);

    bool writeXML(QString path, QDomDocument & document);

    QString getDeclFilePath();

    QString getConfigFilePath();

    QDomElement createRouterArgs(QDomDocument &);    

};

#endif // CONFIGXML_H
