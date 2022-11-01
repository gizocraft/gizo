

#include <avr/io.h>
#include <util/delay.h>
#include "servo.h"
#include "h_bridge.h"

#define DEBOUNCE_PERIOD_MS 10

void init(void)
{
	init_servo();
	init_h_bridge();
}

int main(void)
{
    init ();
    DDRF |= (1<< PF1); // a1 output stoplicht_weg_rood
    DDRF |= (1<< PF2); // a2 output stoplicht_weg_geel
    DDRF |= (1<< PF3); // a3 output stoplicht_weg_groen
    DDRK |= (1<< PF0); // a8 output stoplicht_boot_rood
    DDRK |= (1<< PF1); // a9 output stoplicht_boot_groen
    DDRK |= (1<< PF4); // a12 output buzzer
    DDRE |= (1<< PE4); // RODE LED KAST
    DDRE |= (1<< PE5); // GELE LED KAST
    DDRG |= (1<< PG5); // GROENE LED KAST

    DDRH &= ~(1<<PH4);  // PWM 4 INPUT NOOD STOP
    DDRF &= ~(1<<PF7);  // A7 input switch neutraal
    DDRF &= ~(1<<PF6);  // A6 input limit switch dicht
    DDRF &= ~(1<<PF0);  // A0 input l switch OPEN
    DDRK &= ~(1<<PF5);  // A13 input limit switch open
    DDRK &= ~(1<<PK6);  // A14 input knop kast 1
    DDRK &= ~(1<<PK7);  // A15 input knop kast 2
    DDRB &= ~(1<<PB4);  // PWM 10 input sensor voor
    DDRK &= ~(1<<PK3); //  a11 input sensor achter

    int brug_open = 0;
    int brug_dicht = 1;

    while(1){

 if (!nood_stop()){

    if(!knop_kast1() && !switch_omhoog()) { // handmatige mode

    brug_open = 1;
    brug_dicht= 0;
    stoplicht(1);
    _delay_ms(1500);
    }
    if(!knop_kast2()&& !switch_omhoog()){

     brug_open = 0;
     brug_dicht= 1;
    }

    if(sensor_voor() && !switch_omlaag()) { //automatische mode

    brug_open = 1;
    brug_dicht= 0;
    }

    if(sensor_achter() && !switch_omlaag()){

     brug_open = 0;
     brug_dicht= 1;
    }

    if (!limit_switch_dicht() && !limit_switch_open()){ // brug niet gezien door beide sensoren
    buzz();
 }

    if (brug_open == 1){  //brug gaat open

      if( !limit_switch_open()){ // proces openen
        sluit_slagboom();
        draai_motor_links();
        PORTG = (1<< PG5); // led groen knopkast aan
         PORTE = (0<< PE5);// led geel knopkast uit
         stoplicht(3);


      }

       if (limit_switch_open()){ // brug is open
        motor_stil();
        stoplicht(2);
       }

    }
     if (brug_dicht == 1){ //brug gaat dicht

         if( !limit_switch_dicht()){ //proces sluiten
            stoplicht(3);
        draai_motor_rechts();


      }
       if (limit_switch_dicht()){ //brug is dicht
        PORTE = (1<< PE5);
        PORTG = (0<< PG5);
        motor_stil();
        open_slagboom();
        stoplicht(0);
       }

    }

 }



 else{            // noodknop actief

    stoplicht(3);
    motor_stil();
    sluit_slagboom();
    PORTE = (1<< PE4); //error led aan
 }

    }
}


     void motor_stil(void){
       h_bridge_set_percentage(0);
    }

    void draai_motor_rechts(void){
       h_bridge_set_percentage(-30);
    }

    void draai_motor_links(void){
      h_bridge_set_percentage(30);
    }

    void open_slagboom(void){

       servo1_set_percentage(100);
       servo2_set_percentage(100);
    }

     void sluit_slagboom(void){
        servo1_set_percentage(-100);
        servo2_set_percentage(-100);


    }
    void buzz(void){

    PORTK = (1<< PF4); // geluid aan :(
     _delay_ms(100);
    PORTK = (0<< PF4); // geluid uit :)
     _delay_ms(100);

    }

    void stoplicht(int k){

    switch (k){

        case 0: PORTF = (1<< PF3);// groen wegdek
                PORTK = (1<< PF0);// rood boot
                    break;
        case 1:  PORTF = (1<< PF2); //geel
                    break;
        case 2:  PORTF = (1<< PF1); //rood boot
                 PORTK = (1<< PF1);// groen wegdek
                    break;
        case 3: PORTK = (1<< PF0);// rood boot
                PORTF = (1<< PF1); //rood wegdek
                    break;
     }
    }

int switch_omhoog(void)
{
    if (PINF &(1<<PF0)){     // check pin laag
    _delay_ms (DEBOUNCE_PERIOD_MS);
        if (PINF &(1<<PF0)) return 1;// check opnieuw en geef dit aan
        }
        return 0; // geen gebeurtenis
}

int switch_omlaag(void)
{
    if (PINF &(1<<PF7)){
    _delay_ms (DEBOUNCE_PERIOD_MS);
        if (PINF &(1<<PF7)) return 1;
        }
        return 0;
}

 int limit_switch_dicht(void)
{
    if (PINF &(1<<PF6)){
    _delay_ms (DEBOUNCE_PERIOD_MS);
        if (PINF &(1<<PF6)) return 1;
        }
        return 0;
}

int limit_switch_open(void)
{
    if (PINK &(1<<PF5)){
    _delay_ms (DEBOUNCE_PERIOD_MS);
        if (PINK &(1<<PF5)) return 1;
        }
        return 0;
}

int nood_stop(void)
{
    if (PINH &(1<<PH4)){
    _delay_ms (DEBOUNCE_PERIOD_MS);
        if (PINH &(1<<PH4)) return 1;
        }
        return 0;
}

int sensor_voor(void)
{
    if (PINB &(1<<PB4)){
    _delay_ms (500);
        if (PINB &(1<<PB4)) return 1;
        }
        return 0;
}

int sensor_achter(void)
{
    if (PINK &(1<<PK3)){
    _delay_ms (500);
        if (PINK &(1<<PK3)) return 1;
        }
        return 0;
}
int knop_kast1(void)
{
    if (PINK &(1<<PK6)){
    _delay_ms (DEBOUNCE_PERIOD_MS);
        if (PINK &(1<<PK6)) return 1;
        }
        return 0;
}

int knop_kast2(void)
{
    if (PINK &(1<<PK7)){
    _delay_ms (DEBOUNCE_PERIOD_MS);
        if (PINK &(1<<PK7)) return 1;
        }
        return 0;
}

