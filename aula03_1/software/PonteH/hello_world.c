/*
 * "Hello World" example.
 *
 * This example prints 'Hello from Nios II' to the STDOUT stream. It runs on
 * the Nios II 'standard', 'full_featured', 'fast', and 'low_cost' example
 * designs. It runs with or without the MicroC/OS-II RTOS and requires a STDOUT
 * device in your system's hardware.
 * The memory footprint of this hosted application is ~69 kbytes by default
 * using the standard reference design.
 *
 * For a reduced footprint version of this template, and an explanation of how
 * to reduce the memory footprint for a given application, see the
 * "small_hello_world" template.
 *
 */

#include <stdio.h>
#include "system.h"
#include <alt_types.h>
#include <io.h> /* Leiutura e escrita no Avalon */
#include "altera_avalon_pio_regs.h"
#include "sys/alt_irq.h"

char reversed;
int duty_cycle;
volatile int *edge_capture;
volatile int *prev_state;

//#define SIM

//int NAME_init( ..... );          // Inicializa o periférico
//int NAME_config( ..... );        // Configura o periférico
//int NAME_halt( ..... );          // Desativa o periférico
//int NAME_en_irq( ..... );        // Habilita interrupção
//int NAME_disable_irq( ..... );   // Desabilita interrupção
//int NAME_read_xxxxx( ..... );    // read data xxxx from device
//int NAME_write_xxxxx( ..... );   // write data to xxxx device

// LED Peripheral
#define REG_DATA_OFFSET 1

void handle_button_interrupts(void *context, alt_u32 id)
{

	volatile int *edge_capture_ptr = (volatile int *) context;
//	*edge_capture_ptr = IORD_ALTERA_AVALON_PIO_EDGE_CAP(PIO_1_BASE);
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PIO_1_BASE, 0x0);
//	*reversed = (*prev_state != *edge_capture_ptr) ?
//			!*reversed : *reversed;
	reversed = !reversed;
	printf("hello from interrupt -- %d\n", reversed);
}

void handle_timeout_interrupts(void *context, alt_u32 id)
{
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP();
}

int hbridge_disable_irq()
{
	IOWR_ALTERA_AVALON_PIO_IRQ_MASK(PIO_1_BASE, 0x0);
	return 0;
}

int hbridge_read_duty_cycle()
{
	return duty_cycle;
}

int hbridge_write_duty_cycle(int dt)
{
	if (dt < 0) return 1;
	duty_cycle = dt;
	return 0;
}


int hbridge_enable_irq()
{
	void *edge_capture_ptr = (void *) &edge_capture;
	IOWR_ALTERA_AVALON_PIO_IRQ_MASK(PIO_1_BASE, 0xf);
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PIO_1_BASE, 0x0);
	alt_irq_register(PIO_1_IRQ, edge_capture_ptr,
			handle_button_interrupts);
	return 0;
}

int hbridge_init(int dc, char rev) {
	duty_cycle = dc;
	reversed = rev;
	hbridge_enable_irq();
	return 0;
}

int main(void){
  int led = 0;
  unsigned int *p_led = (unsigned int *) PERIPHERAL_LED_0_BASE;

  *prev_state = 0;
  hbridge_init(2, 0);

#ifndef SIM
  printf("Embarcados++ \n");
#endif

  while(1){
	  if (led < 4 && led > -1){
		  *(p_led+REG_DATA_OFFSET) = (0x1 << led);
		  printf("reversed: %d\n", reversed);
		  if (reversed)
			  led++;
		  else
			  led--;
#ifndef SIM
          usleep(500000 >> duty_cycle); // remover durante a simulação
#endif
	  }
	  else{
		  led = (led < 0) ? 3 : 0;
	  }

  };

  return 0;
}