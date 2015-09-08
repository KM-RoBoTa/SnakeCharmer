//inclusión de headers y librerias
#include "simplegaits.h"
#include "ui_simplegaits.h"
//#include "dynamixel.h"
#include <math.h>
#include <QStringBuilder>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>

// Elementos de la tabla de control del dynamixel AX-12
//#define P_GOAL_POSITION_L	30 //byte bajo de la posición objetivo
//#define P_GOAL_POSITION_H	31 //byte alto de la posición objetivo
//#define P_PRESENT_SPEED         38 //byte bajo de la velocidad
//#define P_LOAD_H                41 //byte alto del torque
//#define P_LOAD_L                40 //byte bajo del torque
// Ajustes por defecto
//#define DEFAULT_BAUDNUM		1  // Velocidad de transmisión: 1Mbps
//#define NUM_ACTUATOR		16 // Número de actuadores
//#define MUESTREO                10 // Número de muestras por segundo
//Variables globales
int direccion,torque_e,velocidad_d;
int MOD_ACTIVOS [16]={0};


//funciones
double timeval_diff(struct timeval *a, struct timeval *b);
int hallar_torque(int torque);
float Choset(int amplitud_par,int amplitud_impar, int desfase, int dtheta_dn, int dtheta_dt,int t_time,int n,int offset_par,int offset_impar);


//Clase donde se construyen las aplicaciones
SimpleGaits::SimpleGaits(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SimpleGaits)
{
    ui->setupUi(this);
}

SimpleGaits::~SimpleGaits()
{
    delete ui;
}

//Llamado a las casillas de verificación de la interfaz. Indican si se desea que el módulo, de número específico, se quiere activo
//o inactivo. Este valor (activo->1, inactivo->0) es colocado en el arreglo (variable global) MOD_ACTIVOS.
//Adicionalmente, se verifica con la casilla de verificación si se desea guardar en un archivo externo o no tanto el torque de los
//servomotores como su velocidad. Estos volores son guadados en las variables globales torque_e y velocidad_d
int SimpleGaits::q1()   {if(ui->q1->checkState()==Qt::Checked)      return MOD_ACTIVOS[0]=1;     else  return MOD_ACTIVOS[0]=0;}
int SimpleGaits::q2()   {if(ui->q2->checkState()==Qt::Checked)      return MOD_ACTIVOS[1]=1;     else  return MOD_ACTIVOS[1]=0;}
int SimpleGaits::q3()   {if(ui->q3->checkState()==Qt::Checked)      return MOD_ACTIVOS[2]=1;     else  return MOD_ACTIVOS[2]=0;}
int SimpleGaits::q4()   {if(ui->q4->checkState()==Qt::Checked)      return MOD_ACTIVOS[3]=1;     else  return MOD_ACTIVOS[3]=0;}
int SimpleGaits::q5()   {if(ui->q5->checkState()==Qt::Checked)      return MOD_ACTIVOS[4]=1;     else  return MOD_ACTIVOS[4]=0;}
int SimpleGaits::q6()   {if(ui->q6->checkState()==Qt::Checked)      return MOD_ACTIVOS[5]=1;     else  return MOD_ACTIVOS[5]=0;}
int SimpleGaits::q7()   {if(ui->q7->checkState()==Qt::Checked)      return MOD_ACTIVOS[6]=1;     else  return MOD_ACTIVOS[6]=0;}
int SimpleGaits::q8()   {if(ui->q8->checkState()==Qt::Checked)      return MOD_ACTIVOS[7]=1;     else  return MOD_ACTIVOS[7]=0;}
int SimpleGaits::q9()   {if(ui->q9->checkState()==Qt::Checked)      return MOD_ACTIVOS[8]=1;     else  return MOD_ACTIVOS[8]=0;}
int SimpleGaits::q10()  {if(ui->q10->checkState()==Qt::Checked)     return MOD_ACTIVOS[9]=1;     else  return MOD_ACTIVOS[9]=0;}
int SimpleGaits::q11()  {if(ui->q11->checkState()==Qt::Checked)     return MOD_ACTIVOS[10]=1;    else  return MOD_ACTIVOS[10]=0;}
int SimpleGaits::q12()  {if(ui->q12->checkState()==Qt::Checked)     return MOD_ACTIVOS[11]=1;    else  return MOD_ACTIVOS[11]=0;}
int SimpleGaits::q13()  {if(ui->q13->checkState()==Qt::Checked)     return MOD_ACTIVOS[12]=1;    else  return MOD_ACTIVOS[12]=0;}
int SimpleGaits::q14()  {if(ui->q14->checkState()==Qt::Checked)     return MOD_ACTIVOS[13]=1;    else  return MOD_ACTIVOS[13]=0;}
int SimpleGaits::q15()  {if(ui->q15->checkState()==Qt::Checked)     return MOD_ACTIVOS[14]=1;    else  return MOD_ACTIVOS[14]=0;}
int SimpleGaits::q16()  {if(ui->q16->checkState()==Qt::Checked)     return MOD_ACTIVOS[15]=1;    else  return MOD_ACTIVOS[15]=0;}
int SimpleGaits::torque()   {if(ui->torque->checkState()==Qt::Checked)      return torque_e=1;     else  return torque_e=0;}
int SimpleGaits::velocidad()   {if(ui->velocidad->checkState()==Qt::Checked)      return velocidad_d=1;     else  return velocidad_d=0;}

