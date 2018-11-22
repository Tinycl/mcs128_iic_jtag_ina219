#include "tiny_ect.h"

void ECT_Init(void) {
  TSCR2_PR = 7; //128 DIV
  TSCR2_TOI = 1; //enable overflow interrupt
  TSCR1_TEN = 1; //enable timer interrupt
}
