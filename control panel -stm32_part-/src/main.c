//Custom libraries
#include "hal_uart.h"
#include "hal_gpio.h"
//C libraries
#include <stdint.h>
//libopencm3 libraries
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>



static void clock_setup(void)
{
  rcc_clock_setup_pll(&rcc_hse_25mhz_3v3[RCC_CLOCK_3V3_96MHZ]);
}

int main(void)
{
  clock_setup();
  buttons_setup();
  led_setup();
  uart_init(9600);
  uart_send_string("UART READY \n");
  
  while(1)
  {
    if (button_read(BUTTON1) )
    {
      print_text("code running");
      led_on();
      delay_ms(1000);
      led_off();
      delay_ms(1000);
    }
  }
}
