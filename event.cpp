#include "event.h"

Event::Event(ModuleConfig * conf, QString ar, QString ev, QString id, QString cl, QString met) :
    moduleConf(conf), area(ar), event(ev), identifier(id), className(cl), method(met)
{
}

bool Event::createFile()
{
    bool res;
    QStringList classToParts = className.split("_");
    QString curPath = moduleConf->getModulePath();
    for (int i = 2; i < classToParts.size(); i++) {
        if ((i + 1) < classToParts.size()) {
            QString dirName(classToParts.at(i));
            QDir currentDir(curPath);
            res = currentDir.mkdir(dirName);
            curPath += QDir::fromNativeSeparators("/" + dirName);
        } else {
            QFile file(
                    QDir::fromNativeSeparators(curPath + "/" + classToParts.at(i) + ".php")
                );
            if (!file.exists()) {
                res = file.open(QIODevice::WriteOnly | QIODevice::Text);
                if (res) {
                    QTextStream stream(&file);
                    stream << "<?php\nclass " << className << "\n{\n"
                           << " public function " << method << "($event)\n{\n\n}\n" << "\n}\n";
                }
            }

        }
    }
    return res;
}
