#include "extendedsliders.h"
#include "ui_extendedsliders.h"

ExtendedSliders::ExtendedSliders(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ExtendedSliders)
{
    ui->setupUi(this);
    values = std::vector<double>(16,0);

    numSliders = 16;

    allSliders.push_back(ui->slider1);
    allSliders.push_back(ui->slider2);
    allSliders.push_back(ui->slider3);
    allSliders.push_back(ui->slider4);
    allSliders.push_back(ui->slider5);
    allSliders.push_back(ui->slider6);
    allSliders.push_back(ui->slider7);
    allSliders.push_back(ui->slider8);
    allSliders.push_back(ui->slider9);
    allSliders.push_back(ui->slider10);
    allSliders.push_back(ui->slider11);
    allSliders.push_back(ui->slider12);
    allSliders.push_back(ui->slider13);
    allSliders.push_back(ui->slider14);
    allSliders.push_back(ui->slider15);
    allSliders.push_back(ui->slider16);

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

    on_masterBox_toggled(1);
    ui->constOddButton->setChecked(1);
    ui->constEvenButton->setChecked(1);

    rampOddOffset = 0;
    rampEvenOffset = 0;
}

ExtendedSliders::~ExtendedSliders()
{
    delete ui;
}

//---------- Reset ----------//
void ExtendedSliders::resetSliders()
{
    ui->coupledBox->setChecked(0);
    setValues(0,0);
    ui->masterBox->setChecked(1);
    ui->constOddButton->setChecked(1);
    ui->constEvenButton->setChecked(1);
    ui->rampOddBox->setValue(0);
    ui->rampEvenBox->setValue(0);
}

//---------- Getter/Setter ----------//
std::vector<double> ExtendedSliders::getValues()
{
    return values;
}

double ExtendedSliders::getOddMasterValue()
{
    return ui->boxOdd->value();
}

double ExtendedSliders::getEvenMasterValue()
{
    return ui->boxEven->value();
}

bool ExtendedSliders::masterChecked()
{
//    return ui->masterBox->isChecked() && ui->constButton->isChecked();
    return 0;
}

void ExtendedSliders::setValues(const std::vector<double>& input)
{
    ui->masterBox->setChecked(0);
    for (unsigned int i = 0; i < input.size() && i < allSliders.size(); i++)
    {
        allBoxes[i]->setValue(input[i]);
    }
}

void ExtendedSliders::setValues(double oddVal, double evenVal)
{
    ui->masterBox->setChecked(1);
    ui->boxOdd->setValue(oddVal);
    ui->boxEven->setValue(evenVal);
}

void ExtendedSliders::setNumSliders(int num)
{
    numSliders = num;
    for (unsigned int i = 0; i < allSliders.size(); i++)
    {
        if ((int) i < num)
        {
            allBoxes[i]->show();
            allSliders[i]->show();
            allLabels[i]->show();
        }
        else
        {
            allBoxes[i]->hide();
            allSliders[i]->hide();
            allLabels[i]->hide();
        }
    }
}
//----------------------------------------//


//---------- Checkboxes ----------//
void ExtendedSliders::on_masterBox_toggled(bool checked)
{
    for (unsigned int i = 0; i < allSliders.size(); i++)
    {
        allSliders[i]->setEnabled(!checked);
        allBoxes[i]->setEnabled(!checked);
    }
    ui->boxOdd->setEnabled(checked);
    ui->boxEven->setEnabled(checked);
    ui->sliderOdd->setEnabled(checked);
    ui->sliderEven->setEnabled(checked);
    ui->coupledBox->setEnabled(checked);
    if (checked)
    {
        on_boxOdd_valueChanged(ui->boxOdd->value());
        on_boxEven_valueChanged(ui->boxEven->value());
    }
}


void ExtendedSliders::on_safeCheckBox_toggled(bool checked)
{
    if (checked)
    {
        for (unsigned int i = 0; i < allSliders.size(); i++)
        {
            allSliders[i]->setRange(-333,333);
            ui->sliderOdd->setRange(-333,333);
            ui->sliderEven->setRange(-333,333);
            ui->rampOddSlider->setRange(-333,333);
            ui->rampEvenSlider->setRange(-333,333);
            allBoxes[i]->setRange((double)-1/3,(double)1/3);
            ui->boxOdd->setRange((double)-1/3,(double)1/3);
            ui->boxEven->setRange((double)-1/3,(double)1/3);
            ui->rampOddBox->setRange((double)-1/3,(double)1/3);
            ui->rampEvenBox->setRange((double)-1/3,(double)1/3);
        }
    }
    else
    {
        for (unsigned int i = 0; i < allSliders.size(); i++)
        {
            allSliders[i]->setRange(-500,500);
            ui->sliderOdd->setRange(-500,500);
            ui->sliderEven->setRange(-500,500);
            ui->rampOddSlider->setRange(-500,500);
            ui->rampEvenSlider->setRange(-500,500);
            allBoxes[i]->setRange((double)-1/2,(double)1/2);
            ui->boxOdd->setRange((double)-1/2,(double)1/2);
            ui->boxEven->setRange((double)-1/2,(double)1/2);
            ui->rampOddBox->setRange((double)-1/2,(double)1/2);
            ui->rampEvenBox->setRange((double)-1/2,(double)1/2);
        }
    }
}

