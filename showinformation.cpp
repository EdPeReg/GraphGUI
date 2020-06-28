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
        // 0 Because our first element is a QMap.
        QMap<QString, int> aux = particlesInformation.at(i);
        auto origX = aux.find("origen X");
        auto origY = aux.find("origen Y");
        auto destX = aux.find("destino X");
        auto destY = aux.find("destino Y");
        int distance = computeEuclideanDist(*origX, *origY, *destX, *destY);

        QString aux2 = "Particula: " + QString::number(i + 1);
        ui->txtEdtParticleInfo->append(aux2);

        for(mit = vit->begin(); mit != vit->end(); mit++) {
            value = QString::number(mit.value());
            ui->txtEdtParticleInfo->append(mit.key() + " : " + value);
            value = QString::number(distance);
        }
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
