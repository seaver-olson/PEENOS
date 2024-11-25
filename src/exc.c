/*
 * Copyright (C) 2018 bzt (bztsrc@github)
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 */

#include "rprintf.h"

/**
 * common exception handler
 */
void exc_handler(unsigned long type, unsigned long esr, unsigned long elr, unsigned long spsr, unsigned long far)
{

    switch(type) {
        case 0: esp_printf(putc,"Synchronous"); break;
        case 1: esp_printf(putc,"IRQ"); break;
        case 2: esp_printf(putc,"FIQ"); break;
        case 3: esp_printf(putc,"SError"); break;
    }
    esp_printf(putc,": ");
    // decode exception type (some, not all. See ARM DDI0487B_b chapter D10.2.28)
    switch(esr>>26) {
        case 0b000000: esp_printf(putc,"Unknown"); break;
        case 0b000001: esp_printf(putc,"Trapped WFI/WFE"); break;
        case 0b001110: esp_printf(putc,"Illegal execution"); break;
        case 0b010101: esp_printf(putc,"System call"); break;
        case 0b100000: esp_printf(putc,"Instruction abort, lower EL"); break;
        case 0b100001: esp_printf(putc,"Instruction abort, same EL"); break;
        case 0b100010: esp_printf(putc,"Instruction alignment fault"); break;
        case 0b100100: esp_printf(putc,"Data abort, lower EL"); break;
        case 0b100101: esp_printf(putc,"Data abort, same EL"); break;
        case 0b100110: esp_printf(putc,"Stack alignment fault"); break;
        case 0b101100: esp_printf(putc,"Floating point"); break;
        default: esp_printf(putc,"Unknown"); break;
    }
    // decode data abort cause
    if(esr>>26==0b100100 || esr>>26==0b100101) {
        esp_printf(putc,", ");
        switch((esr>>2)&0x3) {
            case 0: esp_printf(putc,"Address size fault"); break;
            case 1: esp_printf(putc,"Translation fault"); break;
            case 2: esp_printf(putc,"Access flag fault"); break;
            case 3: esp_printf(putc,"Permission fault"); break;
        }
        switch(esr&0x3) {
            case 0: esp_printf(putc," at level 0"); break;
            case 1: esp_printf(putc," at level 1"); break;
            case 2: esp_printf(putc," at level 2"); break;
            case 3: esp_printf(putc," at level 3"); break;
        }
    }
    // dump registers
    esp_printf(putc,":\n  ESR_EL1 ");
    esp_printhex(esr>>32);
    esp_printhex(esr);
    esp_printf(putc," ELR_EL1 ");
    esp_printhex(elr>>32);
    esp_printhex(elr);
    esp_printf(putc,"\n SPSR_EL1 ");
    esp_printhex(spsr>>32);
    esp_printhex(spsr);
    esp_printf(putc," FAR_EL1 ");
    esp_printhex(far>>32);
    esp_printhex(far);
    esp_printf(putc,"\n");
    // no return from exception for now
    while(1);
}