void ExtendedSliders::on_constOddButton_toggled(bool checked)
{
    ui->rampOddBox->setEnabled(!checked);
    ui->rampOddSlider->setEnabled(!checked);
    if (checked)
    {
        ui->rampOddBox->setValue(0);
    }
}

void ExtendedSliders::on_constEvenButton_toggled(bool checked)
{
    ui->rampEvenBox->setEnabled(!checked);
    ui->rampEvenSlider->setEnabled(!checked);
    if (checked)
    {
        ui->rampEvenBox->setValue(0);
    }
}
//----------------------------------------//


//---------- Slider Action ----------//
void ExtendedSliders::on_sliderOdd_actionTriggered(int action)
{
    ui->boxOdd->setValue((double)ui->sliderOdd->sliderPosition()/1000);
}

void ExtendedSliders::on_sliderEven_actionTriggered(int action)
{
    ui->boxEven->setValue((double)ui->sliderEven->sliderPosition()/1000);
}

void ExtendedSliders::on_slider1_actionTriggered(int action)
{
    ui->box1->setValue((double)ui->slider1->sliderPosition()/1000);
}

void ExtendedSliders::on_slider2_actionTriggered(int action)
{
    ui->box2->setValue((double)ui->slider2->sliderPosition()/1000);
}

void ExtendedSliders::on_slider3_actionTriggered(int action)
{
    ui->box3->setValue((double)ui->slider3->sliderPosition()/1000);
}

void ExtendedSliders::on_slider4_actionTriggered(int action)
{
    ui->box4->setValue((double)ui->slider4->sliderPosition()/1000);
}

void ExtendedSliders::on_slider5_actionTriggered(int action)
{
    ui->box5->setValue((double)ui->slider5->sliderPosition()/1000);
}

void ExtendedSliders::on_slider6_actionTriggered(int action)
{
    ui->box6->setValue((double)ui->slider6->sliderPosition()/1000);
}

void ExtendedSliders::on_slider7_actionTriggered(int action)
{
    ui->box7->setValue((double)ui->slider7->sliderPosition()/1000);
}

void ExtendedSliders::on_slider8_actionTriggered(int action)
{
    ui->box8->setValue((double)ui->slider8->sliderPosition()/1000);
}

void ExtendedSliders::on_slider9_actionTriggered(int action)
{
    ui->box9->setValue((double)ui->slider9->sliderPosition()/1000);
}

void ExtendedSliders::on_slider10_actionTriggered(int action)
{
    ui->box10->setValue((double)ui->slider10->sliderPosition()/1000);
}

void ExtendedSliders::on_slider11_actionTriggered(int action)
{
    ui->box11->setValue((double)ui->slider11->sliderPosition()/1000);
}

void ExtendedSliders::on_slider12_actionTriggered(int action)
{
    ui->box12->setValue((double)ui->slider12->sliderPosition()/1000);
}

void ExtendedSliders::on_slider13_actionTriggered(int action)
{
    ui->box13->setValue((double)ui->slider13->sliderPosition()/1000);
}

void ExtendedSliders::on_slider14_actionTriggered(int action)
{
    ui->box14->setValue((double)ui->slider14->sliderPosition()/1000);
}

void ExtendedSliders::on_slider15_actionTriggered(int action)
{
    ui->box15->setValue((double)ui->slider15->sliderPosition()/1000);
}

void ExtendedSliders::on_slider16_actionTriggered(int action)
{
    ui->box16->setValue((double)ui->slider16->sliderPosition()/1000);
}

void ExtendedSliders::on_rampOddSlider_actionTriggered(int action)
{
    ui->rampOddBox->setValue((double)ui->rampOddSlider->sliderPosition()/1000);
}

void ExtendedSliders::on_rampEvenSlider_actionTriggered(int action)
{
    ui->rampEvenBox->setValue((double)ui->rampEvenSlider->sliderPosition()/1000);
}
//----------------------------------------//


