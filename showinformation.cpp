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
    ui->txtEdtParticleInfo->clear();

    QString value;
    int i = 0;
    foreach(const auto &element, particlesInformation) {
        QString aux2 = "Particula: " + QString::number(i + 1);
        ui->txtEdtParticleInfo->append(aux2);
        auto id = element.find("id");
        auto origX = element.find("origen X");
        auto origY = element.find("origen Y");
        auto destX = element.find("destino X");
        auto destY = element.find("destino Y");
        auto velocidad = element.find("velocidad");
        auto red = element.find("R");
        auto green = element.find("G");
        auto blue = element.find("B");
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
