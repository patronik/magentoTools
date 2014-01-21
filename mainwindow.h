#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonArray>

#include "configxml.h"
#include "moduleconfig.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool magentoExists;

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();    

    void on_pushButton_3_clicked();

    void on_addRewriteBtn_clicked();

    void on_companyDropDown_currentIndexChanged(int);

    void on_moduleDropDown_currentIndexChanged(int);

    void on_typeDropDown_currentIndexChanged(int);

    void on_classFromDropDown_currentIndexChanged(int);    

    void on_rewriteList_doubleClicked(const QModelIndex &);

    void on_companyField_textChanged(const QString &);

    void on_moduleNameField_textChanged(const QString &);    

    void on_applyObserverButtom_clicked();

    void on_eventList_doubleClicked(const QModelIndex &index);

    void on_observerEvent_currentIndexChanged(int index);

private:

    bool createSkeleton();

    bool removeDir(QDir dir);

    void initModuleConfig();

    void initRewrite();

    void initEvent();

    void initObserverClass();

    void updateRewriteModules();

    void updateRewriteTypes();

    void updateRewriteClasses();

    void updateRewriteClassTo();

    ModuleConfig * moduleConf;

    Ui::MainWindow * ui;

    ConfigXML * config;

};

#endif // MAINWINDOW_H
