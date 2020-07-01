#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , particleExist(false)
    , isBtnShowParticlePressed(false)
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
    connect(ui->btnShowParticleTable, &QPushButton::clicked, this, &MainWindow::btnParticleTable);
    connect(ui->btnSearchID, &QPushButton::clicked, this, &MainWindow::btnSearchID);
    connect(ui->btnDraw, &QPushButton::clicked, this, &MainWindow::btnDrawPressed);
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

    // Iterate in each particle, get its information and append that information
    // in a text edit.
    foreach(const auto &particle, particles) {
        QString aux2 = "Particula: " + QString::number(i + 1);
        ui->txtEdtParticleInfo->append(aux2);
        auto id = particle.find("id");
        auto origX = particle.find("origen X");
        auto origY = particle.find("origen Y");
        auto destX = particle.find("destino X");
        auto destY = particle.find("destino Y");
        auto velocidad = particle.find("velocidad");
        auto red = particle.find("R");
        auto green = particle.find("G");
        auto blue = particle.find("B");
        double distance = computeEuclideanDist(*origX, *origY, *destX, *destY);

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

//    ShowInformation showInfoDialog(this, particles);
//    showInfoDialog.setModal(true);
    //    showInfoDialog.exec();
}

void MainWindow::btnParticleTable()
{
    // First receives the global particles.
    setParticleTable(particles);
}

void MainWindow::btnSearchID() {
    if(validateLnInput()) {
        isBtnShowParticlePressed = true;

        // This is false because when particleExist is true, and you do another search
        // If the id doesn't exist, it will remain true, causing that the qmessagebox
        // doesn't appear.
        particleExist = false;
        QVector< QMap<QString, int> > localParticles = getParticlesByID();

        if(!particleExist) {
            QMessageBox::information(this, "Bad luck", "Particle doesn't exist");
        } else {
            setParticleTable(localParticles);
        }
    }
}

void MainWindow::btnDrawPressed()
{
    drawParticles();
}

