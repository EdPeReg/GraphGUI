#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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

#include "showinformation.h"

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
    void btnShowParticleTable();

    /* Open the user's json file. */
    void openJsonFile();

    /* Saves the particle information into a json file. */
    void saveJsonFile();

    /* To know which tab is selected. */
    void tabSelected();
private:
    Ui::MainWindow *ui;

    enum {
        ADD_PARTICLE,
        TABLE
    };

    QVector<QMap<QString, int>> particlesInformation;
    QMap<QString, int> particleInformation;

    /* Validate the fields information depending of the current tab selected.
       @param tabWidget is the tab selected. */
    bool validateLnInput();

    /* Set the information from the fields. */
    void setParticlesInformation();

    /* Set the information using json information. */
    void setParticlesInformation(const QJsonArray &array);

    /* Read the user's json file.
       @param file is the json file to be readed. */
    void readJsonFile(QFile &file);

    /* Convert particles information into a json array. */
    QJsonArray particlesInformationToJsonArray();

    /* Set up the the table with the proper rows and columns. */
    void setTable();

    /* Clean each field. */
    void cleanFields();

    int computeEuclideanDist(int orgX, int orgY, int destX, int destY);

};
#endif // MAINWINDOW_H
