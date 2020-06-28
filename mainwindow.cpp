#include "mainwindow.h"
#include "ui_mainwindow.h"

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
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::btnSendPressed() {
    validateLnInput();
    showInformation();
}

void MainWindow::validateLnInput() {
    // Because every line edit starts with those characters.
    QRegularExpression exprLineEdit("(lnEdt)");

    // Find every widget.
    QList<QWidget*> widgets = QObject::findChildren<QWidget *>(exprLineEdit);

    foreach(QWidget *widget, widgets) {
        QRegularExpression regAnyDigit("^[0-9]*$"); 	   // Accepts only numbers, no characters.
        QLineEdit *ln = qobject_cast<QLineEdit *>(widget); // Convert my widget to line edit.

        if(ln->text() != "") {
            QRegularExpressionMatch match = regAnyDigit.match(ln->text());

            if(!match.hasMatch()) {
                QMessageBox::warning(this, "ERROR", "Insert only numbers");
                break;
            }
        }
        // If there is some line edits empty.
        else {
            QMessageBox::warning(this, "ERROR", "Some fields are missing");
            break;
        }
    }
}

void MainWindow::showInformation() {

}