//---------- Box Update ----------//
void ExtendedSliders::on_boxOdd_valueChanged(double arg1)
{
    ui->sliderOdd->setValue(arg1*1000);
    if (ui->masterBox->isChecked())
    {
        double weight, val;
        for (unsigned int i = 0; i < allSliders.size(); i+=2)
        {
            if (ui->constOddButton->isChecked())
                weight = 1;
            else if (ui->linupOddButton->isChecked())
                weight = (double) (i+2)/numSliders;
            else if (ui->lindownOddButton->isChecked())
                weight = 1 - (double) (i)/numSliders;
            val = arg1*weight + rampOddOffset;
            allBoxes[i]->setValue(val);
        }
    }
    if (ui->coupledBox->isChecked())
        ui->boxEven->setValue(arg1);
}

void ExtendedSliders::on_boxEven_valueChanged(double arg1)
{
    ui->sliderEven->setValue(arg1*1000);
    if (ui->masterBox->isChecked())
    {
        double weight, val;
        for (unsigned int i = 1; i < allSliders.size(); i+=2)
        {
            if (ui->constEvenButton->isChecked())
                weight = 1;
            else if (ui->linupEvenButton->isChecked())
                weight = (double) (i+1)/numSliders;
            else if (ui->lindownEvenButton->isChecked())
                weight = 1 - (double) (i)/numSliders;
            val = arg1*weight + rampEvenOffset;
            allBoxes[i]->setValue(val);
        }
    }
    if (ui->coupledBox->isChecked())
        ui->boxOdd->setValue(arg1);
}


void ExtendedSliders::on_box1_valueChanged(double arg1)
{
    values[0] = arg1;
    ui->slider1->setValue(arg1*1000);
}

void ExtendedSliders::on_box2_valueChanged(double arg1)
{
    values[1] = arg1;
    ui->slider2->setValue(arg1*1000);
}

void ExtendedSliders::on_box3_valueChanged(double arg1)
{
    values[2] = arg1;
    ui->slider3->setValue(arg1*1000);
}

void ExtendedSliders::on_box4_valueChanged(double arg1)
{
    values[3] = arg1;
    ui->slider4->setValue(arg1*1000);
}

void ExtendedSliders::on_box5_valueChanged(double arg1)
{
    values[4] = arg1;
    ui->slider5->setValue(arg1*1000);
}

void ExtendedSliders::on_box6_valueChanged(double arg1)
{
    values[5] = arg1;
    ui->slider6->setValue(arg1*1000);
}

void ExtendedSliders::on_box7_valueChanged(double arg1)
{
    values[6] = arg1;
    ui->slider7->setValue(arg1*1000);
}

void ExtendedSliders::on_box8_valueChanged(double arg1)
{
    values[7] = arg1;
    ui->slider8->setValue(arg1*1000);
}

void ExtendedSliders::on_box9_valueChanged(double arg1)
{
    values[8] = arg1;
    ui->slider9->setValue(arg1*1000);
}

void ExtendedSliders::on_box10_valueChanged(double arg1)
{
    values[9] = arg1;
    ui->slider10->setValue(arg1*1000);
}

void ExtendedSliders::on_box11_valueChanged(double arg1)
{
    values[10] = arg1;
    ui->slider11->setValue(arg1*1000);
}

void ExtendedSliders::on_box12_valueChanged(double arg1)
{
    values[11] = arg1;
    ui->slider12->setValue(arg1*1000);
}

void ExtendedSliders::on_box13_valueChanged(double arg1)
{
    values[12] = arg1;
    ui->slider13->setValue(arg1*1000);
}

void ExtendedSliders::on_box14_valueChanged(double arg1)
{
    values[13] = arg1;
    ui->slider14->setValue(arg1*1000);
}

void ExtendedSliders::on_box15_valueChanged(double arg1)
{
    values[14] = arg1;
    ui->slider15->setValue(arg1*1000);
}

void ExtendedSliders::on_box16_valueChanged(double arg1)
{
    values[15] = arg1;
    ui->slider16->setValue(arg1*1000);
}

void ExtendedSliders::on_rampOddBox_valueChanged(double arg1)
{
    rampOddOffset = arg1;
    ui->rampOddSlider->setValue(arg1*1000);
    on_boxOdd_valueChanged(ui->boxOdd->value());
}

void ExtendedSliders::on_rampEvenBox_valueChanged(double arg1)
{
    rampEvenOffset = arg1;
    ui->rampEvenSlider->setValue(arg1*1000);
    on_boxEven_valueChanged(ui->boxEven->value());
}
//----------------------------------------//

