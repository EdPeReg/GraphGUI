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

    // WILL GO TO EACH INDEX, 0...n
    // NOW HOE CAN I OBTAIN EACH KEY AND VALUE DE PUTAZO?
    foreach(const QJsonValue &value, jsonDocumentArray) {
        QJsonObject obj = value.toObject();
        QJsonObject obj2 = obj["origen"].toObject(); // ACCECING TO MY KEY
        qDebug() << obj2["x"].toInt() << "\n"; // WILL SHOW ALL THE COLOR VALUES.
    }
}

void MainWindow::saveJsonFile()
{
//    QString filename = QFileDialog::getSaveFileName(this, "Open file", "./", "JSON (*.json)");
//    QFile file(filename);

//    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
//        QMessageBox::critical(this, "Error", "Cannot save file");
//    }

//    QVector< QMap<QString, int> >::iterator vit;
//    QMap<QString, int>::iterator mit;

//    QString key;
//    int value;
//    for(vit = particlesInformation.begin(); vit != particlesInformation.end(); vit++) {
//        QJsonArray array;
//        for(mit = vit->begin(); mit != vit->end(); mit++) {
//            QJsonObject jsonObject;
//            key = mit.key();
//            value = mit.value();
//        }
//    }
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


