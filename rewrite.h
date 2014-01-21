#ifndef REWRITE_H
#define REWRITE_H

#include <QObject>
#include <QTextStream>

#include "moduleconfig.h"

class Rewrite
{

public:
    explicit Rewrite(ModuleConfig *conf = 0, QString mf = "", QString t = "", QString cf = "", QString ct = "");

    QString getModuleFrom() {
        return moduleFrom;
    }

    QString getType() {
        return type;
    }

    QString getClassFrom() {
        return classFrom;
    }

    QString getClassTo() {
        return classTo;
    }

    bool createFile();

signals:

public slots:

private:

    ModuleConfig * moduleConf;

    QString moduleFrom;

    QString type;

    QString classFrom;

    QString classTo;

};

#endif // REWRITE_H