//Llamado a los valores del cuadro de números de la interfaz. Estos valores pertenecen a las variables que componen la ecuación que
//da por resultado el ángulo al cual se debe mover cada servomotor (Adicional se tiene la variable tiempo, indica los segundos deseados
//para la ejecución del gait). Estos valores se asignan a variables (set_variable) y son tomados desde la interfaz (get_variable)
void SimpleGaits::set_amplitud_par(int val)      {SimpleGaits::amplitud_par=val;  }
void SimpleGaits::set_amplitud_impar(int val)    {SimpleGaits::amplitud_impar=val;}
void SimpleGaits::set_offset_par(int val)        {SimpleGaits::offset_par=val;    }
void SimpleGaits::set_offset_impar(int val)      {SimpleGaits::offset_impar=val;  }
void SimpleGaits::set_dtheta_dn(int val)         {SimpleGaits::dtheta_dn=val;     }
void SimpleGaits::set_dtheta_dt(int val)         {SimpleGaits::dtheta_dt=val;     }
void SimpleGaits::set_desfase(int val)           {SimpleGaits::desfase=val;       }
void SimpleGaits::set_tiempo(int val)            {SimpleGaits::tiempo=val;        }

int SimpleGaits::get_amplitud_par()              {return SimpleGaits::amplitud_par;   }
int SimpleGaits::get_amplitud_impar()            {return SimpleGaits::amplitud_impar; }
int SimpleGaits::get_offset_par()                {return SimpleGaits::offset_par;     }
int SimpleGaits::get_offset_impar()              {return SimpleGaits::offset_impar;   }
int SimpleGaits::get_dtheta_dn()                 {return SimpleGaits::dtheta_dn;      }
int SimpleGaits::get_dtheta_dt()                 {return SimpleGaits::dtheta_dt;      }
int SimpleGaits::get_desfase()                   {return SimpleGaits::desfase;        }
int SimpleGaits::get_tiempo()                    {return SimpleGaits::tiempo;         }

