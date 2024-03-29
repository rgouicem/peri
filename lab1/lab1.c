/*
 * This file is a template for the first Lab in the SESI-PERI module.
 *
 * It draws inspiration from Dom & Gert code on ELinux.org.
 *
 * Authors:
 *   Julien Peeters <julien.peeters@lip6.fr>
 */

#include <stdio.h>
#include <stdlib.h>

#include "gpio.h"

static
void
delay ( unsigned int milisec )
{
    struct timespec ts, dummy;
    ts.tv_sec  = ( time_t ) milisec / 1000;
    ts.tv_nsec = ( long ) ( milisec % 1000 ) * 1000000;
    nanosleep ( &ts, &dummy );
}

/*
 * Main program.
 */

#define GPIO_LED0   4

int
main ( int argc, char **argv )
{
    int period, half_period;
    int btn0, btn1;
    int count;

    /* Retreive the mapped GPIO memory. */

    if(gpio_setup() == -1) {
      perror("gpio_setup");
      return -1;
    }
    /* COMPLETE... */

    period = 1000; /* default = 1Hz */
    if ( argc > 1 ) {
        period = atoi ( argv[1] );
    }
    half_period = period / 2;

    /* Setup GPIO of LED0 to output. */
    
    if(gpio_config(GPIO_LED0, GPIO_OUTPUT_PIN) == -1) {
      perror("gpio_config");
      return -1;
    }
    /* COMPLETE... */

    printf ( "-- info: start blinking @ %f Hz.\n", ( 1000.0f / period ) );

    /* Blink led at frequency of 1Hz. */
    count = 20;
    while ( --count > 0 ) {
      gpio_update(GPIO_LED0, 1);
      usleep(half_period*1000);
      gpio_update(GPIO_LED0, 0);
      usleep(half_period*1000);
      /* COMPLETE... */
    }

    /* Reset state of GPIO. */
    
    
    /* COMPLETE... */

    /* Release the GPIO memory mapping. */

    gpio_teardown();
    /* COMPLETE... */
    
    return 0;
}

