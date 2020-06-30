#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    if(ui->tabWidget->currentIndex() == 0) {
        tabSelected();
    }

    ui->spBxColorR->setMaximum(255);
    ui->spBxColorG->setMaximum(255);
    ui->spBxColorB->setMaximum(255);

    ui->txtEdtParticleInfo->setReadOnly(true);

    connect(ui->btnSave, &QPushButton::clicked, this, &MainWindow::btnSavePressed);
    connect(ui->btnShow, &QPushButton::clicked, this, &MainWindow::btnShowPressed);
    connect(ui->action_Open, &QAction::triggered, this, &MainWindow::openJsonFile);
    connect(ui->action_Save, &QAction::triggered, this, &MainWindow::saveJsonFile);
    connect(ui->tabWidget, &QTabWidget::currentChanged, this, &MainWindow::tabSelected);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::btnSavePressed() {
    if(validateLnInput()) {
        setParticlesInformation();
        cleanFields();
    }
}

void MainWindow::btnShowPressed() {
    ui->txtEdtParticleInfo->clear();

    QString value;
    int i = 0;
    foreach(const auto &element, particlesInformation) {
        QString aux2 = "Particula: " + QString::number(i + 1);
        ui->txtEdtParticleInfo->append(aux2);
        auto id = element.find("id");
        auto origX = element.find("origen X");
        auto origY = element.find("origen Y");
        auto destX = element.find("destino X");
        auto destY = element.find("destino Y");
        auto velocidad = element.find("velocidad");
        auto red = element.find("R");
        auto green = element.find("G");
        auto blue = element.find("B");
        int distance = computeEuclideanDist(*origX, *origY, *destX, *destY);

        QString value = QString::number(id.value());
        ui->txtEdtParticleInfo->append(id.key() + " : " + value);
        value = QString::number(origX.value());
        ui->txtEdtParticleInfo->append(origX.key() + " : " + value);
        value = QString::number(origY.value());
        ui->txtEdtParticleInfo->append(origY.key() + " : " + value);
        value = QString::number(destX.value());
        ui->txtEdtParticleInfo->append(destX.key() + " : " + value);
        value = QString::number(destY.value());
        ui->txtEdtParticleInfo->append(destY.key() + " : " + value);
        value = QString::number(velocidad.value());
        ui->txtEdtParticleInfo->append(velocidad.key() + " : " + value);
        value = QString::number(red.value());
        ui->txtEdtParticleInfo->append(red.key() + " : " + value);
        value = QString::number(green.value());
        ui->txtEdtParticleInfo->append(green.key() + " : " + value);
        value = QString::number(blue.value());
        ui->txtEdtParticleInfo->append(blue.key() + " : " + value);
        value = QString::number(distance);
        QString auxStr = "Distancia euclidiana";
        ui->txtEdtParticleInfo->append(auxStr + " : " + value);
        ui->txtEdtParticleInfo->append("\n");

        i++;
    }

//    ShowInformation showInfoDialog(this, particlesInformation);
//    showInfoDialog.setModal(true);
//    showInfoDialog.exec();
}

bool MainWindow::validateLnInput() {
    // Because every line edit starts with those characters.
    QRegularExpression exprLineEdit("(lnEdt)");

    QWidget *tabWidget = ui->tabWidget->widget(ui->tabWidget->currentIndex());

    // Find the widgets that are in our tab.
    QObjectList widgetList = tabWidget->children();


    // Obtain line edit information.
    foreach(auto lnEditWidget, widgetList) {
        QRegularExpression regAnyDigit("^[0-9]*$"); 	   // Accepts only numbers, no characters.
        QLineEdit *ln = qobject_cast<QLineEdit *>(lnEditWidget); // Convert my widget to line edit.

        // If the widget couldn't be converted, go to the next widget.
        if(ln == nullptr) {
            continue;
        }

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

    QJsonArray jsonArray = particlesInformationToJsonArray();
    QJsonDocument document(jsonArray);
    file.write(document.toJson());

    file.close();
}

void MainWindow::tabSelected()
{

    switch(ui->tabWidget->currentIndex()) {
        case ADD_PARTICLE:
//            validateLnInput(tabWidget);
        break;

        break;
        case TABLE:
//            QObjectList list = tabWidget->children();
//            qDebug() << list << "\n\n";
            setTable();
        break;
    }
}

QJsonArray MainWindow::particlesInformationToJsonArray()
{
    int i = 0;
    QJsonArray jsonArray;
    QJsonObject mainObject;
    QJsonObject secondaryObject;

    foreach(const auto &element, particlesInformation) {
        auto id = element.find("id");
        auto origX = element.find("origen X");
        auto origY = element.find("origen Y");
        auto destX = element.find("destino X");
        auto destY = element.find("destino Y");
        auto velocidad = element.find("velocidad");
        auto red = element.find("R");
        auto green = element.find("G");
        auto blue = element.find("B");

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

void MainWindow::setTable()
{
    QStringList labels = {"ID", "Origin X", "Origin Y", "Destination X", "Destination Y",
                               "Speed", "R", "G", "B"};

    int totalColumns = 10; // This also includes eucledian distance.
    ui->tblWgtParticleInfo->setColumnCount(totalColumns);
    ui->tblWgtParticleInfo->setRowCount(particlesInformation.size());
    ui->tblWgtParticleInfo->setHorizontalHeaderLabels(labels);

    for(int i = 0; i < totalColumns; i++) {
//    	ui->tblWgtParticleInfo->setItem()
//        ui->tblWgtParticleInfo->setHorizontalHeaderItem(i, QTableWidgetItem a("ejemplo"));
//        ui->tblWgtParticleInfo->horizontalHeaderItem(i)->setText(labels.at(i));
    }

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

int MainWindow::computeEuclideanDist(int orgX, int orgY, int destX, int destY)
{
    int distance = sqrt(pow(destX - orgX, 2) + pow(destY - orgY, 2));
    return distance;
}