//Una vez se oprime el botón de ejecución del gait se procede a la ejecución de la siguiente rutina:
void SimpleGaits::mover(){
   //variables locales
    int id[NUM_ACTUATOR];
    int baudnum = 1;
    int deviceIndex = 0;
    int i,goalpos,muestra,t_time,present_vel;
    float angulo;
    struct timeval t_ini, t_fin;

    //Llamado a las variables (con el valor actual puesto en la interfaz) que conforman la ecuación para el ángulo del servomotor y a
    //las funciones implementadas previamente (variables de la ecuación y módulos activos)
    int amplitud_par = ui->amplitud_par->text().toInt();        int offset_par= ui->offset_par->text().toInt();
    int amplitud_impar= ui->amplitud_impar->text().toInt();     int offset_impar= ui->offset_impar->text().toInt();
    int dtheta_dn= ui->dtheta_dn->text().toInt();               int desfase= ui->desfase->text().toInt();
    int dtheta_dt= ui->dtheta_dt->text().toInt();               int tiempo= ui->tiempo->text().toInt();

    SimpleGaits::set_amplitud_par(amplitud_par); SimpleGaits::set_amplitud_impar(amplitud_impar);
    SimpleGaits::set_offset_par(offset_par);     SimpleGaits::set_offset_impar(offset_impar);
    SimpleGaits::set_dtheta_dn(dtheta_dn);       SimpleGaits::set_dtheta_dt(dtheta_dt);
    SimpleGaits::set_desfase(desfase);           SimpleGaits::set_tiempo(tiempo);
    SimpleGaits::q1();    SimpleGaits::q2();      SimpleGaits::q3();    SimpleGaits::q4();    SimpleGaits::q5();    SimpleGaits::q6();
    SimpleGaits::q7();    SimpleGaits::q8();      SimpleGaits::q9();    SimpleGaits::q10();   SimpleGaits::q11();   SimpleGaits::q12();
    SimpleGaits::q13();   SimpleGaits::q14();     SimpleGaits::q15();   SimpleGaits::q16();   SimpleGaits::torque();SimpleGaits::velocidad();

    //El vector id se enumera del 1 al número de servomotores
    for( i=0; i<(NUM_ACTUATOR ); i++ )
           id[i] = i+1;

    if( dxl_initialize(deviceIndex, baudnum) == 1 ){ //Se abre el puerto para proceder al envío de paquetes

        //Una vez seleccionado un tiempo desde la interfaz, se coge este número para evaluar las posiciones de los servomotores dentro de
        //cada segundo de tiempo seleccionado
        for(t_time=1;t_time<(tiempo+1);t_time++){
            //Se recorre el número de muestras predeterminado en las variables globales (indica el número de veces por segundo que se
            //le enviará un comando a cada servomotor para moverse)
            for( muestra=0; muestra<MUESTREO; muestra++ ){

                gettimeofday(&t_ini, NULL); //función para iniciar un conteo, se busca que la instrucción no se demore más de 100 ms

                    // Se arma el paquete a enviar al servomotor. Es el envío del comando de posición
                    dxl_set_txpacket_id(BROADCAST_ID);
                    dxl_set_txpacket_instruction(INST_SYNC_WRITE);
                    dxl_set_txpacket_parameter(0, P_GOAL_POSITION_L); // se especifica el parámetro a escribir, en este caso: posición
                    dxl_set_txpacket_parameter(1, 2);
                    for( i=0; i<NUM_ACTUATOR; i++ )//Se recorre el número de servomotores
                    {
                        //Se calcula el ángulo para el número de servomotor en el tiempo determinado
                        angulo= Choset(amplitud_par,amplitud_impar,  desfase,  dtheta_dn,  dtheta_dt,(muestra+(10*(t_time-1))),i,offset_par,offset_impar);
                        //Si el módulo se seleccionó como inactivo, el ángulo se vuelve cero.
                        angulo*=MOD_ACTIVOS[i];
                        goalpos=512-angulo*3.41;
                        dxl_set_txpacket_parameter(2+3*i, id[i]);
                        dxl_set_txpacket_parameter(2+3*i+1, dxl_get_lowbyte(goalpos));
                        dxl_set_txpacket_parameter(2+3*i+2, dxl_get_highbyte(goalpos));

                    }
                    dxl_set_txpacket_length((2+1)*NUM_ACTUATOR+4);
                    //se transmite el paquete
                    dxl_txrx_packet();

                    //Si el usuario seleccionó adquisición de algún dato se procede:
                    if(torque_e | velocidad_d){
                       char ofilename[] = "datos_guardados.txt";       //fichero a editar
                       FILE *ofp;                                      //Declara apuntadores a archivo
                       ofp = fopen(ofilename,"at");                    //Abre archivo de salida para continuar su escritura
                       for( i=0; i<NUM_ACTUATOR; i++ )                 //Se recorre el número de servomotores
                          {
                          if(torque_e & !velocidad_d)
                             {                                         //Si se desea guadar únicamente el torque:
                             int torque_total=dxl_read_word( id[i], P_LOAD_L ); //Se lee el valor del torque
                             torque_total=hallar_torque(torque_total); // Se halla el torque total usando la función hallar_torque
                             fprintf(ofp,"%d %d %d\n",id[i],direccion, torque_total); // Escribe los datos de id del servomotor, dirección del torque y torque
                             }
                          if(!torque_e & velocidad_d)
                             {                                          //Si se desea guadar únicamente la velocidad:
                             present_vel = dxl_read_word( id[i], P_PRESENT_SPEED );//se lee el valor de la velocidad
                             fprintf(ofp,"%d %d\n",id[i], present_vel); // Escribe los datos del id del servomotor y velocidad
                             }
                          if(torque_e & velocidad_d)
                             {                                          //Si se desea guadar el torque y la velocidad:
                             // se arma un paquete de estados para leer más de un dato del servomotor
                             dxl_set_txpacket_id( id[i] );
                             dxl_set_txpacket_instruction( 0x02 );      //Se especifica la función leer
                             dxl_set_txpacket_parameter( 0, P_PRESENT_SPEED );//Se especifica que se desea la adquisición del parámetro de velocidad
                             dxl_set_txpacket_parameter( 1, 6 );        //Se especifica que se desea la adquisición del parámetro de torque
                             dxl_set_txpacket_length( 4 );
                             dxl_txrx_packet();                         //Se trasnmite el paquete de estados

                             //Se guarda el resultado obtenido del parámetro velocidad y torque
                             present_vel= dxl_makeword(dxl_get_rxpacket_parameter(0),dxl_get_rxpacket_parameter(1));                             
                             int torque_total = dxl_makeword(dxl_get_rxpacket_parameter(2),dxl_get_rxpacket_parameter(3));
                             torque_total=hallar_torque(torque_total);  // Se halla el torque total usando la función hallar_torque
                             fprintf(ofp,"%d %d %d %d\n",id[i],direccion, torque_total,present_vel);//Escribe los datos id del servomotor, direccion del
                             }
                           }
                         fprintf(ofp,"\n");
                         fclose(ofp);                                    // Cierra el archivo en el que se estaba escribiendo
                       }

                    gettimeofday(&t_fin, NULL); //Se finaliza el conteo iniciado antes del proceso de transmisión del paquete de estados
                    while((timeval_diff(&t_fin, &t_ini)*1000)<100)//Mientras no se cumpla el tiempo de 100ms se queda en este ciclo
                            gettimeofday(&t_fin, NULL);
                }
            }
        }
    dxl_terminate(); //Se cierra el puerto de comunicación
    }

