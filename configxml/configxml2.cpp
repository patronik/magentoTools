#include "../configxml.h"

bool ConfigXML::deleteDeclFile()
{
    return QFile::remove(getDeclFilePath());
}

QDomElement ConfigXML::createRouterArgs(QDomDocument & document)
{
    QDomElement args = document.createElement("args");

    // arg1
    QDomElement arg1 = document.createElement("module");
    arg1.appendChild(document.createTextNode(CnMn));
    args.appendChild(arg1);

    // arg2
    QDomElement arg2 = document.createElement("frontName");
    if (moduleConf->getRouterKey() != "") {
        arg2.appendChild(document.createTextNode(moduleConf->getRouterKey()));
    } else {
        arg2.appendChild(document.createTextNode(moduleConf->getModuleName().toLower()));
    }
    args.appendChild(arg2);

    return args;

}

QString ConfigXML::getClassFrom(QString classFrom)
{
    QStringList classFromParts = classFrom.split("_");
    QString result;
    for (int i = 3; i < classFromParts.size(); i++) {
        result += classFromParts.at(i).toLower();
        if ((i + 1) < classFromParts.size()) {
            result += "_";
        }
    }
    return result;
}


bool ConfigXML::writeXML(QString path, QDomDocument &document)
{
    QFile outFile(path);
    if (outFile.open( QIODevice::WriteOnly | QIODevice::Text )) {
        QTextStream stream(&outFile);
        stream << document.toString();
    } else {
        return false;
    }
    outFile.close();
    return true;
}

QString ConfigXML::getDeclFilePath()
{
    return moduleConf->getMagePath() + QDir::toNativeSeparators("/app/etc/modules/") + CnMn + ".xml";
}

QString ConfigXML::getConfigFilePath()
{
    return moduleConf->getModulePath() + QDir::toNativeSeparators("/etc/") + "config.xml";
}

