#include <iostream>
#include "dxsystem/setup.h"
#include "dxsystem/additionaltools.h"

#include "controller.h"
#include "ui_controller.h"

Controller::Controller(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Controller)
{
    ui->setupUi(this);
    portNum = 0;
    dxInitialized = 0;
    runFlag = 0;
    timer = new QElapsedTimer();
    msecElapsed = 0;
    activateRunButtons(0);
}

Controller::~Controller()
{
    delete p;
    if (dxInitialized)
        delete d;
    delete ui;
}

//---------- Initializing System ----------//
void Controller::on_portnumBox_valueChanged(int arg1)
{
    portNum = arg1;
}

void Controller::on_initButton_toggled(bool checked)
{
    activateRunButtons(checked);
    if(checked) {
        try{
            d = dxsystemSetup(portNum);
            dxInitialized = 1;
            ui->advgaits->setNumMotors(d->getNumMotors());
            ui->portnumBox->setEnabled(0);
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

//----------------------------------------//


//---------- Activating Widgets ----------//
void Controller::activateRunButtons(bool activate)
{
    ui->startButton->setEnabled(activate);
    ui->pauseButton->setEnabled(activate);
    ui->resetButton->setEnabled(activate);
}

void Controller::activateTabs(bool activate)
{
    ui->initButton->setEnabled(activate);
    int currentIndex = ui->modeTabs->currentIndex();
    for (int i = 0; i < ui->modeTabs->count(); i++)
    {
        ui->modeTabs->setTabEnabled(i, activate || i == currentIndex);
    }
}
//----------------------------------------//


//---------- Running Snake ----------//
#define SLOW_SPEED 100
#define MAX_SPEED 0
void Controller::setInitialPosition()
{
    if (ui->modeTabs->currentIndex() == 0)
    {
        p = ui->simpgaits->getParams();
    }
    else
    {
        p = ui->advgaits->getParams();
    }

    unsigned int numMotors = d->getNumMotors();
    d->setAllSpeed(vector<int>(numMotors,SLOW_SPEED));
    d->setAllPosition(calculatePositions((double)msecElapsed/1000));
    d->getAllPosition(); // status packets
    int checks = 0;
    for (unsigned int ii = 0; ii < numMotors; ii++)
    {
        while(d->isMoving(ii) && checks++ < 10000);
    }
    d->setAllSpeed(vector<int>(numMotors,MAX_SPEED));
}

void Controller::runSnake()
{
    double duration = ui->durationBox->value();
    double currentTime, timeElapsed;
    do
    {
        if (ui->modeTabs->currentIndex() == 0)
        {
            p = ui->simpgaits->getParams();
        }
        else
        {
            p = ui->advgaits->getParams();
        }

        timeElapsed = timer->elapsed();
        currentTime = (double) (timeElapsed + msecElapsed)/1000;
        d->setAllPosition(calculatePositions(currentTime));
        d->getAllPosition(); // status packets
        /* Manipulate position data here (e.g. return/save position, send to visualization) */

    } while(runFlag && (duration == 0 || timeElapsed < duration*1000));
    if (duration != 0)
    {
        ui->pauseButton->setChecked(1);
    }
}

std::vector<int> Controller::calculatePositions(double t)
{   
    double angle, offset, amp, wav;
    vector<int> allPos(d->getNumMotors(),-1);

    for (unsigned int i = 0; i < d->getNumMotors(); i++) {

        if (i%2==0) // n odd (n = i + 1, since i is 0-indexed)
        {
            offset = p->manualoffset ? p->offset[i] : p->masteroffsetOdd;
            amp = p->manualamplitude ? p->amplitude[i] : p->amplitudeOdd;
            wav = p->wavelengthOdd;
            if (p->wavelengthOdd == 0)
                wav = 1;
            angle = (offset + amp*sin(2*PI*(p->frequencyOdd*t + i/wav)))*PI;
        }
        else
        {
            offset = p->manualoffset ? p->offset[i] : p->masteroffsetEven;
            amp = p->manualamplitude ? p->amplitude[i] : p->amplitudeEven;
            wav = p->wavelengthEven;
            if (p->wavelengthEven == 0)
                wav = 1;
            angle = (offset + amp*sin(PI*(2*(p->frequencyEven*t + i/wav) + (p->phaseEven-p->phaseOdd))))*PI;
        }
        if (angle > PI/2)
            angle = PI/2;
        else if (angle < -PI/2)
            angle = -PI/2;
        allPos[i] = angle2Position(angle,d->getModel(i),true);
    }
    return allPos;
}
//----------------------------------------//


//---------- Start/Pause/Reset buttons ----------//
void Controller::on_startButton_toggled(bool checked)
{
    if (checked)
    {
        runFlag = 1;
        if (ui->modeTabs->currentIndex() == 0)
            ui->simpgaits->setEnableGaitButtons(0);
        else if (ui->modeTabs->currentIndex() == 1)
            ui->advgaits->activateTabs(0);
        activateTabs(0);
        setInitialPosition();
        timer->start();
        snakeRunning = std::thread(&Controller::runSnake, this);
    }
}

void Controller::on_pauseButton_toggled(bool checked)
{
    if (runFlag && checked)
    {
        runFlag = 0;
        if (ui->modeTabs->currentIndex() == 0)
            ui->simpgaits->setEnableGaitButtons(1);
        else if (ui->modeTabs->currentIndex() == 1)
            ui->advgaits->activateTabs(1);
        activateTabs(1);
        snakeRunning.join();
        msecElapsed += timer->elapsed();
        timer->invalidate();
    }
}

void Controller::on_resetButton_toggled(bool checked)
{
    if (checked)
    {
        msecElapsed = 0;
        if (runFlag)
        {
            runFlag = 0;
            if (ui->modeTabs->currentIndex() == 0)
                ui->simpgaits->setEnableGaitButtons(1);
            else if (ui->modeTabs->currentIndex() == 1)
                ui->advgaits->activateTabs(1);
            activateTabs(1);
            snakeRunning.join();
            timer->invalidate();
        }
        if (ui->modeTabs->currentIndex() == 0)
            ui->simpgaits->switch2Home(1);
        else
            ui->advgaits->resetParams();
        setInitialPosition();
    }
}
//----------------------------------------//


//---------- Mode toggled (Simp/Adv) ----------//
void Controller::on_modeTabs_currentChanged(int index)
{
    if (index == 1)
    {
        ui->advgaits->setParams(ui->simpgaits->getParams());
    }
    else
    {
        ui->simpgaits->setParams(ui->advgaits->getParams());
    }
}
//----------------------------------------//
