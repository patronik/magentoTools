#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->codePoolBox->addItem(QString("local"));
    ui->codePoolBox->addItem(QString("community"));

    for (int i = 0; i < 10; i++) {
        ui->vnum1->addItem(QString::number(i));
        ui->vnum2->addItem(QString::number(i));
        ui->vnum3->addItem(QString::number(i));
    }

    magentoExists = false;

    ui->MagentoRootPathLabel
            ->setText(tr("Please choose magento root directory"));


    moduleConf = new ModuleConfig(this);

    config = new ConfigXML(this, moduleConf);

    initModuleConfig();

    initRewrite();

    initEvent();

    initObserverClass();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete moduleConf;
    delete config;
}

void MainWindow::initModuleConfig()
{
    moduleConf->setPool(ui->codePoolBox->currentText());
    moduleConf->setCompanyName(ui->companyField->text());
    moduleConf->setModuleName(ui->moduleNameField->text());
    moduleConf->setModuleVersion(
        ui->vnum1->currentText(),
        ui->vnum2->currentText(),
        ui->vnum3->currentText()
    );
    moduleConf->setHasBlock(ui->blockCheckBox->isChecked());
    moduleConf->setHasHelper(ui->helperCheckBox->isChecked());
    moduleConf->setHasController(ui->controllerCheckBox->isChecked());
    moduleConf->setHasAdminController(ui->adminControllerCheckBox->isChecked());
    moduleConf->setHasModel(ui->modelCheckBox->isChecked());
    moduleConf->setRouterKey(ui->routerName->text());
    moduleConf->setHasScript(ui->installCheckBox->isChecked());
}


// Select dir
void MainWindow::on_pushButton_clicked()
{
    QDir rootDir = QDir::root();
    QString path = QDir::toNativeSeparators(QFileDialog::getExistingDirectory(this, 
			tr("Open Directory"), rootDir.path(),
                    		QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks));

    magentoExists = QDir(path).exists(QString("app")) &&
            QDir(path).exists(QString("lib"));

    if (magentoExists) {
        moduleConf->setMagePath(path);
        ui->MagentoRootPathLabel->setText(moduleConf->getMagePath());
    }
}

bool MainWindow::createSkeleton()
{        

    if(QDir().exists(moduleConf->getCompanyPath())
        || QDir().mkdir(moduleConf->getCompanyPath())
      )
    {
        bool res;

        res = QDir().mkdir(moduleConf->getModulePath());

        res = QDir(moduleConf->getModulePath()).mkdir(QString("etc"));

        if (ui->blockCheckBox->isChecked()) {
            res = QDir(moduleConf->getModulePath()).mkdir(QString("Block"));
        }

        if (ui->controllerCheckBox->isChecked()) {
            res = QDir(moduleConf->getModulePath()).mkdir(QString("controllers"));
        }

        if (ui->helperCheckBox->isChecked()) {
            res = QDir(moduleConf->getModulePath()).mkdir(QString("Helper"));
        }

        if (ui->modelCheckBox->isChecked()) {
            res = QDir(moduleConf->getModulePath()).mkdir(QString("Model"));
        }

        // install script
        if (ui->installCheckBox->isChecked()) {
            res = QDir(moduleConf->getModulePath()).mkdir(QString("sql"));
            if (res) {

                QString sqlDirPath = QDir::toNativeSeparators(
                            moduleConf->getModulePath() + QString("/sql")
                        );

                res = QDir(sqlDirPath).mkdir(moduleConf->getSetupDirName());

                QString scriptFilePath = QDir::toNativeSeparators(
                            sqlDirPath + "/" + moduleConf->getSetupDirName()
                            + "/mysql4-install-" + moduleConf->getModuleVersion() + ".php"
                    );

                QFile scriptFile(scriptFilePath);

                res = scriptFile.open(QIODevice::WriteOnly | QIODevice::Text);

                if (res) {
                    QTextStream stream(&scriptFile);
                    stream << "<?php\n\n\n?>";
                }
            }
        }
        return res;

    } else {
        QMessageBox::warning(this, tr("Error"),
                tr("Cannot create module directoties\n"
                   "Please check file permissions"), QMessageBox::Ok);        
    }
    return false;
}

bool MainWindow::removeDir(QDir dir)
{
    if ( !dir.exists() )
        return false;

    foreach ( QFileInfo fi,
        dir.entryInfoList(
            QDir::AllEntries |
            QDir::NoDotAndDotDot |
            QDir::Hidden |
            QDir::System
        ) )
    {
        if ( fi.isSymLink() || fi.isFile() )
        {
            if ( !dir.remove(fi.absoluteFilePath()) )
                return false;
        }
        else if ( fi.isDir() )
        {
            if ( !removeDir(fi.filePath()) )
                return false;
        }
    }

    if ( !dir.rmdir(dir.absolutePath()) )
        return false;

    return true;
}

// Save
void MainWindow::on_pushButton_2_clicked()
{
    if (!magentoExists) {
        QMessageBox::information(this, tr("Notice"),
            tr("Please choose magento root directory"), QMessageBox::Ok);
        return;
    }

    // init config
    initModuleConfig();

    if(createSkeleton() && config->createXML()) {
        QMessageBox::information(this, tr("Success"),
                tr("Module has been created successfully"), QMessageBox::Ok);
    }
}

// Clean
void MainWindow::on_pushButton_3_clicked()
{
    if (!magentoExists) {
        QMessageBox::information(this, tr("Notice"),
            tr("Please choose magento root directory"), QMessageBox::Ok);
        return;
    }

    initModuleConfig();

    removeDir(QDir(moduleConf->getModulePath()));

    // remove company dir also if this was the last one module
    QFileInfoList dirList = QDir(moduleConf->getCompanyPath()).entryInfoList(
        QDir::AllEntries |
        QDir::NoDotAndDotDot |
        QDir::Hidden |
        QDir::System
    );

    if (dirList.isEmpty()) {
        removeDir(QDir(moduleConf->getCompanyPath()));
    }

    config->deleteDeclFile();

    QMessageBox::information(this, tr("Success"),
            tr("Module has been removed successfully"), QMessageBox::Ok);
}


void MainWindow::on_companyField_textChanged(const QString &)
{
    initModuleConfig();

    updateRewriteModules();
    updateRewriteTypes();
    updateRewriteClasses();
    updateRewriteClassTo();

    initObserverClass();
}

void MainWindow::on_moduleNameField_textChanged(const QString &)
{
    initModuleConfig();

    updateRewriteModules();
    updateRewriteTypes();
    updateRewriteClasses();
    updateRewriteClassTo();

    initObserverClass();
}

