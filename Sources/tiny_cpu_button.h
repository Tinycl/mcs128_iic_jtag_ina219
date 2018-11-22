#ifndef __TINY_CPU_BUTT0N_H__
#define __TINY_CPU_BUTT0N_H__
#include <mc9s12dg128.h>

#define CPURSTB_M_OUT PORTA_BIT0
#define PWRGD_CPU_M   PORTB_BIT6
#define NMI_M         PORTB_BIT5
#define SMIB_M        PORTB_BIT4

#define PWRGD_CNTRL_M PORTAD0_PTAD6
#define CPURSTB_M_IN  PORTAD0_PTAD5

void cpu_button_io_init(void);
#endif