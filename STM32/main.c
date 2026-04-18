#include "stm32f4xx.h"
#include <stdio.h>
#include "classifier.h"

#define DEBOUNCE_MS 20
#define LETTER_GAP_MS 800
#define MAX_SYMBOLS 5

volatile uint32_t tick = 0;

uint32_t press[MAX_SYMBOLS];
int count = 0;
int waiting_gap = 0;

uint32_t press_start, release_start;
float input_data[5];

void SysTick_Handler() { tick++; }
uint32_t millis() { return tick; }

void SysTick_Init() {
    SysTick->LOAD = 16000 - 1;
    SysTick->CTRL = 7;
}

void GPIO_Init() {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
    GPIOC->MODER &= ~(3 << (13*2));
}

void USART1_Init() {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

    GPIOA->MODER |= (2 << (9*2));
    GPIOA->AFR[1] |= (7 << 4);

    USART1->BRR = 0x8B;
    USART1->CR1 |= USART_CR1_TE | USART_CR1_UE;
}

void send_char(char c) {
    while (!(USART1->SR & USART_SR_TXE));
    USART1->DR = c;
}

void send_line(char *s) {
    while (*s) send_char(*s++);
    send_char('\r'); send_char('\n');
}

int main() {

    SysTick_Init();
    GPIO_Init();
    USART1_Init();

    send_line("Morse ML (Naive Bayes) Ready");

    uint8_t prev = 1;
    uint32_t debounce = 0;

    while (1) {

        uint32_t now = millis();
        uint8_t btn = (GPIOC->IDR & (1<<13)) ? 1 : 0;

        if (btn != prev) {
            if ((now - debounce) > DEBOUNCE_MS) {
                debounce = now;
                prev = btn;

                if (btn == 0) {
                    press_start = now;
                } else {
                    if (count < MAX_SYMBOLS)
                        press[count++] = now - press_start;

                    release_start = now;
                    waiting_gap = 1;
                }
            }
        }

        if (waiting_gap && count > 0) {
            if ((now - release_start) > LETTER_GAP_MS) {

                float max = 0;
                for (int i = 0; i < count; i++)
                    if (press[i] > max) max = press[i];

                for (int i = 0; i < count; i++)
                    input_data[i] = press[i] / max;

                for (int i = count; i < 5; i++)
                    input_data[i] = 0;

                char result = predict(input_data);

                char buf[32];
                sprintf(buf, "Predicted: %c", result);
                send_line(buf);

                count = 0;
                waiting_gap = 0;
            }
        }
    }
}