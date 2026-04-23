#include "hal_uart.h"


#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>

void uart_init(uint32_t baudrate)
{
  // Enable clocks
  rcc_periph_clock_enable(RCC_GPIOA);
  rcc_periph_clock_enable(RCC_USART2);

  // Configure GPIO
  gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, RX2 | TX2);
  gpio_set_af(GPIOA, GPIO_AF7 ,RX2 | TX2);

  // Configure USART
  usart_set_baudrate(USART2, baudrate);
  usart_set_databits(USART2, 8);
  usart_set_stopbits(USART2, USART_STOPBITS_1);
  usart_set_mode(USART2, USART_MODE_TX_RX);
  usart_set_parity(USART2, USART_PARITY_NONE);
  usart_set_flow_control(USART2 , USART_FLOWCONTROL_NONE);

  // Enable USART
  usart_enable(USART2);
}

void print_char(char c)
{
  usart_send_blocking(USART2, c);
}
void print_text(const char *str)
{
  while(*str)
  {
    print_text(*str++);
  }
}
char read_char(void)
{
  return usart_recv_blocking(USART2);
}

