#include "simpgaits.h"
#include "ui_simpgaits.h"

SimpGaits::SimpGaits(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SimpGaits)
{
    ui->setupUi(this);
    gaitMode = 1;
    p = new Params;
    resetHome(true);
}

SimpGaits::~SimpGaits()
{
    delete p;
    delete ui;
}

//---------- Setter/Getter ----------//
int SimpGaits::getGaitMode()
{
    return gaitMode;
}

Params* SimpGaits::getParams()
{
    return p;
}

void SimpGaits::setParams(Params* params)
{
    p = params;
    updateBoxes();
}

void SimpGaits::setCoupling(bool check)
{
    ui->ampCoupled->setChecked(check);
    ui->freqCoupled->setChecked(check);
    ui->offsetCoupled->setChecked(check);
    ui->phaseCoupled->setChecked(check);
    ui->wavCoupled->setChecked(check);
}

void SimpGaits::setEnableGaitButtons(bool enable)
{
    ui->homeButton->setEnabled(enable);
    ui->linButton->setEnabled(enable);
    ui->linoffsetButton->setEnabled(enable);
    ui->linhiButton->setEnabled(enable);
    ui->sidewindButton->setEnabled(enable);
    ui->sidewindloButton->setEnabled(enable);
    ui->helrollButton->setEnabled(enable);
    ui->latrollButton->setEnabled(enable);
}

void SimpGaits::updateBoxes()
{
    ui->ampOddBox->setValue(p->amplitudeOdd);
    ui->ampEvenBox->setValue(p->amplitudeEven);
    ui->freqOddBox->setValue(p->frequencyOdd);
    ui->freqEvenBox->setValue(p->frequencyEven);
    ui->phaseOddBox->setValue(p->phaseOdd);
    ui->phaseEvenBox->setValue(p->phaseEven);
    ui->wavOddBox->setValue(p->wavelengthOdd);
    ui->wavEvenBox->setValue(p->wavelengthEven);

    ui->offsetOddBox->setValue(p->masteroffsetOdd);
    ui->offsetEvenBox->setValue(p->masteroffsetEven);
}
//----------------------------------------//

//---------- Home Settings ----------//
void SimpGaits::switch2Home(bool checked)
{
    ui->homeButton->setChecked(1);
    resetHome(checked);
}

void SimpGaits::resetHome(bool checked)
{
    if (checked) {
        p->amplitudeOdd = 0;
        p->frequencyOdd = 0;
        p->phaseOdd = 0;
        p->wavelengthOdd = 8;
        p->masteroffsetOdd = 0;

        p->amplitudeEven = 0;
        p->frequencyEven = 0;
        p->phaseEven = 0;
        p->wavelengthEven = 8;
        p->masteroffsetEven = 0;

        p->manualoffset = 0;
        p->offset = std::vector<double>(16,0);
        p->manualamplitude = 0;
        p->amplitude = std::vector<double>(16,0);

        gaitMode = HOME_MODE;
        setCoupling(0);
        updateBoxes();
    }
}
//----------------------------------------//


//---------- Default Gaits ----------//
void SimpGaits::on_linButton_clicked(bool checked)
{
    if (checked) {
        p->amplitudeOdd = 0;
        p->frequencyOdd = 1;
        p->phaseOdd = 0;
        p->wavelengthOdd = 8;
        p->masteroffsetOdd = 0;

        p->amplitudeEven = (double) 1/6;
        p->frequencyEven = 1;
        p->phaseEven = 0;
        p->wavelengthEven = 8;
        p->masteroffsetEven = 0;

        p->manualoffset = 0;
        p->offset = std::vector<double>(16,0);
        p->manualamplitude = 0;
        p->amplitude = std::vector<double>(16,0);

        gaitMode = LINEAR_MODE;
        setCoupling(0);
        updateBoxes();
    }
}

void SimpGaits::on_linoffsetButton_clicked(bool checked)
{
    if (checked) {

        p->amplitudeOdd = 0;
        p->frequencyOdd = 1;
        p->phaseOdd = 0;
        p->wavelengthOdd = 8;
        p->masteroffsetOdd = (double) 1/18;

        p->amplitudeEven = (double) 1/6;
        p->frequencyEven = 1;
        p->phaseEven = 0;
        p->wavelengthEven = 8;
        p->masteroffsetEven = 0;

        p->manualoffset = 0;
        p->offset = std::vector<double>(16,0);
        p->manualamplitude = 0;
        p->amplitude = std::vector<double>(16,0);

        gaitMode = LINOFFSET_MODE;
        setCoupling(0);
        updateBoxes();
    }
}

