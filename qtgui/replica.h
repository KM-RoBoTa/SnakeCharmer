#ifndef REPLICA_H
#define REPLICA_H

#include <QDoubleSpinBox>
#include <QLabel>
#include <QWidget>
#include "dxsystem/dxmotorsystem.h"

namespace Ui {
class Replica;
}

class Replica : public QWidget
{
    Q_OBJECT

public:
    explicit Replica(QWidget *parent = 0);
    ~Replica();
    vector<double> getOffset();
    void resetReplica();

private slots:
    void on_initButton_toggled(bool checked);

private:
    Ui::Replica *ui;
    bool dxInitialized;
    DXMotorSystem* d;
    vector<double> allOffset;

    vector<QDoubleSpinBox*> allBoxes;
    vector<QLabel*> allLabels;
};

#endif // REPLICA_H
