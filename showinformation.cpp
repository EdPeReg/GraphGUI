#include "showinformation.h"
#include "ui_showinformation.h"

ShowInformation::ShowInformation(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShowInformation)
{
    ui->setupUi(this);
}

ShowInformation::~ShowInformation()
{
    delete ui;
}
