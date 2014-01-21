#ifndef MODULECONFIG_H
#define MODULECONFIG_H

#include <QObject>
#include <QDir>

class ModuleConfig: public QObject
{
    Q_OBJECT
public:
     explicit ModuleConfig(QObject *parent = 0);

    void setMagePath(QString str){
        this->magePath = str;
    }

    void setPool(QString str){
        this->pool = str;
    }

    void setCompanyName(QString str){
        this->companyName = str;
    }

    void setModuleName(QString str){
        this->moduleName = str;
    }

    void setModuleVersion(QString n1, QString n2, QString n3){
        moduleVersion = n1 + "." + n2 + "." + n3;
    }

    void setRouterKey(QString val) {
        routerKey = val;
    }

    QString getMagePath(){
        return this->magePath;
    }

    QString getPool(){
        return this->pool;
    }

    QString getCompanyName(){
        return this->companyName;
    }

    QString getModuleName(){
        return this->moduleName;
    }

    QString getCompanyPath(){
        return this->magePath + QDir::toNativeSeparators("/app/code/")
                + pool + s + this->companyName;
    }

    QString getModulePath(){
        return this->magePath + QDir::toNativeSeparators("/app/code/")
                + pool + s + this->companyName + s + this->moduleName;
    }

    QString getModuleVersion(){
        return moduleVersion;
    }

    QString getSetupDirName() {
        return companyName.toLower() + "_" + moduleName.toLower() + "_setup";
    }

    QString getRouterKey() {
        return routerKey;
    }

    // blocks | controllers | models | helpers
    void setHasBlock(bool val){
        hasBlock = val;
    }

    void setHasHelper(bool val){
        hasHelper = val;
    }

    void setHasController(bool val){
        hasController = val;
    }

    void setHasAdminController(bool val){
        hasAdminController = val;
    }

    void setHasModel(bool val){
        hasModel = val;
    }

    void setHasScript(bool val){
        hasScript = val;
    }

    bool getHasBlock(){
        return hasBlock;
    }

    bool getHasHelper(){
        return hasHelper;
    }

    bool getHasController(){
        return hasController;
    }

    bool getHasAdminController(){
        return hasAdminController;
    }

    bool getHasModel(){
        return hasModel;
    }

    bool getHasScript(){
        return hasScript;
    }

    static QString s;

private:
    QString magePath;

    QString pool;

    QString companyName;

    QString moduleName;

    QString moduleVersion;

    QString routerKey;

    bool hasScript;

    // blocks | controllers | models | helpers
    bool hasBlock;

    bool hasHelper;

    bool hasController;

    bool hasAdminController;

    bool hasModel;

};

#endif // MODULECONFIG_H
