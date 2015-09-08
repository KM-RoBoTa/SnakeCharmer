#include <iostream>
#include "dxsystem/setup.h"
#include "dxsystem/dmodeldetails.h"

#include "replica.h"
#include "ui_replica.h"

Replica::Replica(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Replica)
{
    ui->setupUi(this);

    allOffset = vector<double>(16, 0);

    allBoxes.push_back(ui->box1);
    allBoxes.push_back(ui->box2);
    allBoxes.push_back(ui->box3);
    allBoxes.push_back(ui->box4);
    allBoxes.push_back(ui->box5);
    allBoxes.push_back(ui->box6);
    allBoxes.push_back(ui->box7);
    allBoxes.push_back(ui->box8);
    allBoxes.push_back(ui->box9);
    allBoxes.push_back(ui->box10);
    allBoxes.push_back(ui->box11);
    allBoxes.push_back(ui->box12);
    allBoxes.push_back(ui->box13);
    allBoxes.push_back(ui->box14);
    allBoxes.push_back(ui->box15);
    allBoxes.push_back(ui->box16);

    allLabels.push_back(ui->label1);
    allLabels.push_back(ui->label2);
    allLabels.push_back(ui->label3);
    allLabels.push_back(ui->label4);
    allLabels.push_back(ui->label5);
    allLabels.push_back(ui->label6);
    allLabels.push_back(ui->label7);
    allLabels.push_back(ui->label8);
    allLabels.push_back(ui->label9);
    allLabels.push_back(ui->label10);
    allLabels.push_back(ui->label11);
    allLabels.push_back(ui->label12);
    allLabels.push_back(ui->label13);
    allLabels.push_back(ui->label14);
    allLabels.push_back(ui->label15);
    allLabels.push_back(ui->label16);
}

Replica::~Replica()
{
    if (dxInitialized)
        delete d;
    delete ui;
}

void Replica::on_initButton_toggled(bool checked)
{
    if(checked) {
        try{
            d = dxsystemSetup(ui->portnumBox->value());
            dxInitialized = 1;
            ui->portnumBox->setEnabled(0);
            d->setAllTorqueEn(0);
            d->setAllTorqueLimit(vector<int>(d->getNumMotors(),0));
            getOffset();

            unsigned int numMotors = d->getNumMotors();
            for (unsigned int i = 0; i < allBoxes.size(); i++)
            {
                if ( i < numMotors)
                {
                    allBoxes[i]->show();
                    allLabels[i]->show();
                }
                else
                {
                    allBoxes[i]->hide();
                    allLabels[i]->hide();
                }
            }


        } catch(runtime_error e) {
            std::cerr << e.what() << endl;
            dxInitialized = 0;
            ui->initButton->setChecked(0);
        }
    } else {
        if (dxInitialized)
            delete d;
        dxInitialized = 0;
        ui->portnumBox->setEnabled(1);
    }
}

vector<double> Replica::getOffset()
{
    if (!dxInitialized)
        return vector<double>(16,0);

    vector<int> allPos = d->getAllPosition();

    // Note: this function currently causes unpredictable behavior in the GUI (because of rapid sending of data to main GUI)
    // Alternative solution: create another function that receives the offset upon button pressed, send that as offset

    double angle;
    int model;
    for (unsigned int i = 0; i < d->getNumMotors(); i++)
    {
        model = d->getModel(i);
        angle = allPos[i] * (double) modelAngleRange(model)/modelMaxPosition(model) - modelAngleRange(model)/2;
        allOffset[i] = angle/180;
    }
    ui->box1->setValue(allOffset[0]);
    ui->box2->setValue(allOffset[1]);
    ui->box3->setValue(allOffset[2]);
    ui->box4->setValue(allOffset[3]);
    ui->box5->setValue(allOffset[4]);
    ui->box6->setValue(allOffset[5]);
    ui->box7->setValue(allOffset[6]);
    ui->box8->setValue(allOffset[7]);
    ui->box9->setValue(allOffset[8]);
    ui->box10->setValue(allOffset[9]);
    ui->box11->setValue(allOffset[10]);
    ui->box12->setValue(allOffset[11]);
    ui->box13->setValue(allOffset[12]);
    ui->box14->setValue(allOffset[13]);
    ui->box15->setValue(allOffset[14]);
    ui->box16->setValue(allOffset[15]);
    update();
    return allOffset;
}

void Replica::resetReplica()
{
    ui->initButton->setChecked(0);
}
