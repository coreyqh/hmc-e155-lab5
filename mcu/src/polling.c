//// button_polling.c
//// Josh Brake
//// jbrake@hmc.edu
//// 10/31/22
//// adapted for Quadrature encoders polling
//// Corey Hickson chickson@hmc.edu 10/8/25

///*
//  This program polls the user button on the Nucleo-L432KC board and has a
//  delay within the main loop to simulate the problems with polling for 
//  catching events.
//*/

#include "main.h"

int main(void) {
   // Enable  output
    gpioEnable(GPIO_PORT_A);
    pinMode(QA_PIN, GPIO_INPUT);
    pinMode(QB_PIN, GPIO_INPUT);
    GPIOA->PUPDR &= ~(0b11 << gpioPinOffset(QA_PIN));// reset pull up/down 
    GPIOA->PUPDR &= ~(0b11 << gpioPinOffset(QB_PIN));// reset pull up/down 
    GPIOA->PUPDR |= (0b01 << gpioPinOffset(QA_PIN));// set pull up
    GPIOA->PUPDR |= (0b01 << gpioPinOffset(QB_PIN));// set pull up

   // Initialize timer
   RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;
   initTIM(DELAY_TIM);

   int volatile cur_A_state = digitalRead(QA_PIN);
   int volatile prev_A_state = cur_A_state;
   int volatile cur_B_state = digitalRead(QB_PIN);
   int volatile prev_B_state = cur_B_state;

   while(1){
       prev_A_state = cur_A_state;
       cur_A_state = digitalRead(QA_PIN);
       prev_B_state = cur_B_state;
       cur_B_state = digitalRead(QB_PIN);

       if (prev_A_state != cur_A_state) {
           // A button transition
       }

       if (prev_B_state != cur_B_state) {
           // B button transition
       }

       delay_millis(DELAY_TIM, 200);
   }
}
