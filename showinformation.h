#ifndef SHOWINFORMATION_H
#define SHOWINFORMATION_H

#include <QDialog>
#include <QMap>
#include <QVector>
#include <QDebug>
#include <cmath>

namespace Ui {
class ShowInformation;
}

class ShowInformation : public QDialog
{
    Q_OBJECT

public:
    explicit ShowInformation(QWidget *parent = nullptr, QVector< QMap<QString, int> > particlesInformation = { });
    ~ShowInformation();

private:
    Ui::ShowInformation *ui;
    QVector< QMap<QString, int> > particlesInformation;

    /* Will put the particles information into the text edit. */
    void putParticlesInformation();

    int computeEuclideanDist(int orgX, int orgY, int destX, int destY);
};

#endif // SHOWINFORMATION_H
