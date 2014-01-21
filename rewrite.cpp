#include "rewrite.h"

Rewrite::Rewrite(ModuleConfig *conf, QString mf, QString t, QString cf, QString ct) :
    moduleConf(conf), moduleFrom(mf), type(t), classFrom(cf), classTo(ct)
{
}

bool Rewrite::createFile()
{
    bool res;
    QStringList classToParts = classFrom.split("_");
    QString curPath = moduleConf->getModulePath();
    for (int i = 2; i < classToParts.size(); i++) {
        if ((i + 1) < classToParts.size()) {
            QString dirName(classToParts.at(i));
            QDir currentDir(curPath);
            res = currentDir.mkdir(dirName);
            curPath += QDir::fromNativeSeparators("/" + dirName);
        } else {
            QFile file(QDir::fromNativeSeparators(curPath + "/" + classToParts.at(i) + ".php"));
                if (!file.exists()) {
                    res = file.open(QIODevice::WriteOnly | QIODevice::Text);
                    if (res) {
                        QTextStream stream(&file);
                        stream << "<?php\nclass " << classTo
                               << " extends \n" << classFrom << "\n{\n\n}\n";
                }
            }

        }
    }
    return res;
}
