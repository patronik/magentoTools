#include "../mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::initEvent()
{
    // init events
    QJsonObject * pDoc = config->getEventData();
    QStringList eventsKeys = pDoc->keys();
    for (int i = 0; i < eventsKeys.size(); ++i) {
        QJsonValue value = (*pDoc)[eventsKeys[i]];
        ui->observerEvent->addItem(value.toString());
    }

    ui->observerAreaDropDown->addItem("global");
    ui->observerAreaDropDown->addItem("frontend");
    ui->observerAreaDropDown->addItem("admin");
}

void MainWindow::initObserverClass()
{
    QString className = moduleConf->getCompanyName()
            + "_" + moduleConf->getModuleName();

    ui->observerIdentifier->setText(
            className.toLower() + "_" + ui->observerEvent->currentText()
        );

    QString curClass = ui->observerClass->text();
    if (curClass != "") {
        QStringList classParts = curClass.split("_");
        if (classParts.size() == 4) {
            ui->observerClass->setText(className + "_Model_" + classParts.at(3));
        } else {
            ui->observerClass->setText(className + "_Model_Observer");
        }
    } else {
        ui->observerClass->setText(className + "_Model_Observer");
    }
}

void MainWindow::on_observerEvent_currentIndexChanged(int)
{
    initObserverClass();
}

void MainWindow::on_applyObserverButtom_clicked()
{
    QString area = ui->observerAreaDropDown->currentText();
    QString event = ui->observerEvent->currentText();
    QString identifier = ui->observerIdentifier->text();
    QString className = ui->observerClass->text();
    QString method = ui->observerMethod->text();

    if (area != "" && event != "" && identifier != ""
        && className != "" && method != "") {
        config->events.append(Event(moduleConf, area, event, identifier, className, method));
        ui->eventList->addItem(area + "\n " + event + "\n  " +
                identifier + "\n   " + className + "\n    " + method);
    }

}

void MainWindow::on_eventList_doubleClicked(const QModelIndex &)
{
    QListWidgetItem * itemToDelete = ui->eventList->currentItem();
    config->events.removeAt(ui->eventList->currentRow());
    delete itemToDelete;
}
