#ifndef SHOWINFORMATION_H
#define SHOWINFORMATION_H

#include <QDialog>

namespace Ui {
class ShowInformation;
}

class ShowInformation : public QDialog
{
    Q_OBJECT

public:
    explicit ShowInformation(QWidget *parent = nullptr);
    ~ShowInformation();

private:
    Ui::ShowInformation *ui;
};

#endif // SHOWINFORMATION_H
