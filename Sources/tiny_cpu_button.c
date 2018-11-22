#include "tiny_cpu_button.h"

void cpu_button_io_init(void) 
{
    DDRA_BIT0 = 1; //out
    DDRB_BIT6 = 1;
    DDRB_BIT5 = 1;
    DDRB_BIT4 = 1;
    
    
    ATD0DIEN_IEN5 = 1; //digit in
    ATD0DIEN_IEN6 = 1; //digit in
}