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
    void btnSendPressed();
    void btnShowPressed();

private:
    Ui::MainWindow *ui;

    QVector<QMap<QString, int>> particlesInformation;
    QMap<QString, int> particleInformation;

    /* Validate that the information entered into the fields are corredct. */
    bool validateLnInput();

    /* Set the information from the fields. */
    void setParticlesInformation();

    /* Set the information using json information. */
    void setParticlesInformation(const QJsonArray &array);

    /* Open the user's json file. */
    void openJsonFile();

    /* Read the user's json file.
       @param file is the json file to be readed. */
    void readJsonFile(QFile &file);

    /* Saves the particle information into a json file. */
    void saveJsonFile();

    /* Clean each field. */
    void cleanFields();

};
#endif // MAINWINDOW_H
