#include "configxml.h"

ConfigXML::ConfigXML(QObject *parent, ModuleConfig *conf) :
    QObject(parent), moduleConf(conf), configXMLDoc(0)
{
    // read classes from json
    QFile file(":/rc/classes.json");

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString content = file.readAll();
        QJsonParseError error;
        QJsonDocument jd = QJsonDocument::fromJson(
                    content.toUtf8(), &error);
        if (error.error == QJsonParseError::NoError) {
            classData = jd.object();
        } else {
            QMessageBox::warning(0, tr("Error"),
                error.errorString(), QMessageBox::Ok);
        }
        file.close();
    } else {
        QMessageBox::warning(0, tr("Error"), 
            tr("Cannot read json file"), QMessageBox::Ok);
    }

    // read events from json
    QFile file2(":/rc/events.json");

    if (file2.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString content = file2.readAll();
        QJsonParseError error;
        QJsonDocument jd = QJsonDocument::fromJson(
                    content.toUtf8(), &error);
        if (error.error == QJsonParseError::NoError) {
            eventData = jd.object();
        } else {
            QMessageBox::warning(0, tr("Error"),
                error.errorString(), QMessageBox::Ok);
        }
        file2.close();
    } else {
        QMessageBox::warning(0, tr("Error"),
            tr("Cannot read json file"), QMessageBox::Ok);
    }
}

bool ConfigXML::createXML()
{
    CnMn = moduleConf->getCompanyName()
            + "_" + moduleConf->getModuleName();
    bool res1 = createDeclFile();
    bool res2 = createConfigFile();
    return res1 && res2;
}

bool ConfigXML::createDeclFile()
{    
    QDomDocument document;    
    QDomProcessingInstruction xmldecl = document.createProcessingInstruction("xml", "version=\"1.0\"");
    document.appendChild(xmldecl);

    QDomElement config = document.createElement("config");
    document.appendChild(config);

    QDomElement modules = document.createElement("modules");
    config.appendChild(modules);

    QDomElement module = document.createElement(CnMn);
    modules.appendChild(module);

    QDomElement active = document.createElement("active");    
    active.appendChild(document.createTextNode("true"));
    module.appendChild(active);

    QDomElement poolElement = document.createElement("codePool");    
    poolElement.appendChild(document.createTextNode(moduleConf->getPool()));
    module.appendChild(poolElement); 

    return writeXML(getDeclFilePath(), document);
}

