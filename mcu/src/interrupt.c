// button_interrupt.c
// Josh Brake
// jbrake@hmc.edu
// 10/31/22
// adapted for Quadrature encoders interrupts
// Corey Hickson chickson@hmc.edu 10/8/25

#include "main.h"

int32_t intrCount = 0;

uint8_t getSign() {
    return 0; // just return positive for now
} 

// Function used by printf to send characters to the laptop
int _write(int file, char *ptr, int len) {
  int i = 0;
  for (i = 0; i < len; i++) {
    ITM_SendChar((*ptr++));
  }
  return len;
}

int main(void) {

    float    pulses    = 0;
    float    rps       = 0;
    uint8_t  sign      = 0;


    // Enable both quad encoders as input
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

    // 1. Enable SYSCFG clock domain in RCC
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    // 2. Configure EXTICR for the input button interrupt
    SYSCFG->EXTICR[2] |= _VAL2FLD(SYSCFG_EXTICR3_EXTI10, 0b000); // Select PA10
    SYSCFG->EXTICR[2] |= _VAL2FLD(SYSCFG_EXTICR3_EXTI8,  0b000); // Select PA8

    // Enable interrupts globally
    __enable_irq();
    __NVIC_EnableIRQ(EXTI15_10_IRQn);
    __NVIC_EnableIRQ(EXTI9_5_IRQn);


    // 1. Configure mask bits
    EXTI->IMR1 |= (1 << gpioPinOffset(QA_PIN)); // Configure the mask bit for Quad A
    EXTI->IMR1 |= (1 << gpioPinOffset(QB_PIN)); // Configure the mask bit for Quad B
    // 2. Disable rising edge triggers
    EXTI->RTSR1 |= (1 << gpioPinOffset(QA_PIN));// Enable rising edge trigger for Quad A
    EXTI->RTSR1 |= (1 << gpioPinOffset(QB_PIN));// Enable rising edge trigger for Quad B
    // 3. Enable falling edge triggers
    EXTI->FTSR1 |= (1 << gpioPinOffset(QA_PIN));// Enable falling edge trigger for Quad A
    EXTI->FTSR1 |= (1 << gpioPinOffset(QB_PIN));// Enable falling edge trigger for Quad B
    // 4. Turn on EXTI interrupt in NVIC_ISER
    NVIC->ISER[0] |= (1 << EXTI9_5_IRQn);// enable EXTI interrupts for pin 8
    // NVIC->ISER[0] |= (1 << 23);
    NVIC->ISER[1] |= (1 << (EXTI15_10_IRQn - 32));// enable EXTI interrupts for pin 10
    // NVIC->ISER[1] |= (1 << (47 - 32));

    while(1){   
        delay_millis(TIM2, SAMPLE_PERIOD);
        pulses = intrCount / 4.0;
        sign = getSign();
        rps = (pulses / PPR) / (SAMPLE_PERIOD / 1000.0);
        rps = sign ? -rps : rps;
        intrCount = 0;
        //printf("Angular Velocity is %.5f RPS\n", rps);
        //printf("%d", digitalRead(PA10));
    }

}

void EXTI15_10_IRQHandler(void){
    // Check that Quad B was what triggered our interrupt
    if (EXTI->PR1 & (1 << gpioPinOffset(QB_PIN))){
        // If so, clear the interrupt (NB: Write 1 to reset.)
        EXTI->PR1 |= (1 << gpioPinOffset(QB_PIN));

        intrCount++;
        printf("B interrupt!!\n");

        // do something to keep track of direction
        
    } 
}

void EXTI9_5_IRQHandler(void){
    // Check that Quad A was what triggered our interrupt
    if (EXTI->PR1 & (1 << gpioPinOffset(QA_PIN))){
        // If so, clear the interrupt (NB: Write 1 to reset.)
        EXTI->PR1 |= (1 << gpioPinOffset(QA_PIN));
        
        intrCount++;
        printf("A interrupt!!\n");
        // do something to keep track of direction

    } 
}