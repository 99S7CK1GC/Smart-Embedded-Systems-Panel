#include "hal_i2c"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/i2c.h>




void i2c_init()
{
  rcc_periph_clock_enable(RCC_I2C1);
  rcc_perith_clock_enable(GPIOB);
  rcc_perith_reset_pulse(RST_I2C1); // better reset the peripheral

  gpio_mode_setup(I2C_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, SDA1_PIN | SCL1_PIN );
  gpio_set_af(I2C_PORT, GPIO_AF4, SDA1_PIN | SCL1_PIN);
  gpio_set_output_options(I2C_PORT, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, SDA1_PIN | SCL1_PIN);


  i2c_peripheral_disable(I2C1);

  i2c_set_clock_frequency(I2C, 42) //set APB1 ( Advanced Peripheral bus at 42Mhz (default one))
  i2c_set_fast_mode(I2C1); // set fast mode ( about 400kbs)
  i2c_set_ccr(I2C1, 32); //  put 32 based on the formula CRR (Control & RESET REGISTER)= ( fapb1 (42Mhz) / 3 * fSCL (400k) = 32 )
  i2c_set_trise(I2C1, 13); // put 13 based on the formula TRISE = ( (t_rise_max_ns / TAPB1) + 1 ) = 13,6 ~ 14
  i2c_set_dutycycle(I2C1, I2C_CCR_DUTY_DIV2) // set duty 0
  i2c_peripheral_eanble(I2C1);
}

//write one byte
void i2c_write_reg(uint8_t addr, uint8_t reg, uint8_t data)
{
  while(I2C_SR2(I2C1) & I2C_SR2_BUSY);

  i2c_send_start(I2C1);
  while(!((I2C1_SR1(I2C1) & I2C_SR1_SB) //SB = start bit
    && (  I2C_SR2(I2C1) & I2C_SR2_MSL))); //MSL = Master Selected

  //Send Slave address
  i2c_send_7bit_address(I2C1, addr, I2C_WRITE) // slave bit && write bit
  while(!(I2C_SR1(I2C1) & I2C_SR1_ADDR)); // wait for address ACK
  (void)I2C_SR2(I2C1); //Reading SR2 clears ADDR flag

  //Send Register address
  i2c_send_data(I2C1, reg);
  while(!(I2C_SR1(I2C1) & I2C_SR1_TxE))

  //Send data byte
  i2c_send_data(I2C1, data);
  while (!(I2C_SR1(I2C1) & I2C_SR1_BTF)); // wait for byte transfer finished

  // Send stop condition
  i2c_send_stop(I2C1);
}
  // reads one byte
  uint8_t i2c_read_reg(uint8_t addr, uint8_t reg)
{
  while(I2C_SR2(I2C1) & I2C_SR2_BUSY);

  //Write register address
  i2c_send_data(I2C1);
  while(!((I2C_SR(I2C1) & I2C_SR1_SB && (I2C_SR2(I2C1) & I2C_SR2_MSL)));
  
  i2c_send_7bit_address(I2C1, addr, I2c_WRITE);
  while (!(I2C_SR1(I2C1) & I2C_SR1_ADDR));
  (void)I2C_SR2(I2C1);

  i2c_send_stop(I2C1);

  while(!(I2C_SR1(I2C1) & I2C_SR1_RxNE));
    return i2c_get_data(I2C1);
}

//read multiple bytes
void i2c_read_bytes(uint8_t addr, uint8_t reg, uint8_t, *buf, uint8_t len)
{
  while(I2C_SR2(I2C) & I2C_SR2_BUSY);

  i2c_send_start(I2C1);
  while (!((I2C_SR1(I2C1) & I2C_SR1_SB) && (I2C_SR2(I2C1) & I2C_SR2_MSL)));
  i2c_send_7bit_address(I2C1) & I2C_SR1_ADDR));
  (void)I2C_SR2(I2C1);
  i2c_send_data(I2C1, reg);
  while (!(I2C_SR1(I2C1) & I2C_SR1_BTF));

  i2c_enable_ack(I2C1); // ACK each byte we recieve (keeps slave sending)
  i2c_send_start(I2C1);
  while (!(I2C_SR2(I2C1) & I2C_SR1_SB) && (I2C_SR2(I2C1) & I2C_SR2_MSL)));
  i2c_send_7bit_address(I2C, addr, I2C_READ);
  while (!(I2C_SR1(I2C1) & I2C_SR1_ADDR));
  (void)I2C_SR2(I2C1);

  //read bytes one by one
  for(uint8_t i = 0; i < len; i++)
  {
    if( i == len - 1)
    {
      //last byte we must disable ACK so slave stops after this byte
      i2c_disable_ack(I2C1);
      i2c_send_stop(I2C1);
    }
    while (!(I2C_SR1(I2C) & I2C_SR1_RxNE));
    buf[i] = i2c_get_data(I2C1);
  }

}

