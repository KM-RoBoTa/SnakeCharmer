#ifndef SIMPGAITS_H
#define SIMPGAITS_H

#include <QWidget>
#include "params.h"

// Gait Modes
#define HOME_MODE       1
#define LINEAR_MODE     2
#define LINOFFSET_MODE  3
#define LINHIGH_MODE    4
#define SIDEWIND_MODE   5
#define SIDEWINDLO_MODE 6
#define LATROLL_MODE    7
#define HELROLL_MODE    8
#define ADV_MODE        0

namespace Ui {
class SimpGaits;
}

class SimpGaits : public QWidget
{
    Q_OBJECT

public:
    explicit SimpGaits(QWidget *parent = 0);
    ~SimpGaits();
    Params* getParams();
    void setParams(Params* params);
    int getGaitMode();
    void setEnableGaitButtons(bool enable);
    void switch2Home(bool checked);

private slots:
    void resetHome(bool checked);

    void on_linButton_clicked(bool checked);
    void on_linoffsetButton_clicked(bool checked);
    void on_linhiButton_clicked(bool checked);
    void on_sidewindButton_clicked(bool checked);
    void on_sidewindloButton_clicked(bool checked);
    void on_helrollButton_clicked(bool checked);
    void on_latrollButton_clicked(bool checked);

    void on_offsetOddBox_valueChanged(double arg1);
    void on_offsetEvenBox_valueChanged(double arg1);
    void on_ampOddBox_valueChanged(double arg1);
    void on_ampEvenBox_valueChanged(double arg1);
    void on_freqOddBox_valueChanged(double arg1);
    void on_freqEvenBox_valueChanged(double arg1);
    void on_wavOddBox_valueChanged(double arg1);
    void on_wavEvenBox_valueChanged(double arg1);
    void on_phaseOddBox_valueChanged(double arg1);
    void on_phaseEvenBox_valueChanged(double arg1);

    void on_freqInvert_clicked();
    void on_wavInvert_clicked();
    void on_phaseInvert_clicked();
    void on_swopButton_clicked();

private:
    Ui::SimpGaits *ui;
    Params *p;
    int gaitMode;
    void updateBoxes();
    void setCoupling(bool check);
};

#endif // SIMPGAITS_H
