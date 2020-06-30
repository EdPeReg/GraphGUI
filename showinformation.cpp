#include "showinformation.h"
#include "ui_showinformation.h"

ShowInformation::ShowInformation(QWidget *parent, QVector< QMap<QString, int> > pI) :
    QDialog(parent),
    ui(new Ui::ShowInformation),
    particlesInformation(pI)
{
    ui->setupUi(this);

    putParticlesInformation();
}

ShowInformation::~ShowInformation()
{
    delete ui;
}

void ShowInformation::putParticlesInformation() {
    ui->txtEdtParticleInfo->setReadOnly(true);

    QVector< QMap<QString, int> >::iterator vit;

    QString value;
    int i = 0;

    for(vit = particlesInformation.begin(); vit != particlesInformation.end(); vit++) {
        // THIS IS SO BIG... MAYBE IT CAN BE DONE IN A FOR BUT... IF I DO THAT
        // I WILL NOT APPEND IN THE ORDER I WANT.
        QString aux2 = "Particula: " + QString::number(i + 1);
        ui->txtEdtParticleInfo->append(aux2);
        auto id = particlesInformation.at(i).find("id");
        auto origX = particlesInformation.at(i).find("origen X");
        auto origY = particlesInformation.at(i).find("origen Y");
        auto destX = particlesInformation.at(i).find("destino X");
        auto destY = particlesInformation.at(i).find("destino Y");
        auto velocidad = particlesInformation.at(i).find("velocidad");
        auto red = particlesInformation.at(i).find("R");
        auto green = particlesInformation.at(i).find("G");
        auto blue = particlesInformation.at(i).find("B");
        int distance = computeEuclideanDist(*origX, *origY, *destX, *destY);

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
}

int ShowInformation::computeEuclideanDist(int orgX, int orgY, int destX, int destY)
{
    int distance = sqrt(pow(destX - orgX, 2) + pow(destY - orgY, 2));
    return distance;
}
