#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef struct e6502_s e6502_t;
struct e6502_s
{
    /** interface */
    struct
    {
        /** Remember to call reset before running */
        int (*reset)(e6502_t* self);
        int (*step)(e6502_t* self);
        int (*run)(e6502_t* self);
        int (*trigger_irq)(e6502_t* self);
        int (*trigger_nmi)(e6502_t* self);
    } iface;
    /** callbacks. You should set those before using the interfaces */
    struct
    {
        void (*store)(uint16_t addr, uint8_t data, void* ctx);
        uint8_t (*load)(uint16_t addr, void* ctx);
        void* bus_ctx;
        /**
         * If lock is not provided, DO NOT call trigger_irq or trigger_nmi
         * Also DO NOT call reset while running.
         * Spinlock is recommended.
         */
        void (*lock)(void* ctx);
        void (*unlock)(void* ctx);
        void* lock_ctx;
    } cb;
    /** internal states, DO NOT modify */
    /** registers */
    struct
    {
        uint8_t X;
        uint8_t Y;
        uint8_t A;
        uint8_t SP;
        uint16_t PC;    /** incremented right after fetch */
        union 
        {
            uint8_t byte;
            struct
            {
                uint8_t C:1;
                uint8_t Z:1;
                uint8_t I:1;
                uint8_t D:1;
                uint8_t B:1;
                uint8_t _:1;
                uint8_t V:1;                    /** @link{http://www.6502.org/tutorials/vflag.html} */
                uint8_t N:1;
            } __attribute__((packed)) bits;     /** @link{https://www.nesdev.org/wiki/Status_flags} */
        } __attribute__((packed)) status;
    } reg;
    /** for interrupt simulation */
    struct
    {
        bool irq_pending;
        bool nmi_pending;
    } states;
};

void e6502_init(e6502_t* self);

