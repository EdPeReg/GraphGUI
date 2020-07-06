#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;

// TODO
// - Able to do zoom in our graphics view.
// - When there is a bar graph showed, able to hover a bar and say what particle belongs.
// - Able to click in the particle and show what particle belongs in a table.


// CHECK AGAIN CLOSEST PARTICLES IN GRAPH JSON FILES.

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , particle(nullptr)
    , node(nullptr)
    , particles(0)
    , nodes(0)
    , particleExist(false)
    , isBtnShowParticlePressed(false)
    , isAscendingPressed(false)
    , isDescendingPressed(false)
    , isBtnGraphParticlesPressed(false)
    , isBtnBarGraphParticlesPressed(false)
{
    ui->setupUi(this);

    ui->spBxColorR->setMaximum(255);
    ui->spBxColorG->setMaximum(255);
    ui->spBxColorB->setMaximum(255);

    connect(ui->btnSave, &QPushButton::clicked, this, &MainWindow::btnSavePressed);
    connect(ui->btnShow, &QPushButton::clicked, this, &MainWindow::btnShowPressed);
    connect(ui->btnShowParticleTable, &QPushButton::clicked, this, &MainWindow::btnParticleTable);
    connect(ui->btnSearchID, &QPushButton::clicked, this, &MainWindow::btnSearchID);
    connect(ui->btnDraw, &QPushButton::clicked, this, &MainWindow::btnDrawPressed);
    connect(ui->btnClear, &QPushButton::clicked, this, &MainWindow::btnClearPressed);
    connect(ui->action_Open, &QAction::triggered, this, &MainWindow::openJsonFile);
    connect(ui->action_Save, &QAction::triggered, this, &MainWindow::saveJsonFile);
    connect(ui->action_Ascending, &QAction::triggered, this, &MainWindow::sortAscending);
    connect(ui->action_Descending, &QAction::triggered, this, &MainWindow::sortDescending);
    connect(ui->action_Closest_Points, &QAction::triggered, this, &MainWindow::drawClosestPoints);
    connect(ui->tabWidget, &QTabWidget::currentChanged, this, &MainWindow::tabSelected);
}