void SimpGaits::on_linhiButton_clicked(bool checked)
{
    if (checked) {
        p->amplitudeOdd = 0;
        p->frequencyOdd = 1;
        p->phaseOdd = 0;
        p->wavelengthOdd = 8;
        p->masteroffsetOdd = (double) 1/18;

        p->amplitudeEven = (double) 1/3;
        p->frequencyEven = 1;
        p->phaseEven = 0;
        p->wavelengthEven = 8;
        p->masteroffsetEven = 0;

        p->manualoffset = 0;
        p->offset = std::vector<double>(16,0);
        p->manualamplitude = 0;
        p->amplitude = std::vector<double>(16,0);

        gaitMode = LINHIGH_MODE;
        setCoupling(0);
        updateBoxes();
    }
}

void SimpGaits::on_sidewindButton_clicked(bool checked)
{
    if (checked) {
        p->amplitudeOdd = (double) 1/4.5;
        p->frequencyOdd = 1;
        p->phaseOdd = 0;
        p->wavelengthOdd = 12;
        p->masteroffsetOdd = 0;

        p->amplitudeEven = (double) 1/18;
        p->frequencyEven = 1;
        p->phaseEven = (double) 1/4;
        p->wavelengthEven = 12;
        p->masteroffsetEven = 0;

        p->manualoffset = 0;
        p->offset = std::vector<double>(16,0);
        p->manualamplitude = 0;
        p->amplitude = std::vector<double>(16,0);

        gaitMode = SIDEWIND_MODE;
        setCoupling(0);
        updateBoxes();
    }
}

void SimpGaits::on_sidewindloButton_clicked(bool checked)
{
    if (checked) {
        p->amplitudeOdd = (double) 1/4.5;
        p->frequencyOdd = 1;
        p->phaseOdd = 0;
        p->wavelengthOdd = 6;
        p->masteroffsetOdd = 0;

        p->amplitudeEven = (double) 1/18;
        p->frequencyEven = 1;
        p->phaseEven = (double) 1/4;
        p->wavelengthEven = 6;
        p->masteroffsetEven = 0;

        p->manualoffset = 0;
        p->offset = std::vector<double>(16,0);
        p->manualamplitude = 0;
        p->amplitude = std::vector<double>(16,0);

        gaitMode = SIDEWINDLO_MODE;
        setCoupling(0);
        updateBoxes();
    }
}

void SimpGaits::on_helrollButton_clicked(bool checked)
{
    if (checked) {
        p->amplitudeOdd = (double) 1/3;
        p->frequencyOdd = 1;
        p->phaseOdd = 0;
        p->wavelengthOdd = 64;
        p->masteroffsetOdd = 0;

        p->amplitudeEven = (double) 1/3;
        p->frequencyEven = 1;
        p->phaseEven = (double) 1/2;
        p->wavelengthEven = 64;
        p->masteroffsetEven = 0;

        p->manualoffset = 0;
        p->offset = std::vector<double>(16,0);
        p->manualamplitude = 0;
        p->amplitude = std::vector<double>(16,0);

        gaitMode = HELROLL_MODE;
        setCoupling(0);
        updateBoxes();
    }
}

void SimpGaits::on_latrollButton_clicked(bool checked)
{
    if (checked) {
        p->amplitudeOdd = (double) 1/18;
        p->frequencyOdd = 1;
        p->phaseOdd = 0;
        p->wavelengthOdd = 8000;
        p->masteroffsetOdd = 0;

        p->amplitudeEven = (double) 1/18;
        p->frequencyEven = 1;
        p->phaseEven = (double) 1/2;
        p->wavelengthEven = 8000;
        p->masteroffsetEven = 0;

        p->manualoffset = 0;
        p->offset = std::vector<double>(16,0);
        p->manualamplitude = 0;
        p->amplitude = std::vector<double>(16,0);

        gaitMode = LATROLL_MODE;
        setCoupling(0);
        updateBoxes();
    }
}
//----------------------------------------//


