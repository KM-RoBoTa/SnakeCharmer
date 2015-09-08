#include "advgaits.h"
#include "ui_advgaits.h"

#define MAX_OFFSET_COUNT 16

AdvGaits::AdvGaits(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AdvGaits)
{
    ui->setupUi(this);
    p = new Params;

    resetParams();
    p->frequencyOdd = 0;
    p->frequencyEven = 0;
    p->phaseOdd = 0;
    p->phaseEven = 0;
    p->wavelengthOdd = 0;
    p->wavelengthEven = 0;
}

AdvGaits::~AdvGaits()
{
    delete p;
    delete ui;
}

//---------- Getter/Setter ----------//
Params* AdvGaits::getParams()
{
    p->amplitude = ui->ampSliders->getValues();
    p->amplitudeOdd = ui->ampSliders->getOddMasterValue();
    p->amplitudeEven = ui->ampSliders->getEvenMasterValue();
    p->manualamplitude = !(ui->ampSliders->masterChecked());

    if (ui->offsetTabModes->currentIndex() == 1)
    {
        p->offset = ui->replicaWidget->getOffset();
        p->manualoffset = 1;
        p->masteroffsetOdd = 0;
        p->masteroffsetEven = 0;
    }
    else
    {
        p->offset = ui->offsetSliders->getValues();
        p->masteroffsetOdd = ui->offsetSliders->getOddMasterValue();
        p->masteroffsetEven = ui->offsetSliders->getEvenMasterValue();
        p->manualoffset = !(ui->offsetSliders->masterChecked());
    }
    return p;
}

void AdvGaits::setParams(Params *pvar)
{
    setCoupling(0);

    p = pvar;

    ui->wavOddBox->setValue(pvar->wavelengthOdd);
    ui->wavEvenBox->setValue(pvar->wavelengthEven);
    ui->freqOddBox->setValue(pvar->frequencyOdd);
    ui->freqEvenBox->setValue(pvar->frequencyEven);
    ui->phaseOddBox->setValue(pvar->phaseOdd);
    ui->phaseEvenBox->setValue(pvar->phaseEven);


    if (pvar->manualoffset)
        setOffset(pvar->offset);
    else
        setOffset(pvar->masteroffsetOdd,pvar->masteroffsetEven);

    if (pvar->manualamplitude)
        setAmplitude(pvar->amplitude);
    else
        setAmplitude(pvar->amplitudeOdd,pvar->amplitudeEven);

}

void AdvGaits::resetParams()
{
    setCoupling(0);

    setAmplitude(0,0);
    setOffset(0,0);
    ui->ampSliders->resetSliders();
    ui->offsetSliders->resetSliders();
    ui->wavOddBox->setValue(8);
    ui->wavEvenBox->setValue(8);
    ui->freqOddBox->setValue(0);
    ui->freqEvenBox->setValue(0);
    ui->phaseOddBox->setValue(0);
    ui->phaseEvenBox->setValue(0);
}

void AdvGaits::setNumMotors(int num)
{
    numMotors = num;
    ui->ampSliders->setNumSliders(num);
    ui->offsetSliders->setNumSliders(num);
}

void AdvGaits::setCoupling(bool check)
{
    ui->freqCoupled->setChecked(check);
    ui->phaseCoupled->setChecked(check);
    ui->wavCoupled->setChecked(check);

}

void AdvGaits::setAmplitude(double ampOdd, double ampEven)
{
    ui->ampSliders->setValues(ampOdd,ampEven);
    p->amplitudeOdd = ampOdd;
    p->amplitudeEven = ampEven;
    p->manualamplitude = !(ui->ampSliders->masterChecked());
}

void AdvGaits::setAmplitude(std::vector<double> amps)
{
    ui->ampSliders->setValues(amps);
    p->amplitude = amps;
    p->manualamplitude = !(ui->ampSliders->masterChecked());
}

