#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <thread>
#include <QElapsedTimer>
#include <QWidget>
#include "params.h"
#include "dxsystem/dxmotorsystem.h"

namespace Ui {
class Controller;
}

class Controller : public QWidget
{
    Q_OBJECT

public:
    explicit Controller(QWidget *parent = 0);
    ~Controller();    

private slots:
    void on_initButton_toggled(bool checked);
    void on_portnumBox_valueChanged(int arg1);

    void on_modeTabs_currentChanged(int index);

    void on_startButton_toggled(bool checked);
    void on_pauseButton_toggled(bool checked);
    void on_resetButton_toggled(bool checked);

private:
    Ui::Controller *ui;
    DXMotorSystem *d;
    bool dxInitialized;
    int portNum;
    bool runFlag;
    QElapsedTimer *timer;
    Params *p;
    long msecElapsed;
    std::thread snakeRunning;

    void resetParamPair();
    void activateRunButtons(bool activate);
    void activateTabs(bool activate);

    void setInitialPosition();
    std::vector<int> calculatePositions(double t);
    void runSnake();

};

#endif // CONTROLLER_H