//---------- Update Spinboxes ----------//
void SimpGaits::on_offsetOddBox_valueChanged(double arg1)
{
    p->masteroffsetOdd = arg1;

    if (ui->offsetCoupled->isChecked())
    {
        ui->offsetEvenBox->setValue(arg1);
    }
}


void SimpGaits::on_offsetEvenBox_valueChanged(double arg1)
{
    p->masteroffsetEven = arg1;

    if (ui->offsetCoupled->isChecked())
    {
        ui->offsetOddBox->setValue(arg1);
    }
}

void SimpGaits::on_ampOddBox_valueChanged(double arg1)
{
    p->amplitudeOdd = arg1;

    if (ui->ampCoupled->isChecked())
    {
        ui->ampEvenBox->setValue(arg1);
    }
}

void SimpGaits::on_ampEvenBox_valueChanged(double arg1)
{
    p->amplitudeEven = arg1;

    if (ui->ampCoupled->isChecked())
    {
        ui->ampOddBox->setValue(arg1);
    }
}

void SimpGaits::on_freqOddBox_valueChanged(double arg1)
{
    p->frequencyOdd = arg1;

    if (ui->freqCoupled->isChecked())
    {
        ui->freqEvenBox->setValue(arg1);
    }
}

void SimpGaits::on_freqEvenBox_valueChanged(double arg1)
{
    p->frequencyEven = arg1;

    if (ui->freqCoupled->isChecked())
    {
        ui->freqOddBox->setValue(arg1);
    }
}

void SimpGaits::on_wavOddBox_valueChanged(double arg1)
{
    p->wavelengthOdd = arg1;

    if (ui->wavCoupled->isChecked())
    {
        ui->wavEvenBox->setValue(arg1);
    }
}

void SimpGaits::on_wavEvenBox_valueChanged(double arg1)
{
    p->wavelengthEven = arg1;

    if (ui->wavCoupled->isChecked())
    {
        ui->wavOddBox->setValue(arg1);
    }
}

void SimpGaits::on_phaseOddBox_valueChanged(double arg1)
{
    p->phaseOdd = arg1;

    if (ui->phaseCoupled->isChecked())
    {
        ui->phaseEvenBox->setValue(arg1);
    }
}

void SimpGaits::on_phaseEvenBox_valueChanged(double arg1)
{
    p->phaseEven = arg1;

    if (ui->phaseCoupled->isChecked())
    {
        ui->phaseOddBox->setValue(arg1);
    }
}
//----------------------------------------//


//---------- Invert/Modify buttons ----------//
void SimpGaits::on_freqInvert_clicked()
{
    ui->freqOddBox->setValue(-1*ui->freqOddBox->value());
    ui->freqEvenBox->setValue(-1*ui->freqEvenBox->value());
}


void SimpGaits::on_wavInvert_clicked()
{
    ui->wavOddBox->setValue(-1*ui->wavOddBox->value());
    ui->wavEvenBox->setValue(-1*ui->wavEvenBox->value());
}

void SimpGaits::on_phaseInvert_clicked()
{
    ui->phaseOddBox->setValue(-1*ui->phaseOddBox->value());
    ui->phaseEvenBox->setValue(-1*ui->phaseEvenBox->value());
}

void SimpGaits::on_swopButton_clicked()
{
    double ampEven = ui->ampEvenBox->value();
    double offsetEven = ui->offsetEvenBox->value();
    double wavEven = ui->wavEvenBox->value();
    double freqEven = ui->freqEvenBox->value();
    double phaseEven = ui->phaseEvenBox->value();

    ui->ampEvenBox->setValue(ui->ampOddBox->value());
    ui->offsetEvenBox->setValue(ui->offsetOddBox->value());
    ui->wavEvenBox->setValue(ui->wavOddBox->value());
    ui->freqEvenBox->setValue(ui->freqOddBox->value());
    ui->phaseEvenBox->setValue(ui->phaseOddBox->value());

    ui->ampOddBox->setValue(ampEven);
    ui->offsetOddBox->setValue(offsetEven);
    ui->wavOddBox->setValue(wavEven);
    ui->freqOddBox->setValue(freqEven);
    ui->phaseOddBox->setValue(phaseEven);
}
//----------------------------------------//