void AdvGaits::setOffset(double offsetOdd, double offsetEven)
{
    ui->offsetSliders->setValues(offsetOdd,offsetEven);
    p->masteroffsetOdd = offsetOdd;
    p->masteroffsetEven = offsetEven;
    p->manualoffset = !(ui->offsetSliders->masterChecked());
}

void AdvGaits::setOffset(std::vector<double> offsets)
{
    ui->offsetSliders->setValues(offsets);
    p->offset = offsets;
    p->manualoffset = !(ui->offsetSliders->masterChecked());
}
//----------------------------------------//


//---------- Tab Functions ----------//
void AdvGaits::on_offsetTabModes_currentChanged(int index)
{
    ui->replicaWidget->resetReplica();
}

void AdvGaits::activateTabs(bool activate)
{
    int currentAmpIndex = ui->ampTabModes->currentIndex();
    for (int i = 0; i < ui->ampTabModes->count(); i++)
    {
        ui->ampTabModes->setTabEnabled(i, activate || i == currentAmpIndex);
    }
    int currentOffsetIndex = ui->offsetTabModes->currentIndex();
    for (int i = 0; i < ui->offsetTabModes->count(); i++)
    {
        ui->offsetTabModes->setTabEnabled(i, activate || i == currentOffsetIndex);
    }
}
//----------------------------------------//


//---------- Slider Action ----------//
void AdvGaits::on_wavOddSlider_actionTriggered(int action)
{
    int val = ui->wavOddSlider->sliderPosition();
    int wav;
    if (val <= 64)
        wav = val;
    else
        wav = 1 << (val-64+6);

    if (ui->wavOddNeg->isChecked())
        wav = -wav;

    ui->wavOddBox->setValue(wav);
}

void AdvGaits::on_wavEvenSlider_actionTriggered(int action)
{
    int val = ui->wavEvenSlider->sliderPosition();
    int wav;
    if (val <= 64)
        wav = val;
    else
        wav = 1 << (val-64+6);

    if (ui->wavEvenNeg->isChecked())
        wav = -wav;

    ui->wavEvenBox->setValue(wav);
}

void AdvGaits::on_freqOddDial_actionTriggered(int action)
{
    double freq = (double) ui->freqOddDial->sliderPosition()/1000;
    if (ui->freqOddNeg->isChecked())
        freq = -freq;
    ui->freqOddBox->setValue(freq);
}

void AdvGaits::on_freqEvenDial_actionTriggered(int action)
{
    double freq = (double) ui->freqEvenDial->sliderPosition()/1000;
    if (ui->freqEvenNeg->isChecked())
        freq = -freq;
    ui->freqEvenBox->setValue(freq);
}

void AdvGaits::on_phaseOddDial_actionTriggered(int action)
{
    double phase = (double) ui->phaseOddDial->sliderPosition()/1000;
    if (ui->phaseOddNeg->isChecked())
        phase = -phase;
    ui->phaseOddBox->setValue(phase);
}

void AdvGaits::on_phaseEvenDial_actionTriggered(int action)
{
    double phase = (double) ui->phaseEvenDial->sliderPosition()/1000;
    if (ui->phaseEvenNeg->isChecked())
        phase = -phase;
    ui->phaseEvenBox->setValue(phase);
}
//----------------------------------------//


//---------- Box Update ----------//
void AdvGaits::on_wavOddBox_valueChanged(double arg1)
{
    p->wavelengthOdd = arg1;

    int val = arg1 < 0 ? -arg1 : arg1;
    if (val <= 64)
        ;
    else if (val <= 128)
        val = 65;
    else if (val <= 256)
        val = 66;
    else if (val <= 512)
        val = 67;
    else if (val <= 1024)
        val = 68;
    else if (val <= 2048)
        val = 69;
    else if (val <= 4096)
        val = 69;
    else
        val = 71;

    ui->wavOddSlider->setValue(val);
    if (arg1 != 0)
        ui->wavOddNeg->setChecked(arg1 < 0);
    if (ui->wavCoupled->isChecked())
        ui->wavEvenBox->setValue(arg1);
}

