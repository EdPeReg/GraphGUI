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
    QMap<QString, int>::iterator mit;

    QString value;
    int i = 0;
    for(vit = particlesInformation.begin(); vit != particlesInformation.end(); vit++) {
        auto origX = particlesInformation.at(i).find("origen X");
        auto origY = particlesInformation.at(i).find("origen Y");
        auto destX = particlesInformation.at(i).find("destino X");
        auto destY = particlesInformation.at(i).find("destino Y");
        int distance = computeEuclideanDist(*origX, *origY, *destX, *destY);

        QString aux2 = "Particula: " + QString::number(i + 1);
        ui->txtEdtParticleInfo->append(aux2);

        for(mit = vit->begin(); mit != vit->end(); mit++) {
            value = QString::number(mit.value());
            ui->txtEdtParticleInfo->append(mit.key() + " : " + value);
        }
        value = QString::number(distance);
        QString auxStr = "Distancia euclidiana";
        ui->txtEdtParticleInfo->append(auxStr + " : " + value);
        i++;
        ui->txtEdtParticleInfo->append("\n");
    }
}

int ShowInformation::computeEuclideanDist(int orgX, int orgY, int destX, int destY)
{
    int distance = sqrt(pow(destX - orgX, 2) + pow(destY - orgY, 2));
    return distance;
}
