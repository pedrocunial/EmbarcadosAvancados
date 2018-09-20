#include <stdio.h>
#include "system.h"
#include <alt_types.h>
#include <unistd.h>
#include <io.h> /* Leiutura e escrita no Avalon */
#include "altera_avalon_pio_regs.h"
#include "sys/alt_irq.h"

int n = 0;
volatile int* edge_capture;

void init_pio();

int main(void){
  unsigned int led = 0;
  int n = 0;

  init_pio();

  printf("Embarcados++ \n");

  while(1){
	  IOWR_ALTERA_AVALON_PIO_DATA(PIO_0_BASE, n);
//	  int value = IORD_32DIRECT(PIO_1_BASE, 0);
//	  IOWR_32DIRECT(PIO_0_BASE, 0, 0x01 << value);
//	  usleep(50000);
  };

  return 0;
}


void delay(int n){
	unsigned int delay = 0 ;
	while(delay < n){
		delay++;
	}
}

void handle_button_interrupts(void* context, alt_u32 id)
{
    /* Cast context to edge_capture's type. It is important that this be
     * declared volatile to avoid unwanted compiler optimization.
     */
    volatile int* edge_capture_ptr = (volatile int*) context;
    /* Store the value in the Button's edge capture register in *context. */
    *edge_capture_ptr = IORD_ALTERA_AVALON_PIO_EDGE_CAP(PIO_0_BASE);
    n++;
    /* Reset the Button's edge capture register. */
    IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PIO_0_BASE, 0);
    int value = IORD_32DIRECT(PIO_1_BASE, 0);
	IOWR_32DIRECT(PIO_0_BASE, 0, 0x01 << value);
}

/* Initialize the pio. */
void init_pio()
{
    /* Recast the edge_capture pointer to match the alt_irq_register() function
     * prototype. */
    void* edge_capture_ptr = (void*) &edge_capture;
    /* Enable first four interrupts. */
    IOWR_ALTERA_AVALON_PIO_IRQ_MASK(PIO_1_BASE, 0xf);
    /* Reset the edge capture register. */
    IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PIO_1_BASE, 0x0);
    /* Register the interrupt handler. */
    alt_irq_register( PIO_1_IRQ, edge_capture_ptr,
                      handle_button_interrupts );
}


//#if defined(MAIN1)
//int main()
//{
//  volatile int in,  out;
//  while (1)
//  {
//      in = IORD_ALTERA_AVALON_PIO_DATA(PIO_0_BASE);
//      out = in;
//      IOWR_ALTERA_AVALON_PIO_DATA(PIO_BASE, out);
//  }
//}
//#endif
