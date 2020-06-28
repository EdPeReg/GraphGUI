#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QDebug>
#include <QMap>
#include <QString>

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
    QVector<QString> lnEdtValues;

    /* Validate that the information entered into the fields are corredct. */
    bool validateLnInput();

    /* Will open a qdialog to show the fields information. */
    void showInformation();

    /* Obtains the information from the fields. */
    void getParticlesInformation();

    /* Clean each field. */
    void cleanFields();
};
#endif // MAINWINDOW_H