//Una vez se oprime el botón de about se abre la ventana
void SimpleGaits::abrir_about(){
    theabout= new about;
    theabout->show();
}

//Una vez se oprime uno de los botones de los gaits, tanto en las casillas de verificación de módulos activos como en los valores de cuadros
//de número, se predeterminan ciertos valores que permiten la ejecución del determinado gait. Acontinuación se muestran la asignación de
//los valores predeterminados para cada botón de gait
void SimpleGaits::linear(){
    ui->tiempo->setValue(4);      ui->amplitud_par->setValue(30); ui->amplitud_impar->setValue(0); ui->offset_par->setValue(0);
    ui->offset_impar->setValue(0);ui->dtheta_dn->setValue(120);   ui->dtheta_dt->setValue(36);     ui->desfase->setValue(0);
    ui->q1->setChecked(true); ui->q2->setChecked(true); ui->q3->setChecked(true); ui->q4->setChecked(true); ui->q5->setChecked(true);
    ui->q6->setChecked(true); ui->q7->setChecked(true); ui->q8->setChecked(true); ui->q9->setChecked(true); ui->q10->setChecked(true);
    ui->q11->setChecked(true); ui->q12->setChecked(true); ui->q13->setChecked(true); ui->q14->setChecked(true); ui->q15->setChecked(true);
    ui->q16->setChecked(true);
}

