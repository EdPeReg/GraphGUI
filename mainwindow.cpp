#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->spBxColorR->setMaximum(255);
    ui->spBxColorG->setMaximum(255);
    ui->spBxColorB->setMaximum(255);

    connect(ui->btnSend, &QPushButton::clicked, this, &MainWindow::btnSendPressed);
    connect(ui->btnShow, &QPushButton::clicked, this, &MainWindow::btnShowPressed);
    connect(ui->action_Open, &QAction::triggered, this, &MainWindow::openJsonFile);
    connect(ui->action_Save, &QAction::triggered, this, &MainWindow::saveJsonFile);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::btnSendPressed() {
    if(validateLnInput()) {
        setParticlesInformation();
        cleanFields();
    }
}

void MainWindow::btnShowPressed() {
    ShowInformation showInfoDialog(this, particlesInformation);
    showInfoDialog.setModal(true);
    showInfoDialog.exec();
}

bool MainWindow::validateLnInput() {
    // Because every line edit starts with those characters.
    QRegularExpression exprLineEdit("(lnEdt)");

    // Find every widget.
    QList<QWidget*> lnEditWidgets = QObject::findChildren<QWidget *>(exprLineEdit);

    // Obtain line edit information.
    foreach(QWidget *lnEditWidget, lnEditWidgets) {
        QRegularExpression regAnyDigit("^[0-9]*$"); 	   // Accepts only numbers, no characters.
        QLineEdit *ln = qobject_cast<QLineEdit *>(lnEditWidget); // Convert my widget to line edit.

        if(ln->text() != "") {
            QRegularExpressionMatch match = regAnyDigit.match(ln->text());

            if(!match.hasMatch()) {
                QMessageBox::warning(this, "ERROR", "Insert only numbers");
                return false;
            }
        }
        // If there is some line edits empty.
        else {
            QMessageBox::warning(this, "ERROR", "Some fields are missing");
            return false;
        }
    }

    return true;
}

void MainWindow::setParticlesInformation() {
    particleInformation["id"] = ui->lnEdtID->text().toInt();
    particleInformation["origen X"] = ui->lnEdtOrigX->text().toInt();
    particleInformation["origen Y"] = ui->lnEdtOrigY->text().toInt();
    particleInformation["destino X"] = ui->lnEdtDestX->text().toInt();
    particleInformation["destino Y"] = ui->lnEdtDestY->text().toInt();
    particleInformation["velocidad"] = ui->lnEdtSpeed->text().toInt();
    particleInformation["R"] = ui->spBxColorR->value();
    particleInformation["G"] = ui->spBxColorG->value();
    particleInformation["B"] = ui->spBxColorB->value();
    particlesInformation.push_back(particleInformation);
}

void MainWindow::setParticlesInformation(const QJsonArray &jsonDocumentArray)
{
    foreach(const QJsonValue &value, jsonDocumentArray) {
        QJsonObject obj = value.toObject();

        particleInformation["id"] = obj["id"].toInt();

        QJsonObject obj2 = obj.value("origen").toObject();
        particleInformation["origen X"] = obj2["x"].toInt();
        particleInformation["origen Y"] = obj2["y"].toInt();

        obj2 = obj.value("destino").toObject();
        particleInformation["destino X"] = obj2["x"].toInt();
        particleInformation["destino Y"] = obj2["y"].toInt();
        particleInformation["velocidad"] = obj["velocidad"].toInt();

        obj2 = obj.value("color").toObject();
        particleInformation["R"] = obj2["red"].toInt();
        particleInformation["G"] = obj2["green"].toInt();
        particleInformation["B"] = obj2["blue"].toInt();

        particlesInformation.push_back(particleInformation);
    }
}

void MainWindow::openJsonFile()
{
    QString filename = QFileDialog::getOpenFileName(this, "Open file", "./", "JSON (*.json)");
    QFile file(filename);

    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, "Error", "Cannot open file");
    }

    readJsonFile(file);
}

void MainWindow::readJsonFile(QFile &file)
{
    QByteArray data = file.readAll();
    file.close();

    // Creating a json file and passing the file content to a json document.
    QJsonDocument jsonDocument(QJsonDocument::fromJson(data));

    // Our json file is a json array.
    QJsonArray jsonDocumentArray = jsonDocument.array();
    setParticlesInformation(jsonDocumentArray);
}

void MainWindow::saveJsonFile()
{
    QString filename = QFileDialog::getSaveFileName(this, "Open file", "./", "JSON (*.json)");
    QFile file(filename);

    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "Cannot save file");
    }

    QJsonArray jsonArray = particlesInformationIntoJsonArray();
    QJsonDocument document(jsonArray);
    file.write(document.toJson());

    file.close();
}

QJsonArray MainWindow::particlesInformationIntoJsonArray()
{
    QVector< QMap<QString, int> >::iterator vit;

    int i = 0;
    QJsonArray jsonArray;
    QJsonObject mainObject;
    QJsonObject secondaryObject;

    for(vit = particlesInformation.begin(); vit != particlesInformation.end(); vit++) {
        auto id = particlesInformation.at(i).find("id");
        auto origX = particlesInformation.at(i).find("origen X");
        auto origY = particlesInformation.at(i).find("origen Y");
        auto destX = particlesInformation.at(i).find("destino X");
        auto destY = particlesInformation.at(i).find("destino Y");
        auto velocidad = particlesInformation.at(i).find("velocidad");
        auto red = particlesInformation.at(i).find("R");
        auto green = particlesInformation.at(i).find("G");
        auto blue = particlesInformation.at(i).find("B");

        secondaryObject.insert("blue", blue.value());
        secondaryObject.insert("green", green.value());
        secondaryObject.insert("red", red.value());
        mainObject.insert("color", secondaryObject);
        // Clear object because the previous values still there.
        secondaryObject = QJsonObject();

        secondaryObject.insert("y", destY.value());
        secondaryObject.insert("x", destX.value());
        mainObject.insert("destino", secondaryObject);

        secondaryObject.insert("y", origY.value());
        secondaryObject.insert("x", origX.value());
        mainObject.insert("origen", secondaryObject);
        secondaryObject = QJsonObject();

        mainObject.insert("id", id.value());
        mainObject.insert("velocidad", velocidad.value());

        jsonArray.insert(i, mainObject);

        i++;
    }

    return jsonArray;
}

void MainWindow::cleanFields() {
    QRegularExpression expLnEdt("lnEdt");
    QRegularExpression expSpBox("spBx");
    QList<QWidget *> widgetsLnEdt = QObject::findChildren<QWidget*>(expLnEdt);
    QList<QWidget *> widgetsSpBox = QObject::findChildren<QWidget*>(expSpBox);
    foreach(QWidget *widgetLnEdt, widgetsLnEdt) {
        QLineEdit *lnEdt = qobject_cast<QLineEdit*>(widgetLnEdt);
        lnEdt->clear();
    }

    foreach(QWidget *widgetSpBox, widgetsSpBox) {
        QSpinBox *spBox = qobject_cast<QSpinBox*>(widgetSpBox);
        spBox->setValue(0);
    }
}


