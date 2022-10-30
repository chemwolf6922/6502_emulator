#ifndef _E_6502_H
#define _E_6502_H

#include <stdint.h>

typedef void(*e6502_store_t)(uint16_t addr, uint8_t data);
typedef uint8_t(*e6502_load_t)(uint16_t addr);

/** for safe interrupt simulation */
typedef void(*e6502_mutex_take_t)();
typedef void(*e6502_mutex_give_t)();

/** DO NOT use interrupts if mutex is not provided */
int e6502_init(
    e6502_store_t store, e6502_load_t load,
    e6502_mutex_take_t mutex_take, e6502_mutex_give_t mutex_give);
void e6502_run_forever();

/** interrupt injection */
void e6502_trigger_irq();
void e6502_trigger_nmi();

#endif
