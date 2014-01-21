#include "mainwindow.h"
#include <QApplication>
#include <QFontDatabase>
#include <QLibraryInfo>
#include <QDebug>

void install() {

    if (QDir(QDir::toNativeSeparators(QDir::currentPath() + "/lib")).exists()) {
        return;
    }

    QDir().mkdir(QDir::toNativeSeparators(QDir::currentPath() + "/lib"));
    QDir().mkdir(QDir::toNativeSeparators(QDir::currentPath() + "/lib/fonts"));
    QFile::copy(":/fonts/Sans_Serif.ttf",
                QDir::toNativeSeparators(QDir::currentPath() + "/lib/fonts/Sans_Serif.ttf"));

}

int main(int argc, char *argv[])
{
    install();

    QApplication a(argc, argv);

    MainWindow w;
    w.setFixedSize(w.width(),w.height());
    w.show();

    return a.exec();
}
