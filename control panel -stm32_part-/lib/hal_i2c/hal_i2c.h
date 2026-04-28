#pragma once 
#include <stdunt.h>
#include"hal_gpio.h"

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/i2c.h>
#include <libopencm3/stm32/timer.h>

void i2c_init();
uint8_t i2c_read_reg();
void i2c_read_bytes();
void i2c_write_reg();