void SimpleGaits::linear_high(){
    ui->tiempo->setValue(4);      ui->amplitud_par->setValue(45); ui->amplitud_impar->setValue(0); ui->offset_par->setValue(0);
    ui->offset_impar->setValue(10);ui->dtheta_dn->setValue(120);   ui->dtheta_dt->setValue(18);     ui->desfase->setValue(0);
    ui->q1->setChecked(true); ui->q2->setChecked(true); ui->q3->setChecked(true); ui->q4->setChecked(true); ui->q5->setChecked(true);
    ui->q6->setChecked(true); ui->q7->setChecked(true); ui->q8->setChecked(true); ui->q9->setChecked(true); ui->q10->setChecked(true);
    ui->q11->setChecked(true); ui->q12->setChecked(true); ui->q13->setChecked(true); ui->q14->setChecked(true); ui->q15->setChecked(true);
    ui->q16->setChecked(true);
}

void SimpleGaits::linear_offset(){
    ui->tiempo->setValue(4);      ui->amplitud_par->setValue(30); ui->amplitud_impar->setValue(0); ui->offset_par->setValue(0);
    ui->offset_impar->setValue(10);ui->dtheta_dn->setValue(120);  ui->dtheta_dt->setValue(36);     ui->desfase->setValue(0);
    ui->q1->setChecked(true); ui->q2->setChecked(true); ui->q3->setChecked(true); ui->q4->setChecked(true); ui->q5->setChecked(true);
    ui->q6->setChecked(true); ui->q7->setChecked(true); ui->q8->setChecked(true); ui->q9->setChecked(true); ui->q10->setChecked(true);
    ui->q11->setChecked(true); ui->q12->setChecked(true); ui->q13->setChecked(true); ui->q14->setChecked(true); ui->q15->setChecked(true);
    ui->q16->setChecked(true);
}

void SimpleGaits::rolling(){
    ui->tiempo->setValue(4);     ui->amplitud_par->setValue(10);  ui->amplitud_impar->setValue(10);ui->offset_par->setValue(0);
    ui->offset_impar->setValue(0);ui->dtheta_dn->setValue(0);     ui->dtheta_dt->setValue(36);     ui->desfase->setValue(90);
    ui->q1->setChecked(true); ui->q2->setChecked(true); ui->q3->setChecked(true); ui->q4->setChecked(true); ui->q5->setChecked(true);
    ui->q6->setChecked(true); ui->q7->setChecked(true); ui->q8->setChecked(true); ui->q9->setChecked(true); ui->q10->setChecked(true);
    ui->q11->setChecked(true); ui->q12->setChecked(true); ui->q13->setChecked(true); ui->q14->setChecked(true); ui->q15->setChecked(true);
    ui->q16->setChecked(true);
}

void SimpleGaits::sidewinding(){
    ui->tiempo->setValue(4);    ui->amplitud_par->setValue(40);   ui->amplitud_impar->setValue(10);ui->offset_par->setValue(0);
    ui->offset_impar->setValue(0);ui->dtheta_dn->setValue(30);    ui->dtheta_dt->setValue(36);     ui->desfase->setValue(45);
    ui->q1->setChecked(true); ui->q2->setChecked(true); ui->q3->setChecked(true); ui->q4->setChecked(true); ui->q5->setChecked(true);
    ui->q6->setChecked(true); ui->q7->setChecked(true); ui->q8->setChecked(true); ui->q9->setChecked(true); ui->q10->setChecked(true);
    ui->q11->setChecked(true); ui->q12->setChecked(true); ui->q13->setChecked(true); ui->q14->setChecked(true); ui->q15->setChecked(true);
    ui->q16->setChecked(true);
}

