#ifndef SIMPLEGAITS_H
#define SIMPLEGAITS_H

#include <QMainWindow>
//#include <about.h>

namespace Ui {
    class SimpleGaits;
}

class SimpleGaits : public QWidget
{
    Q_OBJECT

public:
    explicit SimpleGaits(QWidget *parent = 0);
    ~SimpleGaits();
    int q1(),q2(),q3(),q4(),q5(),q6(),q7(),q8(),q9(),q10(),q11(),q12(),q13(),q14(),q15(),q16(),torque(),velocidad();

    void set_amplitud_par(int val),     set_amplitud_impar(int val),    set_offset_par(int val),    set_desfase(int val);
    void set_offset_impar(int val),     set_dtheta_dn(int val),         set_dtheta_dt(int val),     set_tiempo(int val);
    int get_amplitud_par(),             get_amplitud_impar(),           get_offset_par(),           get_offset_impar();
    int get_dtheta_dn(),                get_dtheta_dt(),                get_desfase(),              get_tiempo();
    int mueve();

private:
    Ui::SimpleGaits *ui;
//    about *theabout;
    int amplitud_par,amplitud_impar,offset_par,offset_impar,dtheta_dn,dtheta_dt,desfase,tiempo;

private slots:
    void abrir_about();
    void mover();
    void linear();
    void linear_high();
    void linear_offset();
    void rolling();
    void sidewinding();
    void sidewinding_low();
    void home();
    void helix_rolling();

};

#endif // SIMPLEGAITS_H
