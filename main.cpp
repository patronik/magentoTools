#include "mainwindow.h"
#include <QApplication>

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
    #ifndef Q_OS_WIN32
        install();
    #endif

    QApplication a(argc, argv);

    MainWindow w;
    w.setFixedSize(w.width(),w.height());
    w.show();

    return a.exec();
}