bool ConfigXML::createConfigFile()
{
    bool hasGlobal = false, hasFrontend = false, hasAdmin = false;

    QDomDocument document;
    configXMLDoc = &document;

    QDomProcessingInstruction xmldecl = document.createProcessingInstruction("xml", "version=\"1.0\"");
    document.appendChild(xmldecl); 

    // config element
    QDomElement config = document.createElement("config");
    document.appendChild(config);



    // MODULES & VERSION /////////////////////////////////////////
    // modules section
    QDomElement modules = document.createElement("modules");
    config.appendChild(modules);

    // module name
    QDomElement module = document.createElement(CnMn);
    modules.appendChild(module);

    // version
    QDomElement version = document.createElement("version");    
    version.appendChild(document.createTextNode(moduleConf->getModuleVersion()));
    module.appendChild(version);    
    // MODULES & VERSION END /////////////////////////////////////////

    // init global, frontend, admin sections
    QDomElement global = document.createElement("global");
    QDomElement frontend = document.createElement("frontend");
    QDomElement admin = document.createElement("admin");


    // blocks | models | helpers   
    if (moduleConf->getHasBlock()        
        || moduleConf->getHasHelper()
        || moduleConf->getHasModel()
        || moduleConf->getHasScript()
       )
    {
        hasGlobal = true;

        // init blocks, helpers, models sections
        QDomElement blocks = document.createElement("blocks");
        QDomElement helpers = document.createElement("helpers");
        QDomElement models = document.createElement("models");

        if (moduleConf->getHasBlock()) {            
            // block module
            QDomElement blockModule = document.createElement(
                        moduleConf->getModuleName().toLower()
                    );
            blocks.appendChild(blockModule);

            // class
            QDomElement blockClass = document.createElement("class");            
            blockClass.appendChild(document.createTextNode(CnMn + "_Block"));
            blockModule.appendChild(blockClass);

            // blocks section
            global.appendChild(blocks);
        }

        if (moduleConf->getHasHelper()) {            
            // helpers module
            QDomElement helperModule = document.createElement
                    (moduleConf->getModuleName().toLower()
                    );
            helpers.appendChild(helperModule);

            // class
            QDomElement helperClass = document.createElement("class");            
            helperClass.appendChild(document.createTextNode(CnMn + "_Helper"));
            helperModule.appendChild(helperClass);

            // helpers section
            global.appendChild(helpers);
        }

        if (moduleConf->getHasModel()) {            
            // block module
            QDomElement modelModule = document.createElement(moduleConf->getModuleName().toLower());
            models.appendChild(modelModule);

            // class
            QDomElement modelClass = document.createElement("class");            
            modelClass.appendChild(document.createTextNode(CnMn + "_Model"));
            modelModule.appendChild(modelClass);

            // models section
            global.appendChild(models);
        }

        // apply selected rewrites
        for (int i = 0; i < rewrites.size(); i++) {

            hasGlobal = true;

            Rewrite r = rewrites.at(i);

            if (r.getType() == "Block") {
                applyRewrite(r, blocks);
            }

            if (r.getType() == "Helper") {
                applyRewrite(r, helpers);
            }

            if (r.getType() == "Model") {
                applyRewrite(r, models);
            }

            r.createFile();
        }



        //////////////////// EVENTS ///////////////////
        // apply selected events
        QDomElement globalEvents = document.createElement("events");
        QDomElement frontendEvents = document.createElement("events");
        QDomElement adminEvents = document.createElement("events");
        for (int i = 0; i < events.size(); i++) {

            Event e = events.at(i);

            if (e.getArea() == "global") {
                hasGlobal = true;
                applyEvent(e, globalEvents);
            }

            if (e.getArea() == "frontend") {
                hasFrontend = true;
                applyEvent(e, frontendEvents);
            }

            if (e.getArea() == "admin") {
                hasAdmin = true;
                applyEvent(e, adminEvents);
            }

            e.createFile();
        }

        if (globalEvents.childNodes().size() > 0) {
            global.appendChild(globalEvents);
        }

        if (frontendEvents.childNodes().size() > 0) {
            frontend.appendChild(frontendEvents);
        }

        if (adminEvents.childNodes().size() > 0) {
            admin.appendChild(adminEvents);
        }
        //////////////////// EVENTS END ///////////////////



        if (moduleConf->getHasScript()) {

            hasGlobal = true;

            // resources section
            QDomElement resourcesElem = document.createElement("resources");            

            // resources setup module
            QDomElement moduleToSetup = document.createElement(moduleConf->getSetupDirName());
            resourcesElem.appendChild(moduleToSetup);

            // setup section
            QDomElement setupElem = document.createElement("setup");
            moduleToSetup.appendChild(setupElem);

            QDomElement moduleElem = document.createElement("module");
            moduleElem.appendChild(document.createTextNode(CnMn));

            setupElem.appendChild(moduleElem);

            global.appendChild(resourcesElem);
        }        


    }

    if (moduleConf->getHasController() ||
            moduleConf->getHasAdminController()
        )
    {                       
        // frontend router
        if (moduleConf->getHasController()) {

            hasFrontend = true;

            // routers
            QDomElement routers = document.createElement("routers");

            // router module
            QDomElement routerModule1 = document.createElement(
                        moduleConf->getModuleName().toLower()
                    );            

            QDomElement use = document.createElement("use");
            use.appendChild(document.createTextNode("standard"));

            routerModule1.appendChild(use);

            routerModule1.appendChild(createRouterArgs(document));
            routers.appendChild(routerModule1);
            frontend.appendChild(routers);            
        }

        // admin router
        if (moduleConf->getHasAdminController()) {                   

            hasAdmin = true;

            // routers
            QDomElement routers = document.createElement("routers");                        

            // router module
            QDomElement routerModule2 = document.createElement(
                        moduleConf->getModuleName().toLower()
                    );            

            QDomElement use = document.createElement("use");                       
            use.appendChild(document.createTextNode("admin"));

            routerModule2.appendChild(use);          

            routerModule2.appendChild(createRouterArgs(document));
            routers.appendChild(routerModule2);
            admin.appendChild(routers);

        }
    }


    // add sections and write to file
    if (hasGlobal) {
        config.appendChild(global);
    }

    if (hasFrontend) {
        config.appendChild(frontend);
    }

    if (hasAdmin) {
        config.appendChild(admin);
    }

    return writeXML(getConfigFilePath(), document);
}


void ConfigXML::applyRewrite(Rewrite &r, QDomElement & section)
{
    QDomElement moduleRewriteFrom = configXMLDoc->createElement(
                r.getModuleFrom().toLower()
            );    
    section.appendChild(moduleRewriteFrom);

    QDomElement rewriteElemet = configXMLDoc->createElement("rewrite");
    moduleRewriteFrom.appendChild(rewriteElemet);

    QDomElement classFromElement = configXMLDoc->createElement(
                getClassFrom(r.getClassFrom())
            );
    rewriteElemet.appendChild(classFromElement);

    classFromElement.appendChild(
                configXMLDoc->createTextNode(r.getClassTo())
                );
}

void ConfigXML::applyEvent(Event & e, QDomElement & section)
{
    QDomElement event = configXMLDoc->createElement(e.getEvent());
    QDomElement observers = configXMLDoc->createElement("observers");
    QDomElement identifier = configXMLDoc->createElement(e.getIdentifier());
    QDomElement className = configXMLDoc->createElement("class");
    className.appendChild(
                configXMLDoc->createTextNode(e.getClassName())
                );
    QDomElement method = configXMLDoc->createElement("method");
    method.appendChild(
                configXMLDoc->createTextNode(e.getMethod())
                );
    identifier.appendChild(className);
    identifier.appendChild(method);

    observers.appendChild(identifier);

    event.appendChild(observers);

    section.appendChild(event);
}


