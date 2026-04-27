#pragma once

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>

#include <stdbool.h> 
#include <stdint.h>

// UART Definiton 
#define RX2             GPIO3
#define TX2             GPIO2 

// MPU6050 + BME280 Definition
#define SDA1_PIN        GPIO7
#define SCL1_PIN        GPIO6
#define I2C_PORT       GPIOB 

// Screen Definition
#define CLOCK_PIN       GPIO5
#define CLOCK_PORT      GPIOA
#define MOSI_PIN        GPIO7
#define MOSI_PORT       GPIOA
#define RESET_PIN       GPIO14
#define RESET_PORT      GPIOA
#define DC_PIN          GPIO1
#define DC_PORT         GPIOA
#define CS_PIN          GPIO4
#define CS_PORT         GPIOA 

// Buttons + LED Defintion 
#define BUTTON1_PIN    GPIO12
#define BUTTON2_PIN    GPIO11
#define BUTTONS_PORT    GPIOA 
#define LED_PIN        GPIO15
#define LED_PORT       GPIOA


void led_setup(void);
void buttons_setup(void);
void delay_ms(uint32_t count);
bool button_read(uint16_t pin);
void led_on();
void led_off();
