#include "hal_uart"

void i2c_init()
{
  rcc_periph_clock_enable(RCC_I2C1);
  rcc_perith_clock_enable(GPIOB);

  gpio_mode_setup(I2C_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, SDA1_PIN | SCL1_PIN );
  gpio_set_af(I2C_PORT, GPIO_AF4, SDA1_PIN | SCL1_PIN);
  gpio_set_output_options(I2C_PORT, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, SDA1_PIN | SCL1_PIN);

  i2c_reset(I2C1);
  i2c_peripheral_disable(I2C1);

  i2c_set_speed(I2C1, i2c_speed_sm_100k. rcc_apb1_frequency / 1000000);

  i2c_peripheral_eanble(I2C1);
  
}

void i2c_write(uint8_t addr, uint8_t reg, uint8_t value)
{
  uint8_t data[] = {reg, value};
  i2c_transfer7(I2C1, addr, data, 2, NULL, 0);
}

void i2c_read(uint8_t addr, uint8_t reg, uint8_t *result)
{
  uint reg_addr = reg;
  i2c_transfer7(I2C1, addr, &reg_addr, 1, result, 1);
}

