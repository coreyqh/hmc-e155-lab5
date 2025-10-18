
// #include "main.h"

// /*------------------------------------------------------------------------------------

// BEGIN AI CODE

// -------------------------------------------------------------------------------------*/

// // --- Initialization ---
// void Encoder_Init(void) {
//     // Enable GPIOA and TIM2 clocks
//     RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
//     RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;

//     // Configure PA0, PA1 as AF1 (TIM2_CH1, TIM2_CH2)
//     GPIOA->MODER &= ~((3 << (0*2)) | (3 << (1*2))); // clear mode
//     GPIOA->MODER |=  (2 << (0*2)) | (2 << (1*2));   // AF mode
//     GPIOA->AFR[0] |= (1 << (0*4)) | (1 << (1*4));   // AF1

//     // Configure TIM2 in encoder mode
//     TIM2->SMCR = TIM_SMCR_SMS_0 | TIM_SMCR_SMS_1;   // Encoder mode 3 (count on both TI1 and TI2)
//     TIM2->CCMR1 = (1 << 0) | (1 << 8);              // CC1S=01, CC2S=01 (inputs)
//     TIM2->CCER = 0;                                 // Rising edge polarity
//     TIM2->ARR = 0xFFFFFFFF;                         // Max count
//     TIM2->CNT = 0;                                  // Reset counter

//     // Enable update interrupt
//     TIM2->DIER |= TIM_DIER_UIE;
//     NVIC_EnableIRQ(TIM2_IRQn);

//     // Start timer
//     TIM2->CR1 |= TIM_CR1_CEN;
// }

// // --- Interrupt Handler ---
// void TIM2_IRQHandler(void) {
//     if (TIM2->SR & TIM_SR_UIF) {   // Update interrupt flag
//         TIM2->SR &= ~TIM_SR_UIF;   // Clear flag
//         // Handle overflow/underflow here
//     }
// }

// #define ENCODER_CPR   (408 * 4)    // Example: 1024 PPR encoder, x4 decoding

// volatile int32_t last_count = 0;
// volatile float rps = 0.0f;

// void SysTick_Handler(void) {
//     int32_t current = TIM2->CNT;
//     int32_t delta = current - last_count;
//     last_count = current;

//     // Handle wraparound (TIM2 is 32-bit, so usually not needed unless very long intervals)
//     // delta is signed, so negative means reverse rotation

//     float revolutions = (float)delta / ENCODER_CPR;
//     rps = revolutions / (SAMPLE_PERIOD/1000);
// }


// /*------------------------------------------------------------------------------------

// END AI CODE

// -------------------------------------------------------------------------------------*/

// // Function used by printf to send characters to the laptop
// int _write(int file, char *ptr, int len) {
//   int i = 0;
//   for (i = 0; i < len; i++) {
//     ITM_SendChar((*ptr++));
//   }
//   return len;
// }

// int main(void) {
//     Encoder_Init();
//     initTIM(DELAY_TIM);
//     while(1) {
//         delay_millis(DELAY_TIM, 200);
//         printf("Angular Velocity is %.5f RPS \n", rps);
//     }
    
// }