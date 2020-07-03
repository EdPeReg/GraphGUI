#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtMath>
#include <QVector>
#include <QMainWindow>
#include <QMessageBox>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QDebug>
#include <QMap>
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
#include <QCloseEvent>

//#include <QWheelEvent>

//#include "particle.h"

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

    QVector<QMap<QString, int>> particles;
    QMap<QString, int> particle;
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

    /* Set up the the table with the proper rows, columns and particle information.
       @particles is the vector with the particles information. */
    void setParticleTable(QVector< QMap<QString, int> > particles);

    /* Will get the particles that the user found by ID.
     * @return A vector that contains all particles found by ID. */
    QVector< QMap<QString, int> > getParticlesByID();

    /* Draw the particles as lines. */
    void drawParticles();

//    void wheelEvent(QWheelEvent *event) ;

    /* Clean each field. */
    void cleanFields();

    /* Compute the distance between two points. */
    double computeEuclideanDist(double orgX, double orgY, double destX, double destY);

};
#endif // MAINWINDOW_H