void SimpleGaits::sidewinding_low(){
    ui->tiempo->setValue(4);    ui->amplitud_par->setValue(40);   ui->amplitud_impar->setValue(10);ui->offset_par->setValue(0);
    ui->offset_impar->setValue(0);ui->dtheta_dn->setValue(120);   ui->dtheta_dt->setValue(36);     ui->desfase->setValue(45);
    ui->q1->setChecked(true); ui->q2->setChecked(true); ui->q3->setChecked(true); ui->q4->setChecked(true); ui->q5->setChecked(true);
    ui->q6->setChecked(true); ui->q7->setChecked(true); ui->q8->setChecked(true); ui->q9->setChecked(true); ui->q10->setChecked(true);
    ui->q11->setChecked(true); ui->q12->setChecked(true); ui->q13->setChecked(true); ui->q14->setChecked(true); ui->q15->setChecked(true);
    ui->q16->setChecked(true);
}

void SimpleGaits::helix_rolling(){
    ui->tiempo->setValue(4);    ui->amplitud_par->setValue(60);   ui->amplitud_impar->setValue(60);ui->offset_par->setValue(0);
    ui->offset_impar->setValue(0);ui->dtheta_dn->setValue(10);   ui->dtheta_dt->setValue(18);     ui->desfase->setValue(90);
    ui->q1->setChecked(true); ui->q2->setChecked(true); ui->q3->setChecked(true); ui->q4->setChecked(true); ui->q5->setChecked(true);
    ui->q6->setChecked(true); ui->q7->setChecked(true); ui->q8->setChecked(true); ui->q9->setChecked(true); ui->q10->setChecked(true);
    ui->q11->setChecked(true); ui->q12->setChecked(true); ui->q13->setChecked(true); ui->q14->setChecked(true); ui->q15->setChecked(true);
    ui->q16->setChecked(true);
}

void SimpleGaits::home(){
    ui->tiempo->setValue(4);    ui->amplitud_par->setValue(0);    ui->amplitud_impar->setValue(0); ui->offset_par->setValue(0);
    ui->offset_impar->setValue(0);ui->dtheta_dn->setValue(0);     ui->dtheta_dt->setValue(9);     ui->desfase->setValue(0);
    ui->q1->setChecked(true); ui->q2->setChecked(true); ui->q3->setChecked(true); ui->q4->setChecked(true); ui->q5->setChecked(true);
    ui->q6->setChecked(true); ui->q7->setChecked(true); ui->q8->setChecked(true); ui->q9->setChecked(true); ui->q10->setChecked(true);
    ui->q11->setChecked(true); ui->q12->setChecked(true); ui->q13->setChecked(true); ui->q14->setChecked(true); ui->q15->setChecked(true);
    ui->q16->setChecked(true);
}

//Función que determina la diferencia de tiempos, en el programa es usada para que esta diferencia no supere 100ms
double timeval_diff(struct timeval *a, struct timeval *b)
{
  return
    (double)(a->tv_sec + (double)a->tv_usec/1000000) -
    (double)(b->tv_sec + (double)b->tv_usec/1000000);
}

//Función para una vez se adquiere la variable de torque, especifca la dirección en la que se encuentra el torque y el valor del torque
int hallar_torque(int torque){
    if(torque>1023)
    {
        direccion=1;
        return torque-1024;
    }
        else
    {
        direccion=0;
        return torque;
    }
}

//Función que efectúa el cálculo de la ecuación que determina el ángulo del servomotor para determinadas variables predefinidas
float Choset(int amplitud_par,int amplitud_impar, int desfase, int dtheta_dn, int dtheta_dt,int t_time,int n,int offset_par,int offset_impar){
int theta;
                       theta=(dtheta_dn*n + dtheta_dt*(t_time));
                       if (n%2==0)
                           return (((offset_par*3.14159)/180) + ((amplitud_par*3.14159)/180)*sin((theta*3.14159)/180))*180/3.14159;
                        else
                            return (((offset_impar*3.14159)/180) + ((amplitud_impar*3.14159)/180)*sin(((theta+desfase)*3.14159)/180))*180/3.14159;
}
