

#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include "servo.h"
#include "h_bridge.h"

#define DEBOUNCE_PERIOD_MS 10
#define LOCK_PERIODE_MS 200
void init(void)
{
	init_servo();
	init_h_bridge();
}



int main(void)
{
    init ();
    DDRF |= (1<< PF1); // a1 outpout stoplicht_weg_rood
    DDRF |= (1<< PF2); // a2 outpout stoplicht_weg_geel
    DDRF |= (1<< PF3); // a3 outpout stoplicht weg_groen
    DDRK |= (1<< PF0); // a8 outpout stoplicht boot rood
    DDRK |= (1<< PF1); // a9 outpout stoplicht boot groen
    DDRK |= (1<< PF2); // a10 outpout
    DDRK |= (1<< PF4); // a12 outpout buzzer
    DDRE |= (1<< PE4); // RODE LED KAST
    DDRE |= (1<< PE5); // GELE LED KAST
    DDRG |= (1<< PG5); // GROENE LED KAST

    DDRH &= ~(1<<PH4);  // PWM 4 INPUT NOOD STOP
    DDRF &= ~(1<<PF7);  // A7 input  switch neutraal
    DDRF &= ~(1<<PF6);  // A6 input  limit switch dicht
    DDRF &= ~(1<<PF0);  // A0 input  l switch OPEN
    DDRK &= ~(1<<PF5);  // A13 input  limit switch open
    DDRF &= ~(1<<PF4);  // A4 input
    DDRK &= ~(1<<PK6);  // A14 input knop kast 1
    DDRK &= ~(1<<PK7);  // A15 input knop kast 2
    DDRB &= ~(1<<PB4);  // pwm 10 input sensor voor
    DDRK &= ~(1<<PK3); //  a11 input snsor achter

    bool brug_open ;
    bool brug_dicht ;


    while(1){

    if (switch_omlaag()){ // brug dicht
       PORTF = (1<< PF0);
        stoplicht(0);
        open_slagboom();




    }
    if ( !switch_omlaag()) {
        PORTF = (0<< PF0);  // brug open
       stoplicht(2);
       sluit_slagboom();

      }

   if( limit_switch_dicht()){
    draai_motor_links();
   }
   if(limit_switch_open()){
    draai_motor_rechts();


   }

  if (!switch_omhoog()){
    buzz();
  }




        }
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
    PORTK = (1<< PF4);
    }

    void stoplicht(int k){

    switch (k){

        case 0: PORTF = (1<< PF3);// groen
                PORTK = (1<< PF0);// rood
                    break;
        case 1:  PORTF = (1<< PF2); //geel
                    break;
        case 2:  PORTF = (1<< PF1); //rood
                 PORTK = (1<< PF1);// groen
                    break;
     }
    }


int switch_omhoog(void)
{
    if (PINF &(1<<PF0)){
    _delay_ms (DEBOUNCE_PERIOD_MS);
        if (PINF &(1<<PF0)) return 1;
        }
        return 0;
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
    _delay_ms (DEBOUNCE_PERIOD_MS);
        if (PINB &(1<<PB4)) return 1;
        }
        return 0;
}

int sensor_achter(void)
{
    if (PINK &(1<<PK3)){
    _delay_ms (DEBOUNCE_PERIOD_MS);
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