bool MainWindow::validateLnInput() {
    // Because every line edit starts with those characters.
    QRegularExpression exprLineEdit("(lnEdt)");

    QObjectList widgetList = getWidgetsTab();

    // Obtain line edit information.
    foreach(auto widget, widgetList) {
        QRegularExpression regAnyDigit("^[0-9]*$"); 	   // Accepts only numbers, no characters.
        QLineEdit *ln = qobject_cast<QLineEdit *>(widget); // Convert my widget to line edit.

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

QObjectList MainWindow::getWidgetsTab()
{
    QWidget *tabWidget = new QWidget;
    QObjectList widgetList;

    // Getting the widgets from the current tab.
    if(tabSelected() == TAB_ADD_PARTICLE) {
        widgetList = ui->grpBxAddParticle->children();
    } else if(tabSelected() == TAB_TABLE) {
        tabWidget = ui->tabWidget->widget(ui->tabWidget->currentIndex());
        widgetList = tabWidget->children();
    }

    return widgetList;
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
    particles.push_back(particleInformation);
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

        particles.push_back(particleInformation);
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
    // Clearn our particles because when you open a new file, you don't the previous particles
    // stored in the array, if you don't clean it, the previous particles will be showed
    // in the table.
    particles.clear();

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

    QJsonArray jsonArray = particlesToJsonArray();
    QJsonDocument document(jsonArray);
    file.write(document.toJson());

    file.close();
}

int MainWindow::tabSelected()
{
    switch(ui->tabWidget->currentIndex()) {
        case TAB_ADD_PARTICLE:
            return ui->tabWidget->currentIndex();
        break;

        break;
        case TAB_TABLE:
            return ui->tabWidget->currentIndex();
        break;
    }

    return -1; // ERROR.
}

QJsonArray MainWindow::particlesToJsonArray()
{
    int i = 0;
    QJsonArray jsonArray;
    QJsonObject mainObject;
    QJsonObject secondaryObject;

    foreach(const auto &particle, particles) {
        secondaryObject.insert("blue", particle["B"]);
        secondaryObject.insert("green", particle["G"]);
        secondaryObject.insert("red", particle["R"]);
        mainObject.insert("color", secondaryObject);
        // Clear object because the previous values still there.
        secondaryObject = QJsonObject();

        secondaryObject.insert("y", particle["destino Y"]);
        secondaryObject.insert("x", particle["destino X"]);
        mainObject.insert("destino", secondaryObject);

        secondaryObject.insert("y", particle["origen Y"]);
        secondaryObject.insert("x", particle["origen X"]);
        mainObject.insert("origen", secondaryObject);
        secondaryObject = QJsonObject();

        mainObject.insert("id", particle["id"]);
        mainObject.insert("velocidad", particle["velocidad"]);

        jsonArray.insert(i, mainObject);

        i++;
    }

    return jsonArray;
}

void MainWindow::setParticleTable(QVector< QMap<QString, int> > particles)
{
    // If the button that do the search by ID is not pressed, we are going to use
    // our global particles, not the local particles, remember that local particles
    // are the particles that are stored when you do a search by ID.
    if(!isBtnShowParticlePressed) {
        particles = this->particles;
    }

    QStringList labels = {"ID", "Origin X", "Origin Y", "Destination X", "Destination Y",
                               "Speed", "R", "G", "B", "Distance"};

    int totalColumns = 10; // This also includes eucledian distance.
    ui->tblWgtParticleInfo->setColumnCount(totalColumns);
    ui->tblWgtParticleInfo->setRowCount(particles.size());
    ui->tblWgtParticleInfo->setHorizontalHeaderLabels(labels);

    int row = 0;

    foreach(const auto &particle, particles) {
        // The reason why I did this to find their value, it was because somehow, if I use
        // particle["origen X"] and so on, the values are different, and the distance will
        // be also different, why? I don't know.
        // Doing this, the distance now will be correct.
        auto origX = particle.find("origen X");
        auto origY = particle.find("origen Y");
        auto destX = particle.find("destino X");
        auto destY = particle.find("destino Y");
        double distance = computeEuclideanDist(*origX, *origY, *destX, *destY);

        QString item = QString::number(particle["id"]);
        QTableWidgetItem *itemID = new QTableWidgetItem(item);

        item = QString::number(particle["origen X"]);
        QTableWidgetItem *itemOrigX = new QTableWidgetItem(item);
        item = QString::number(particle["origen Y"]);
        QTableWidgetItem *itemOrigY = new QTableWidgetItem(item);

        item = QString::number(particle["destino X"]);
        QTableWidgetItem *itemDestX = new QTableWidgetItem(item);
        item = QString::number(particle["destino Y"]);
        QTableWidgetItem *itemDestY = new QTableWidgetItem(item);

        item = QString::number(particle["velocidad"]);
        QTableWidgetItem *itemVel = new QTableWidgetItem(item);

        item = QString::number(particle["R"]);
        QTableWidgetItem *itemRed = new QTableWidgetItem(item);
        item = QString::number(particle["G"]);
        QTableWidgetItem *itemGreen = new QTableWidgetItem(item);
        item = QString::number(particle["B"]);
        QTableWidgetItem *itemBlue = new QTableWidgetItem(item);

        item = QString::number(distance);
        QTableWidgetItem *itemDistance = new QTableWidgetItem(item);

        ui->tblWgtParticleInfo->setItem(row, 0, itemID);
        ui->tblWgtParticleInfo->setItem(row, 1, itemOrigX);
        ui->tblWgtParticleInfo->setItem(row, 2, itemOrigY);
        ui->tblWgtParticleInfo->setItem(row, 3, itemDestX);
        ui->tblWgtParticleInfo->setItem(row, 4, itemDestY);
        ui->tblWgtParticleInfo->setItem(row, 5, itemVel);
        ui->tblWgtParticleInfo->setItem(row, 6, itemRed);
        ui->tblWgtParticleInfo->setItem(row, 7, itemGreen);
        ui->tblWgtParticleInfo->setItem(row, 8, itemBlue);
        ui->tblWgtParticleInfo->setItem(row, 9, itemDistance);

        row++;
    }
}

QVector<QMap<QString, int> > MainWindow::getParticlesByID()
{
    QVector< QMap<QString, int> > localParticles;
    QMap<QString, int> element;

    // Get user input.
    QString idStr = ui->lnEdtSearchID->text();
    int userID = idStr.toInt();

    foreach(const auto &particle, particles) {
        auto particleID = particle.find("id");

        if(userID == particleID.value()) {
            particleExist = true;
            element = particle;
            localParticles.push_back(element);
        }
    }

    return localParticles;
}

void MainWindow::drawParticles()
{
    QGraphicsScene *scene = new QGraphicsScene(this);
    QPen pen;
    pen.setWidth(2);
    ui->gphViewGraph->setScene(scene);

    foreach(const auto &particle, particles) {
        QColor particleColor;
        particleColor.setRgb(particle["R"], particle["G"], particle["B"]);
        pen.setColor(particleColor);
        scene->addLine(particle["origen X"], particle["origen Y"],
                       particle["destino X"], particle["destino Y"], pen);
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

double MainWindow::computeEuclideanDist(double orgX, double orgY, double destX, double destY)
{
    double distance = sqrt(pow(destX - orgX, 2) + pow(destY - orgY, 2));
    return distance;
}
