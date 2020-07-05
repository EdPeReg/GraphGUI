#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QVector>
#include <QMainWindow>
#include <QMessageBox>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QDebug>
#include <QString>
#include <QFile>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <algorithm>
#include <QPushButton>

#include <QPair>
#include <QCloseEvent>
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>


//#include <QWheelEvent>

#include "particle.h"

struct Node {
    Particle * origin;
    Particle *dest;
    double distance;

    Node() { }

    Node(Particle *orig, Particle *d, double dist) : origin(orig), dest(d), distance(dist) { }

    ~Node() {
        delete origin;
        delete dest;
    }
};

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void btnSavePressed();
    void btnShowPressed();
    void btnParticleTable();
    void btnSearchID();
    void btnDrawPressed();
    void btnClearPressed();

private:
    Ui::MainWindow *ui;

    enum {
        TAB_ADD_PARTICLE,
        TAB_TABLE
    };

    Particle *particle;
    Node *node;
    QVector<Particle *> particles;
    QVector<Node *> nodes;

    QGraphicsScene *particlesScene;
    bool particleExist;
    bool isBtnShowParticlePressed;
    bool isAscendingPressed;
    bool isDescendingPressed;
    bool isBtnGraphParticlesPressed;
    bool isBtnBarGraphParticlesPressed;

    /* Open the user's json file. */
    void openJsonFile();

    /* Saves the particle information into a json file. */
    void saveJsonFile();

    /* Sort the particles in ascending order, using the speed. */
    void sortAscending();

    /* Sort the particles in descending order, using the distance. */
    void sortDescending();

    /* To know which tab is selected.
     * @return the index of the tab selected. */
    int tabSelected();

    /* Validate the fields information depending of the current tab selected. */
    bool validateLnInput();

    /* Get the widgets from the current tab.
     * @return A widget list from the current tab. */
    QObjectList getWidgetsTab();

    /* Set the information from the fields. */
    void setParticlesInformation();

    /* Set the information using json information. */
    void setParticlesInformation(const QJsonArray &array);

    /* Read the user's json file.
       @param file is the json file to be readed. */
    void readJsonFile(QFile &file);

    /* Convert particles information into a json array.
     * @return A json array with the particles information. */
    QJsonArray particlesToJsonArray();

    // TODO
    /* Set up the the table with the proper rows, columns and particle information.
       @param is the vector with the particles information. */
    void setParticleTable(QVector<Particle *> particles);

    // TODO
    /* Will get the particles that the user found by ID.
     * @return A vector that contains all particles found by ID. */
    QVector<Particle *> getParticlesByID();

    /* Draw the particles as lines. */
    void drawParticles();

    void drawClosesParticles();

//    void mouseMoveEvent(QMouseEvent  *event) override;

////    void wheelEvent(QWheelEvent *event) ;

    /* Clean each field. */
    void cleanFields();
};
#endif // MAINWINDOW_H
