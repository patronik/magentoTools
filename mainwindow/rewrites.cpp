#include "../mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::initRewrite()
{
    // init companies
    QStringList companyKeys = config->getClassData()->keys();
    for (int i = 0; i < companyKeys.size(); ++i) {        
        ui->companyDropDown->addItem(companyKeys.at(i));
    }
    updateRewriteModules();
    updateRewriteTypes();
    updateRewriteClasses();
    updateRewriteClassTo();
}

void MainWindow::updateRewriteModules()
{
    // update modules
    QString companyKey = ui->companyDropDown->currentText();
    QJsonValue company = (*config->getClassData())[companyKey];
    QStringList moduleKeys = company.toObject().keys();
    ui->moduleDropDown->clear();
    for (int i = 0; i < moduleKeys.size(); ++i) {        
        ui->moduleDropDown->addItem(moduleKeys.at(i));
    }
}

void MainWindow::updateRewriteTypes()
{
    // update types
    QString companyKey = ui->companyDropDown->currentText();
    QJsonValue company = (*config->getClassData())[companyKey];

    QJsonValue module = company.toObject()[ui->moduleDropDown->currentText()];
    QStringList typeKeys = module.toObject().keys();
    ui->typeDropDown->clear();
    for (int i = 0; i < typeKeys.size(); ++i) {        
        ui->typeDropDown->addItem(typeKeys.at(i));
    }
}

void MainWindow::updateRewriteClasses()
{
    // update classes
    QString companyKey = ui->companyDropDown->currentText();
    QJsonValue company = (*config->getClassData())[companyKey];

    QString moduleKey = ui->moduleDropDown->currentText();
    QJsonValue module = company.toObject()[moduleKey];

    QString typeKey = ui->typeDropDown->currentText();
    QJsonValue type = module.toObject()[typeKey];
    QJsonArray classArray = type.toArray();

    ui->classFromDropDown->clear();
    for (int i = 0; i < classArray.size(); ++i) {        
        ui->classFromDropDown->addItem(classArray[i].toString());
    }    
}

void MainWindow::updateRewriteClassTo()
{
    QString classFrom = ui->classFromDropDown->currentText();
    QStringList classParts = classFrom.split("_");
    if (classParts.size() > 3) {
        QString classTo = moduleConf->getCompanyName() + "_" + moduleConf->getModuleName();
        for (int i = 2; i < classParts.size(); i++) {
            classTo += "_" + classParts.at(i);
        }
        ui->classToField->setText(classTo);
    }
}

void MainWindow::on_companyDropDown_currentIndexChanged(int)
{        
    updateRewriteModules();
    updateRewriteTypes();
    updateRewriteClasses();
}

void MainWindow::on_moduleDropDown_currentIndexChanged(int)
{
    updateRewriteTypes();
    updateRewriteClasses();
}

void MainWindow::on_typeDropDown_currentIndexChanged(int)
{    
    updateRewriteClasses();
}

void MainWindow::on_classFromDropDown_currentIndexChanged(int)
{    
    updateRewriteClassTo();
}

// Add rewrite
void MainWindow::on_addRewriteBtn_clicked()
{
    QString mf = ui->moduleDropDown->currentText();
    QString type = ui->typeDropDown->currentText();
    QString cf = ui->classFromDropDown->currentText();
    QString ct = ui->classToField->text();

    config->rewrites.append(Rewrite(moduleConf, mf, type, cf, ct));
    ui->rewriteList->addItem(cf + "\n" + ct + "\n");
}

// Remove rewrite
void MainWindow::on_rewriteList_doubleClicked(const QModelIndex &)
{
    QListWidgetItem * itemToDelete = ui->rewriteList->currentItem();
    config->rewrites.removeAt(ui->rewriteList->currentRow());
    delete itemToDelete;        
}
