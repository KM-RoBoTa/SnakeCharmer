#ifndef EXTENDEDSLIDERS_H
#define EXTENDEDSLIDERS_H

#include <vector>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QSlider>
#include <QWidget>

namespace Ui {
class ExtendedSliders;
}

class ExtendedSliders : public QWidget
{
    Q_OBJECT

public:
    explicit ExtendedSliders(QWidget *parent = 0);
    ~ExtendedSliders();
    void resetSliders();
    std::vector<double> getValues();
    double getOddMasterValue();
    double getEvenMasterValue();
    bool masterChecked();
    void setValues(const std::vector<double>& input);
    void setValues(double oddVal, double evenVal);
    void setNumSliders(int num);

private slots:
    void on_masterBox_toggled(bool checked);
    void on_safeCheckBox_toggled(bool checked);

    void on_sliderOdd_actionTriggered(int action);
    void on_sliderEven_actionTriggered(int action);
    void on_slider1_actionTriggered(int action);
    void on_slider2_actionTriggered(int action);
    void on_slider3_actionTriggered(int action);
    void on_slider4_actionTriggered(int action);
    void on_slider5_actionTriggered(int action);
    void on_slider6_actionTriggered(int action);
    void on_slider7_actionTriggered(int action);
    void on_slider8_actionTriggered(int action);
    void on_slider9_actionTriggered(int action);
    void on_slider10_actionTriggered(int action);
    void on_slider11_actionTriggered(int action);
    void on_slider12_actionTriggered(int action);
    void on_slider13_actionTriggered(int action);
    void on_slider14_actionTriggered(int action);
    void on_slider15_actionTriggered(int action);
    void on_slider16_actionTriggered(int action);
    void on_rampOddSlider_actionTriggered(int action);
    void on_rampEvenSlider_actionTriggered(int action);

    void on_boxOdd_valueChanged(double arg1);
    void on_boxEven_valueChanged(double arg1);
    void on_box1_valueChanged(double arg1);
    void on_box2_valueChanged(double arg1);
    void on_box3_valueChanged(double arg1);
    void on_box4_valueChanged(double arg1);
    void on_box5_valueChanged(double arg1);
    void on_box6_valueChanged(double arg1);
    void on_box7_valueChanged(double arg1);
    void on_box8_valueChanged(double arg1);
    void on_box9_valueChanged(double arg1);
    void on_box10_valueChanged(double arg1);
    void on_box11_valueChanged(double arg1);
    void on_box12_valueChanged(double arg1);
    void on_box13_valueChanged(double arg1);
    void on_box14_valueChanged(double arg1);
    void on_box15_valueChanged(double arg1);
    void on_box16_valueChanged(double arg1);
    void on_rampOddBox_valueChanged(double arg1);
    void on_rampEvenBox_valueChanged(double arg1);

    void on_constOddButton_toggled(bool checked);
    void on_constEvenButton_toggled(bool checked);

private:
    Ui::ExtendedSliders *ui;
    int numSliders;
    std::vector<double> values;
    std::vector<QSlider*> allSliders;
    std::vector<QDoubleSpinBox*> allBoxes;
    std::vector<QLabel*> allLabels;
    double rampOddOffset;
    double rampEvenOffset;
};

#endif // EXTENDEDSLIDERS_H
