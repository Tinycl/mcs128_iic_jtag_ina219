#include <mc9s12dg128.h>
#include "tiny_mcu_config.h"

void Pll_16M_Init(void) {
    
    CLKSEL=0X00;			                	//disengage PLL to system
    PLLCTL_PLLON=1;			                
    SYNR =0x00 | 0x01;                    
    REFDV=0x20 | 0x01;                 
                                      //pllclock=2*osc*(1+SYNR)/(1+REFDV)
    _asm(nop);                         
    _asm(nop);
    while(!(CRGFLG_LOCK==1));	          //when pll is steady ,then use it;
    CLKSEL_PLLSEL =1;		                //engage PLL to system; 
}

void Pll_24M_Init(void) {
    
    CLKSEL=0X00;			                	//disengage PLL to system
    PLLCTL_PLLON=1;			                
    SYNR =0x00 | 0x05;                 // 00 05      
    REFDV=0x20 | 0x03;                 // 20 03
                                      //pllclock=2*osc*(1+SYNR)/(1+REFDV)
    _asm(nop);                         
    _asm(nop);
    while(!(CRGFLG_LOCK==1));	          //when pll is steady ,then use it;
    CLKSEL_PLLSEL =1;		                //engage PLL to system; 
}

void Pll_32M_Init(void) {
    
    CLKSEL=0X00;			                	//disengage PLL to system
    PLLCTL_PLLON=1;			                
    SYNR =0x40 | 0x03;                    
    REFDV=0x80 | 0x01;                 
                                      //pllclock=2*osc*(1+SYNR)/(1+REFDV)
    _asm(nop);                         
    _asm(nop);
    while(!(CRGFLG_LOCK==1));	          //when pll is steady ,then use it;
    CLKSEL_PLLSEL =1;		                //engage PLL to system; 
}


