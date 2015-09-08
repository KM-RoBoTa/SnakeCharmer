#ifndef ADVGAITS_H
#define ADVGAITS_H

#include <vector>
#include <QSlider>
#include <QWidget>
#include "params.h"

namespace Ui {
class AdvGaits;
}

class AdvGaits : public QWidget
{
    Q_OBJECT

public:
    explicit AdvGaits(QWidget *parent = 0);
    ~AdvGaits();
    Params* getParams();
    void setParams(Params *p);
    void setNumMotors(int num);
    void activateTabs(bool activate);

public slots:
    void resetParams();

private slots:
    void on_offsetTabModes_currentChanged(int index);

    void on_wavOddSlider_actionTriggered(int action);
    void on_wavEvenSlider_actionTriggered(int action);
    void on_freqOddDial_actionTriggered(int action);
    void on_freqEvenDial_actionTriggered(int action);
    void on_phaseOddDial_actionTriggered(int action);
    void on_phaseEvenDial_actionTriggered(int action);

    void on_wavOddBox_valueChanged(double arg1);
    void on_wavEvenBox_valueChanged(double arg1);
    void on_freqOddBox_valueChanged(double arg1);
    void on_freqEvenBox_valueChanged(double arg1);
    void on_phaseOddBox_valueChanged(double arg1);
    void on_phaseEvenBox_valueChanged(double arg1);

    void on_wavOddNeg_toggled(bool checked);
    void on_wavEvenNeg_clicked(bool checked);
    void on_freqOddNeg_clicked(bool checked);
    void on_freqEvenNeg_clicked(bool checked);
    void on_phaseOddNeg_clicked(bool checked);
    void on_phaseEvenNeg_clicked(bool checked);


private:
    Ui::AdvGaits *ui;
    Params *p;
    std::vector<QSlider*> allSliders;
    unsigned int numMotors;
    void setCoupling(bool check);
    void setAmplitude(double ampOdd, double ampEven);
    void setAmplitude(std::vector<double> amps);
    void setOffset(double offsetOdd, double offsetEven);
    void setOffset(std::vector<double> offsets);
};

#endif // ADVGAITS_H
