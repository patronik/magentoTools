#include "moduleconfig.h"

QString ModuleConfig::s =  QDir::separator();

ModuleConfig::ModuleConfig(QObject *parent) :
    QObject(parent)
{
}