void AdvGaits::on_wavEvenBox_valueChanged(double arg1)
{
    p->wavelengthEven = arg1;

    int val = arg1 < 0 ? -arg1 : arg1;
    if (val <= 64)
        ;
    else if (val <= 128)
        val = 65;
    else if (val <= 256)
        val = 66;
    else if (val <= 512)
        val = 67;
    else if (val <= 1024)
        val = 68;
    else if (val <= 2048)
        val = 69;
    else if (val <= 4096)
        val = 69;
    else
        val = 71;

    ui->wavEvenSlider->setValue(val);
    if (arg1 != 0)
        ui->wavEvenNeg->setChecked(arg1 < 0);
    if (ui->wavCoupled->isChecked())
        ui->wavOddBox->setValue(arg1);
}

void AdvGaits::on_freqOddBox_valueChanged(double arg1)
{
    p->frequencyOdd = arg1;
    double val = arg1 < 0 ? -arg1 : arg1;
    ui->freqOddDial->setValue(val*1000);
    if (arg1 != 0)
        ui->freqOddNeg->setChecked(arg1 < 0);
    if (ui->freqCoupled->isChecked())
        ui->freqEvenBox->setValue(arg1);
}

void AdvGaits::on_freqEvenBox_valueChanged(double arg1)
{
    p->frequencyEven = arg1;
    double val = arg1 < 0 ? -arg1 : arg1;
    ui->freqEvenDial->setValue(val*1000);
    if (arg1 != 0)
        ui->freqEvenNeg->setChecked(arg1 < 0);
    if (ui->freqCoupled->isChecked())
        ui->freqOddBox->setValue(arg1);
}

void AdvGaits::on_phaseOddBox_valueChanged(double arg1)
{
    p->phaseOdd = arg1;
    double val = arg1 < 0 ? -arg1 : arg1;
    ui->phaseOddDial->setValue(val*1000);
    if (arg1 != 0)
        ui->phaseOddNeg->setChecked(arg1 < 0);
    if (ui->phaseCoupled->isChecked())
        ui->phaseEvenBox->setValue(arg1);
}

void AdvGaits::on_phaseEvenBox_valueChanged(double arg1)
{
    p->phaseEven = arg1;
    double val = arg1 < 0 ? -arg1 : arg1;
    ui->phaseEvenDial->setValue(val*1000);
    if (arg1 != 0)
        ui->phaseEvenNeg->setChecked(arg1 < 0);
    if (ui->phaseCoupled->isChecked())
        ui->phaseOddBox->setValue(arg1);
}
//----------------------------------------//


//---------- Negative Buttons ----------//
void AdvGaits::on_wavOddNeg_toggled(bool checked)
{
    double val = ui->wavOddBox->value();
    if (checked ^ (val < 0))
        ui->wavOddBox->setValue(-val);
}


void AdvGaits::on_wavEvenNeg_clicked(bool checked)
{
    double val = ui->wavEvenBox->value();
    if (checked ^ (val < 0))
        ui->wavEvenBox->setValue(-val);
}

void AdvGaits::on_freqOddNeg_clicked(bool checked)
{
    double val = ui->freqOddBox->value();
    if (checked ^ (val < 0))
        ui->freqOddBox->setValue(-val);
}

void AdvGaits::on_freqEvenNeg_clicked(bool checked)
{
    double val = ui->freqEvenBox->value();
    if (checked ^ (val < 0))
        ui->freqEvenBox->setValue(-val);
}

void AdvGaits::on_phaseOddNeg_clicked(bool checked)
{
    double val = ui->phaseOddBox->value();
    if (checked ^ (val < 0))
        ui->phaseOddBox->setValue(-val);
}

void AdvGaits::on_phaseEvenNeg_clicked(bool checked)
{
    double val = ui->phaseEvenBox->value();
    if (checked ^ (val < 0))
        ui->phaseEvenBox->setValue(-val);
}
//----------------------------------------//