MainWindow::~MainWindow()
{
    delete ui;

    foreach(auto &particle, particles) {
        delete particle;
    }

    foreach(auto &node, nodes) {
        delete node;
    }
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
        double distance = particle->computeEuclideanDist(particle->getOrigX(),
                                               particle->getOrigY(),
                                               particle->getDestX(),
                                               particle->getDestY());

        QString value = QString::number(particle->getId());
        ui->txtEdtParticleInfo->append("ID : " + value);
        value = QString::number(particle->getOrigX());
        ui->txtEdtParticleInfo->append("Origin X : " + value);
        value = QString::number(particle->getOrigY());
        ui->txtEdtParticleInfo->append("Origin Y : " + value);
        value = QString::number(particle->getDestX());
        ui->txtEdtParticleInfo->append("Destination X : " + value);
        value = QString::number(particle->getDestY());
        ui->txtEdtParticleInfo->append("Destination Y : " + value);
        value = QString::number(particle->getSpeed());
        ui->txtEdtParticleInfo->append("Speed : " + value);
        value = QString::number(particle->getRed());
        ui->txtEdtParticleInfo->append("Color Red : " + value);
        value = QString::number(particle->getGreen());
        ui->txtEdtParticleInfo->append("Color Green : " + value);
        value = QString::number(particle->getBlue());
        ui->txtEdtParticleInfo->append("Color Blue : " + value);
        value = QString::number(distance);
        QString auxStr = "Distancia euclidiana";
        ui->txtEdtParticleInfo->append("Euclidean Distance : " + value);
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
        QVector<Particle *> localParticles = getParticlesByID();

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

void MainWindow::btnClearPressed()
{
    particlesScene->clear();
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
    particle = new Particle;
    particle->setId(ui->lnEdtID->text().toInt());
    particle->setOrigX(ui->lnEdtOrigX->text().toInt());
    particle->setOrigY(ui->lnEdtOrigY->text().toInt());
    particle->setDestX(ui->lnEdtDestX->text().toInt());
    particle->setDestY(ui->lnEdtDestY->text().toInt());
    particle->setSpeed(ui->lnEdtSpeed->text().toInt());
    particle->setRed(ui->spBxColorR->value());
    particle->setGreen(ui->spBxColorG->value());
    particle->setBlue(ui->spBxColorB->value());
    particles.push_back(particle);
}

void MainWindow::setParticlesInformation(const QJsonArray &jsonDocumentArray)
{
    foreach(const QJsonValue &value, jsonDocumentArray) {
        // Remember, each particle needs a single memory allocation.
        particle = new Particle;
        QJsonObject obj = value.toObject();

        particle->setId(obj["id"].toInt());

        QJsonObject obj2 = obj.value("origen").toObject();
        particle->setOrigX(obj2["x"].toInt());
        particle->setOrigY(obj2["y"].toInt());

        obj2 = obj.value("destino").toObject();
        particle->setDestX(obj2["x"].toInt());
        particle->setDestY(obj2["y"].toInt());
        particle->setSpeed(obj["velocidad"].toInt());

        obj2 = obj.value("color").toObject();
        particle->setRed(obj2["red"].toInt());
        particle->setGreen(obj2["green"].toInt());
        particle->setBlue(obj2["blue"].toInt());

        particles.push_back(particle);
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

void MainWindow::sortAscending()
{
   isAscendingPressed = true;
   particle->sortAscending(particles);

}

void MainWindow::sortDescending()
{
    isDescendingPressed = true;
    isAscendingPressed = false;
    particle->sortDescending(particles);
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
        secondaryObject.insert("blue", particle->getBlue());
        secondaryObject.insert("green", particle->getGreen());
        secondaryObject.insert("red", particle->getRed());
        mainObject.insert("color", secondaryObject);
        // Clear object because the previous values still there.
        secondaryObject = QJsonObject();

        secondaryObject.insert("y", particle->getDestY());
        secondaryObject.insert("x", particle->getDestX());
        mainObject.insert("destino", secondaryObject);

        secondaryObject.insert("y", particle->getDestY());
        secondaryObject.insert("x", particle->getDestX());
        mainObject.insert("origen", secondaryObject);
        secondaryObject = QJsonObject();

        mainObject.insert("id", particle->getId());
        mainObject.insert("velocidad", particle->getSpeed());

        jsonArray.insert(i, mainObject);

        i++;
    }

    return jsonArray;
}

void MainWindow::setParticleTable(QVector<Particle *> particles)
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
        double distance = particle->computeEuclideanDist(particle->getOrigX(),
                                               particle->getOrigY(),
                                               particle->getDestX(),
                                               particle->getDestY());

        QString item = QString::number(particle->getId());
        QTableWidgetItem *itemID = new QTableWidgetItem(item);

        item = QString::number(particle->getOrigX());
        QTableWidgetItem *itemOrigX = new QTableWidgetItem(item);
        item = QString::number(particle->getOrigY());
        QTableWidgetItem *itemOrigY = new QTableWidgetItem(item);

        item = QString::number(particle->getDestX());
        QTableWidgetItem *itemDestX = new QTableWidgetItem(item);
        item = QString::number(particle->getDestY());
        QTableWidgetItem *itemDestY = new QTableWidgetItem(item);

        item = QString::number(particle->getSpeed());
        QTableWidgetItem *itemVel = new QTableWidgetItem(item);

        item = QString::number(particle->getRed());
        QTableWidgetItem *itemRed = new QTableWidgetItem(item);
        item = QString::number(particle->getGreen());
        QTableWidgetItem *itemGreen = new QTableWidgetItem(item);
        item = QString::number(particle->getBlue());
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

QVector<Particle *> MainWindow::getParticlesByID()
{
    QVector<Particle *> localParticles;
    Particle *element = new Particle;

    // Get user input.
    QString idStr = ui->lnEdtSearchID->text();
    int userID = idStr.toInt();

    foreach(const auto &particle, particles) {
        if(userID == particle->getId()) {
            particleExist = true;
            element = particle;
            localParticles.push_back(element);
        }
    }

    return localParticles;
}

void MainWindow::drawParticles()
{
    particlesScene = new QGraphicsScene(this);
    particlesScene->clear();
    QPen pen;
    pen.setWidth(2);
    ui->gphViewGraph->setScene(particlesScene);

    QMessageBox msgBox;
    QPushButton *btnParticlesGraph = msgBox.addButton("Particles Graph", QMessageBox::ActionRole);
    QPushButton *btnBarGraph = msgBox.addButton("Bar Graph", QMessageBox::ActionRole);
    QPushButton *btnCancel = msgBox.addButton("Cancel", QMessageBox::RejectRole);

    //  REALLY ?????
    msgBox.setText("                          Drawing option");

    msgBox.exec();
    if(msgBox.clickedButton() == btnParticlesGraph) {
        isBtnGraphParticlesPressed = true;
        isBtnBarGraphParticlesPressed = false;
    } else if(msgBox.clickedButton() == btnBarGraph) {
        isBtnBarGraphParticlesPressed = true;
        isBtnGraphParticlesPressed = false;
    }
    // IS THIS THE ONLY WAY TO MAKE THE WINDOW CLOSE WITH THE X?
    else if(msgBox.clickedButton() == btnCancel) {

    }

    int j = 0;
    foreach(const auto &particle, particles) {
        QColor particleColor;

        if(isBtnGraphParticlesPressed) {
            particleColor.setRgb(particle->getRed(),
                                 particle->getGreen(),
                                 particle->getBlue());
            pen.setColor(particleColor);
            particlesScene->addLine(particle->getOrigX(),
                                    particle->getOrigY(),
                                    particle->getDestX(),
                                    particle->getDestY(), pen);
        } else if(isBtnBarGraphParticlesPressed){
            if(isAscendingPressed) {
                particleColor.setRgb(particle->getRed(),
                             particle->getGreen(),
                             particle->getBlue());
                pen.setColor(particleColor);
                pen.setWidth(4);
                particlesScene->addLine(0, j, particle->getSpeed(), j, pen);
            } else if(isDescendingPressed){
                particleColor.setRgb(particle->getRed(),
                                     particle->getGreen(),
                                     particle->getBlue());
                pen.setColor(particleColor);
                pen.setWidth(4);
                double distance = particle->computeEuclideanDist(particle->getOrigX(),
                                                       particle->getOrigY(),
                                                       particle->getDestX(),
                                                       particle->getDestY());

                particlesScene->addLine(0, j, distance, j, pen);
            } else { // unsorted.
                particleColor.setRgb(particle->getRed(), particle->getRed(), particle->getBlue());
                pen.setColor(particleColor);
                pen.setWidth(4);
                particlesScene->addLine(0, j, particle->getDestX(), j, pen);
            }
        }
        j += 5;
    }
}

// This functions is doing too much maybe? not only draws the closest points, it also
// analize the min distance between the points.
void MainWindow::drawClosestPoints()
{
    QMap<double, Node *> dicOrigOrig;
    QMap<double, Node *> dicOrigDest;
    QMap<double, Node *> dicDestDest;
    QMap<double, Node *> dicDestOrig;

    particlesScene = new QGraphicsScene(this);
    particlesScene->clear();
    QPen pen;
    pen.setWidth(2);
    ui->gphViewGraph->setScene(particlesScene);
    QColor particleColor;

    // Getting all the distance between origin-origin, origin-dest, dest-dest, dest-origin.
    // The nodes will store the points with origin-destination-distance.
    foreach(const auto &rootParticle, particles) {
        foreach(const auto &particle, particles) {
            node = new Node;
            double distOrigOrig = particle->computeEuclideanDist(rootParticle->getOrigX(),
                                                                rootParticle->getOrigY(),
                                                                particle->getOrigX(),
                                                                particle->getOrigY());
            node->origin = rootParticle;
            node->dest = particle;
            node->distance = distOrigOrig;
            dicOrigOrig[distOrigOrig] = node;
            nodes.push_back(node);

            double distOrigDest = particle->computeEuclideanDist(rootParticle->getOrigX(),
                                                          rootParticle->getOrigY(),
                                                          particle->getDestX(),
                                                          particle->getDestY());
            node->origin = rootParticle;
            node->dest = particle;
            node->distance = distOrigDest;
            dicOrigDest[distOrigDest] = node;
            nodes.push_back(node);

            double distDestDest = particle->computeEuclideanDist(rootParticle->getDestX(),
                                                          rootParticle->getDestY(),
                                                          particle->getDestX(),
                                                          particle->getDestY());
            node->origin = rootParticle;
            node->dest = particle;
            node->distance = distDestDest;
            dicDestDest[distDestDest] = node;
            nodes.push_back(node);

            double distDestOrig = particle->computeEuclideanDist(rootParticle->getDestX(),
                                                          rootParticle->getDestY(),
                                                          particle->getOrigX(),
                                                          particle->getOrigY());
            node->origin = rootParticle;
            node->dest = particle;
            node->distance = distDestOrig;
            dicDestOrig[distDestOrig] = node;
            nodes.push_back(node);
        }

        // Delete the distance when the particle it's analize by itself.
        auto pos = dicOrigOrig.find(0);
        dicOrigOrig.erase(pos);
        pos = dicDestDest.find(0);
        dicDestDest.erase(pos);

        // Because we are using a map, is already sorted by key,
        // only get the first position which is the min distance.
        double minOrigOrig = dicOrigOrig.begin().key();
        double minOrigDest = dicOrigDest.begin().key();
        double minDestOrig = dicDestOrig.begin().key();
        double minDestDest = dicDestDest.begin().key();

        // Draw depending of the min distance.
        // Origin-origin.
        if(minOrigOrig < minOrigDest) {
            auto element = dicOrigOrig.find(minOrigOrig);

            particleColor.setRgb(rootParticle->getRed(),
                                 rootParticle->getGreen(),
                                 rootParticle->getBlue());
            pen.setColor(particleColor);

            particlesScene->addRect(rootParticle->getOrigX(),
                                    rootParticle->getOrigY(),
                                    6, 6, pen, particleColor);

            particlesScene->addRect(rootParticle->getDestX(),
                                    rootParticle->getDestY(),
                                    6, 6, pen, particleColor);

            particlesScene->addLine(rootParticle->getOrigX() + 3,
                rootParticle->getOrigY() + 3,
                element.value()->dest->getOrigX() + 3,
                element.value()->dest->getOrigY() + 3, pen);
        }
        // Origin-dest.
        else {
            auto element = dicOrigDest.find(minOrigDest);

            particleColor.setRgb(rootParticle->getRed(),
                                 rootParticle->getGreen(),
                                 rootParticle->getBlue());
            pen.setColor(particleColor);


            particlesScene->addRect(rootParticle->getOrigX(),
                                    rootParticle->getOrigY(),
                                    6, 6, pen, particleColor);

            particlesScene->addRect(rootParticle->getDestX(),
                                    rootParticle->getDestY(),
                                    6, 6, pen, particleColor);

             particlesScene->addLine(rootParticle->getOrigX() + 3,
                        rootParticle->getOrigY() + 3,
                        element.value()->dest->getDestX() + 3,
                        element.value()->dest->getDestY() + 3, pen);
        }

        // Dest-origin.
        if(minDestOrig < minDestDest) {
            auto element = dicDestOrig.find(minDestOrig);

            particleColor.setRgb(rootParticle->getRed(),
                                 rootParticle->getGreen(),
                                 rootParticle->getBlue());
            pen.setColor(particleColor);

            particlesScene->addRect(rootParticle->getOrigX(),
                                    rootParticle->getOrigY(),
                                    6, 6, pen, particleColor);

            particlesScene->addRect(rootParticle->getDestX(),
                                    rootParticle->getDestY(),
                                    6, 6, pen, particleColor);

             particlesScene->addLine(rootParticle->getDestX() + 3,
                rootParticle->getDestY() + 3,
                element.value()->dest->getOrigX() + 3,
                element.value()->dest->getOrigY() + 3, pen);
        }

        // Destino-destino.
        else {
            auto element = dicDestDest.find(minDestDest);

            particleColor.setRgb(rootParticle->getRed(),
                                 rootParticle->getGreen(),
                                 rootParticle->getBlue());
            pen.setColor(particleColor);

            particlesScene->addRect(rootParticle->getOrigX(),
                                    rootParticle->getOrigY(),
                                    6, 6, pen, particleColor);

            particlesScene->addRect(rootParticle->getDestX(),
                                    rootParticle->getDestY(),
                                    6, 6, pen, particleColor);

             particlesScene->addLine(rootParticle->getDestX() + 3,
                                     rootParticle->getDestY() + 3,
                                     element.value()->dest->getDestX() + 3,
                                     element.value()->dest->getDestY() + 3, pen);
        }

        // We don't want the previous information, delete it.
        dicOrigOrig.clear();
        dicOrigDest.clear();
        dicDestOrig.clear();
        dicDestDest.clear();
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

