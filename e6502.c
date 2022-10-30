#include "e6502.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define E6502_VEC_NMI (0xFFFA)
#define E6502_VEC_RST (0xFFFC)
#define E6502_VEC_IRQ (0xFFFE)

#define E6502_STACK_OFFSET (0x100)

#define pack_2u8(h,l) ((((uint16_t)(h))<<8)|((uint16_t)(l)))
#define get_low_byte(v) ((v)&0xFF)
#define get_high_byte(v) (((v)>>8)&0xFF)

typedef struct
{
    /** registers */
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
            uint8_t V:1;    /** @link{http://www.6502.org/tutorials/vflag.html} */
            uint8_t N:1;
        } __attribute__((packed)) bits;     /** @link{https://www.nesdev.org/wiki/Status_flags} */
    } __attribute__((packed)) status;
    /** bus */
    e6502_store_t store;
    e6502_load_t load;
    /** interrupt simulation */
    struct
    {
        bool irq_pending;
        bool nmi_pending;
        e6502_mutex_take_t mutex_take;
        e6502_mutex_give_t mutex_give;
    } interrupt;
} e6502_t;

/** instruction mode, not addressing mode */
typedef enum
{
    E6502_INS_MODE_NONE,
    E6502_INS_MODE_IMMEDIATE,
    E6502_INS_MODE_ADDR
} e6502_ins_mode_t;

typedef void(*e6502_instruction_func_t)(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);
typedef e6502_ins_mode_t(*e6502_addressing_func_t)(uint8_t* p_imm, uint16_t* p_addr);

typedef struct
{
    e6502_instruction_func_t func;
    e6502_addressing_func_t addr_func;
} e6502_instruction_t;

/** instructions */
static void instruction_LDA(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);
static void instruction_LDX(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);
static void instruction_LDY(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);
static void instruction_STA(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);
static void instruction_STX(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);
static void instruction_STY(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);
static void instruction_TAX(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);
static void instruction_TAY(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);
static void instruction_TSX(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);
static void instruction_TXA(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);
static void instruction_TXS(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);
static void instruction_TYA(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);

static void instruction_PHA(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);
static void instruction_PHP(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);
static void instruction_PLA(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);
static void instruction_PLP(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);

static void instruction_DEC(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);
static void instruction_DEX(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);
static void instruction_DEY(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);
static void instruction_INC(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);
static void instruction_INX(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);
static void instruction_INY(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);

static void instruction_ADC(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);
static void instruction_SBC(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);

static void instruction_AND(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);
static void instruction_EOR(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);
static void instruction_ORA(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);

static void instruction_ASL(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);
static void instruction_LSR(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);
static void instruction_ROL(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);
static void instruction_ROR(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);

static void instruction_CLC(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);
static void instruction_CLD(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);
static void instruction_CLI(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);
static void instruction_CLV(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);
static void instruction_SEC(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);
static void instruction_SED(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);
static void instruction_SEI(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);

static void instruction_CMP(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);
static void instruction_CPX(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);
static void instruction_CPY(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);

static void instruction_BCC(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);
static void instruction_BCS(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);
static void instruction_BEQ(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);
static void instruction_BMI(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);
static void instruction_BNE(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);
static void instruction_BPL(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);
static void instruction_BVC(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);
static void instruction_BVS(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);

static void instruction_JMP(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);
static void instruction_JSR(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);
static void instruction_RTS(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);

static void instruction_BRK(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);
static void instruction_RTI(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);

static void instruction_BIT(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);
static void instruction_NOP(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);

/** addressing modes */
static e6502_ins_mode_t addr_mode_accumulator(uint8_t* p_imm, uint16_t* p_addr);
static e6502_ins_mode_t addr_mode_absolute(uint8_t* p_imm, uint16_t* p_addr);
static e6502_ins_mode_t addr_mode_absolute_x(uint8_t* p_imm, uint16_t* p_addr);
static e6502_ins_mode_t addr_mode_absolute_y(uint8_t* p_imm, uint16_t* p_addr);
static e6502_ins_mode_t addr_mode_immediate(uint8_t* p_imm, uint16_t* p_addr);
static e6502_ins_mode_t addr_mode_implied(uint8_t* p_imm, uint16_t* p_addr);
static e6502_ins_mode_t addr_mode_indirect(uint8_t* p_imm, uint16_t* p_addr);
static e6502_ins_mode_t addr_mode_x_indirect(uint8_t* p_imm, uint16_t* p_addr);
static e6502_ins_mode_t addr_mode_indirect_y(uint8_t* p_imm, uint16_t* p_addr);
static e6502_ins_mode_t addr_mode_relative(uint8_t* p_imm, uint16_t* p_addr);
static e6502_ins_mode_t addr_mode_zeropage(uint8_t* p_imm, uint16_t* p_addr);
static e6502_ins_mode_t addr_mode_zeropage_x(uint8_t* p_imm, uint16_t* p_addr);
static e6502_ins_mode_t addr_mode_zeropage_y(uint8_t* p_imm, uint16_t* p_addr);


static e6502_instruction_t instructions[] = {
    /** 0x00 ---------------------------------------- */
    {instruction_BRK,addr_mode_implied},
    {instruction_ORA,addr_mode_x_indirect},
    {NULL,NULL},
    {NULL,NULL},
    {NULL,NULL},
    {instruction_ORA,addr_mode_zeropage},
    {instruction_ASL,addr_mode_zeropage},
    {NULL,NULL},
    {instruction_PHP,addr_mode_implied},
    {instruction_ORA,addr_mode_immediate},
    {instruction_ASL,addr_mode_accumulator},
    {NULL,NULL},
    {NULL,NULL},
    {instruction_ORA,addr_mode_absolute},
    {instruction_ASL,addr_mode_absolute},
    {NULL,NULL},
    /** 0x10 ---------------------------------------- */
    {instruction_BPL,addr_mode_relative},
    {instruction_ORA,addr_mode_indirect_y},
    {NULL,NULL},
    {NULL,NULL},
    {NULL,NULL},
    {instruction_ORA,addr_mode_zeropage_x},
    {instruction_ASL,addr_mode_zeropage_x},
    {NULL,NULL},
    {instruction_CLC,addr_mode_implied},
    {instruction_ORA,addr_mode_absolute_y},
    {NULL,NULL},
    {NULL,NULL},
    {NULL,NULL},
    {instruction_ORA,addr_mode_absolute_x},
    {instruction_ASL,addr_mode_absolute_x},
    {NULL,NULL},
    /** 0x20 ---------------------------------------- */
    {instruction_JSR,addr_mode_absolute},
    {instruction_AND,addr_mode_x_indirect},
    {NULL,NULL},
    {NULL,NULL},
    {instruction_BIT,addr_mode_zeropage},
    {instruction_AND,addr_mode_zeropage},
    {instruction_ROL,addr_mode_zeropage},
    {NULL,NULL},
    {instruction_PLP,addr_mode_implied},
    {instruction_AND,addr_mode_immediate},
    {instruction_ROL,addr_mode_accumulator},
    {NULL,NULL},
    {instruction_BIT,addr_mode_absolute},
    {instruction_AND,addr_mode_absolute},
    {instruction_ROL,addr_mode_absolute},
    {NULL,NULL},
    /** 0x30 ---------------------------------------- */
    {instruction_BMI,addr_mode_relative},
    {instruction_AND,addr_mode_indirect_y},
    {NULL,NULL},
    {NULL,NULL},
    {NULL,NULL},
    {instruction_AND,addr_mode_zeropage_x},
    {instruction_ROL,addr_mode_zeropage_x},
    {NULL,NULL},
    {instruction_SEC,addr_mode_implied},
    {instruction_AND,addr_mode_absolute_y},
    {NULL,NULL},
    {NULL,NULL},
    {NULL,NULL},
    {instruction_AND,addr_mode_absolute_x},
    {instruction_ROL,addr_mode_absolute_x},
    {NULL,NULL},
    /** 0x40 ---------------------------------------- */
    {instruction_RTI,addr_mode_implied},
    {instruction_EOR,addr_mode_x_indirect},
    {NULL,NULL},
    {NULL,NULL},
    {NULL,NULL},
    {instruction_EOR,addr_mode_zeropage},
    {instruction_LSR,addr_mode_zeropage},
    {NULL,NULL},
    {instruction_PHA,addr_mode_implied},
    {instruction_EOR,addr_mode_immediate},
    {instruction_LSR,addr_mode_accumulator},
    {NULL,NULL},
    {instruction_JMP,addr_mode_absolute},
    {instruction_EOR,addr_mode_absolute},
    {instruction_LSR,addr_mode_absolute},
    {NULL,NULL},
    /** 0x50 ---------------------------------------- */
    {instruction_BVC,addr_mode_relative},
    {instruction_EOR,addr_mode_indirect_y},
    {NULL,NULL},
    {NULL,NULL},
    {NULL,NULL},
    {instruction_EOR,addr_mode_zeropage_x},
    {instruction_LSR,addr_mode_zeropage_x},
    {NULL,NULL},
    {instruction_CLI,addr_mode_implied},
    {instruction_EOR,addr_mode_absolute_y},
    {NULL,NULL},
    {NULL,NULL},
    {NULL,NULL},
    {instruction_EOR,addr_mode_absolute_x},
    {instruction_LSR,addr_mode_absolute_x},
    {NULL,NULL},
    /** 0x60 ---------------------------------------- */
    {instruction_RTS,addr_mode_implied},
    {instruction_ADC,addr_mode_x_indirect},
    {NULL,NULL},
    {NULL,NULL},
    {NULL,NULL},
    {instruction_ADC,addr_mode_zeropage},
    {instruction_ROR,addr_mode_zeropage},
    {NULL,NULL},
    {instruction_PLA,addr_mode_implied},
    {instruction_ADC,addr_mode_immediate},
    {instruction_ROR,addr_mode_accumulator},
    {NULL,NULL},
    {instruction_JMP,addr_mode_indirect},
    {instruction_ADC,addr_mode_absolute},
    {instruction_ROR,addr_mode_absolute},
    {NULL,NULL},
    /** 0x70 ---------------------------------------- */
    {instruction_BVS,addr_mode_relative},
    {instruction_ADC,addr_mode_indirect_y},
    {NULL,NULL},
    {NULL,NULL},
    {NULL,NULL},
    {instruction_ADC,addr_mode_zeropage_x},
    {instruction_ROR,addr_mode_zeropage_x},
    {NULL,NULL},
    {instruction_SEI,addr_mode_implied},
    {instruction_ADC,addr_mode_absolute_y},
    {NULL,NULL},
    {NULL,NULL},
    {NULL,NULL},
    {instruction_ADC,addr_mode_absolute_x},
    {instruction_ROR,addr_mode_absolute_x},
    {NULL,NULL},
    /** 0x80 ---------------------------------------- */
    {NULL,NULL},
    {instruction_STA,addr_mode_x_indirect},
    {NULL,NULL},
    {NULL,NULL},
    {instruction_STY,addr_mode_zeropage},
    {instruction_STA,addr_mode_zeropage},
    {instruction_STX,addr_mode_zeropage},
    {NULL,NULL},
    {instruction_DEY,addr_mode_implied},
    {NULL,NULL},
    {instruction_TXA,addr_mode_implied},
    {NULL,NULL},
    {instruction_STY,addr_mode_absolute},
    {instruction_STA,addr_mode_absolute},
    {instruction_STX,addr_mode_absolute},
    {NULL,NULL},
    /** 0x90 ---------------------------------------- */
    {instruction_BCC,addr_mode_relative},
    {instruction_STA,addr_mode_indirect_y},
    {NULL,NULL},
    {NULL,NULL},
    {instruction_STY,addr_mode_zeropage_x},
    {instruction_STA,addr_mode_zeropage_x},
    {instruction_STX,addr_mode_zeropage_y},
    {NULL,NULL},
    {instruction_TYA,addr_mode_implied},
    {instruction_STA,addr_mode_absolute_y},
    {instruction_TXS,addr_mode_implied},
    {NULL,NULL},
    {NULL,NULL},
    {instruction_STA,addr_mode_absolute_x},
    {NULL,NULL},
    {NULL,NULL},
    /** 0xA0 ---------------------------------------- */
    {instruction_LDY,addr_mode_immediate},
    {instruction_LDA,addr_mode_x_indirect},
    {instruction_LDX,addr_mode_immediate},
    {NULL,NULL},
    {instruction_LDY,addr_mode_zeropage},
    {instruction_LDA,addr_mode_zeropage},
    {instruction_LDX,addr_mode_zeropage},
    {NULL,NULL},
    {instruction_TAY,addr_mode_implied},
    {instruction_LDA,addr_mode_immediate},
    {instruction_TAX,addr_mode_implied},
    {NULL,NULL},
    {instruction_LDY,addr_mode_absolute},
    {instruction_LDA,addr_mode_absolute},
    {instruction_LDX,addr_mode_absolute},
    {NULL,NULL},
    /** 0xB0 ---------------------------------------- */
    {instruction_BCS,addr_mode_relative},
    {instruction_LDA,addr_mode_indirect_y},
    {NULL,NULL},
    {NULL,NULL},
    {instruction_LDY,addr_mode_zeropage_x},
    {instruction_LDA,addr_mode_zeropage_x},
    {instruction_LDX,addr_mode_zeropage_y},
    {NULL,NULL},
    {instruction_CLV,addr_mode_implied},
    {instruction_LDA,addr_mode_absolute_y},
    {instruction_TSX,addr_mode_implied},
    {NULL,NULL},
    {instruction_LDY,addr_mode_absolute_x},
    {instruction_LDA,addr_mode_absolute_x},
    {instruction_LDX,addr_mode_absolute_y},
    {NULL,NULL},
    /** 0xC0 ---------------------------------------- */
    {instruction_CPY,addr_mode_immediate},
    {instruction_CMP,addr_mode_x_indirect},
    {NULL,NULL},
    {NULL,NULL},
    {instruction_CPY,addr_mode_zeropage},
    {instruction_CMP,addr_mode_zeropage},
    {instruction_DEC,addr_mode_zeropage},
    {NULL,NULL},
    {instruction_INY,addr_mode_implied},
    {instruction_CMP,addr_mode_immediate},
    {instruction_DEX,addr_mode_implied},
    {NULL,NULL},
    {instruction_CPY,addr_mode_absolute},
    {instruction_CMP,addr_mode_absolute},
    {instruction_DEC,addr_mode_absolute},
    {NULL,NULL},
    /** 0xD0 ---------------------------------------- */
    {instruction_BNE,addr_mode_relative},
    {instruction_CMP,addr_mode_indirect_y},
    {NULL,NULL},
    {NULL,NULL},
    {NULL,NULL},
    {instruction_CMP,addr_mode_zeropage_x},
    {instruction_DEC,addr_mode_zeropage_x},
    {NULL,NULL},
    {instruction_CLD,addr_mode_implied},
    {instruction_CMP,addr_mode_absolute_y},
    {NULL,NULL},
    {NULL,NULL},
    {NULL,NULL},
    {instruction_CMP,addr_mode_absolute_x},
    {instruction_DEC,addr_mode_absolute_x},
    {NULL,NULL},
    /** 0xE0 ---------------------------------------- */
    {instruction_CPX,addr_mode_immediate},
    {instruction_SBC,addr_mode_x_indirect},
    {NULL,NULL},
    {NULL,NULL},
    {instruction_CPX,addr_mode_zeropage},
    {instruction_SBC,addr_mode_zeropage},
    {instruction_INC,addr_mode_zeropage},
    {NULL,NULL},
    {instruction_INX,addr_mode_implied},
    {instruction_SBC,addr_mode_immediate},
    {instruction_NOP,addr_mode_implied},
    {NULL,NULL},
    {instruction_CPX,addr_mode_absolute},
    {instruction_SBC,addr_mode_absolute},
    {instruction_INC,addr_mode_absolute},
    {NULL,NULL},
    /** 0xF0 ---------------------------------------- */
    {instruction_BEQ,addr_mode_relative},
    {instruction_SBC,addr_mode_indirect_y},
    {NULL,NULL},
    {NULL,NULL},
    {NULL,NULL},
    {instruction_SBC,addr_mode_zeropage_x},
    {instruction_INC,addr_mode_zeropage_x},
    {NULL,NULL},
    {instruction_SED,addr_mode_implied},
    {instruction_SBC,addr_mode_absolute_y},
    {NULL,NULL},
    {NULL,NULL},
    {NULL,NULL},
    {instruction_SBC,addr_mode_absolute_x},
    {instruction_INC,addr_mode_absolute_x},
    {NULL,NULL}
};

static e6502_t e6502;

int e6502_init(
    e6502_store_t store, e6502_load_t load,
    e6502_mutex_take_t mutex_take, e6502_mutex_give_t mutex_give)
{
    if(store == NULL || load == NULL)
        return -1;
    memset(&e6502,0,sizeof(e6502));
    e6502.status.bits.I = 1;
    e6502.status.bits._ = 1;
    e6502.store = store;
    e6502.load = load;
    e6502.interrupt.mutex_give = mutex_give;
    e6502.interrupt.mutex_take = mutex_take;
    /** init PC */
    e6502.PC = pack_2u8(e6502.load(E6502_VEC_RST+1),e6502.load(E6502_VEC_RST));
    return 0;
}

void e6502_run_forever()
{
    for(;;)
    {
        /** handle interrupt if mutex is provided */
        if(e6502.interrupt.mutex_take && e6502.interrupt.mutex_give)
        {
            e6502.interrupt.mutex_take();
            if(e6502.interrupt.nmi_pending)
            {
                /** handle nmi */
                e6502.interrupt.nmi_pending = false;
                /** push PC & status into stack */
                e6502.store(e6502.SP+E6502_STACK_OFFSET,get_high_byte(e6502.PC));
                e6502.SP++;
                e6502.store(e6502.SP+E6502_STACK_OFFSET,get_low_byte(e6502.PC));
                e6502.SP++;
                /** B flag only exists in stack */
                e6502.status.bits.B = 0;
                e6502.store(e6502.SP+E6502_STACK_OFFSET,e6502.status.byte);
                e6502.SP++;
                /** set PC */
                e6502.PC = pack_2u8(e6502.load(E6502_VEC_NMI+1),e6502.load(E6502_VEC_NMI));
                /** set I */
                e6502.status.bits.I = 1;
            }
            if(e6502.interrupt.irq_pending)
            {
                /** handle irq */
                e6502.interrupt.irq_pending = false;
                /** push PC & status into stack */
                e6502.store(e6502.SP+E6502_STACK_OFFSET,get_high_byte(e6502.PC));
                e6502.SP++;
                e6502.store(e6502.SP+E6502_STACK_OFFSET,get_low_byte(e6502.PC));
                e6502.SP++;
                /** B flag only exists in stack */
                e6502.status.bits.B = 0;
                e6502.store(e6502.SP+E6502_STACK_OFFSET,e6502.status.byte);
                e6502.SP++;
                /** set PC */
                e6502.PC = pack_2u8(e6502.load(E6502_VEC_IRQ+1),e6502.load(E6502_VEC_IRQ));
                /** set I */
                e6502.status.bits.I = 1;
            }
            e6502.interrupt.mutex_give();
        }
        /** fetch & decoding */
        uint8_t op = e6502.load(e6502.PC++);
        e6502_instruction_t* ins = &instructions[op];
        e6502_ins_mode_t mode = E6502_INS_MODE_NONE;
        uint8_t imm = 0;
        uint16_t addr = 0;
        if(ins->addr_func)
            mode = ins->addr_func(&imm,&addr);
        /** execute */
        if(ins->func)
            ins->func(mode,imm,addr);
    }
}


/** interrupt injection */

void e6502_trigger_irq()
{
    if(e6502.interrupt.mutex_take && e6502.interrupt.mutex_give)
    {
        e6502.interrupt.mutex_take();
        if(!e6502.status.bits.I)
            e6502.interrupt.irq_pending = true;
        e6502.interrupt.mutex_give();
    }
}

void e6502_trigger_nmi()
{
    if(e6502.interrupt.mutex_take && e6502.interrupt.mutex_give)
    {
        e6502.interrupt.mutex_take();
        /** should be atomic, mutex is not requred here */
        e6502.interrupt.nmi_pending = true;
        e6502.interrupt.mutex_give();
    }
}

/** instruction implementations */

#define set_Z_flag(r) {e6502.status.bits.Z = ((r)==0);}
#define set_N_flag(r) {e6502.status.bits.N = ((r)>>7)&0x01;}

static void instruction_LDA(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr)
{
    uint8_t result;
    if(mode == E6502_INS_MODE_IMMEDIATE)
        result = e6502.load(imm);
    else if(mode == E6502_INS_MODE_ADDR)
        result = e6502.load(addr);
    else
        return; /** invalid */
    e6502.A = result;
    set_N_flag(result);
    set_Z_flag(result);
}
static void instruction_LDX(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr)
{
    uint8_t result;
    if(mode == E6502_INS_MODE_IMMEDIATE)
        result = e6502.load(imm);
    else if(mode == E6502_INS_MODE_ADDR)
        result = e6502.load(addr);
    else
        return; /** invalid */
    e6502.X = result;
    set_N_flag(result);
    set_Z_flag(result);
}
static void instruction_LDY(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr)
{
    uint8_t result;
    if(mode == E6502_INS_MODE_IMMEDIATE)
        result = e6502.load(imm);
    else if(mode == E6502_INS_MODE_ADDR)
        result = e6502.load(addr);
    else
        return; /** invalid */
    e6502.Y = result;
    set_N_flag(result);
    set_Z_flag(result);
}

static void instruction_STA(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr)
{
    if(mode == E6502_INS_MODE_ADDR)
        e6502.store(addr,e6502.A);
    else if(mode==E6502_INS_MODE_IMMEDIATE)
        e6502.store(imm,e6502.A);
}
static void instruction_STX(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr)
{
    if(mode == E6502_INS_MODE_ADDR)
        e6502.store(addr,e6502.X);
    else if(mode==E6502_INS_MODE_IMMEDIATE)
        e6502.store(imm,e6502.X);
}
static void instruction_STY(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr)
{
    if(mode == E6502_INS_MODE_ADDR)
        e6502.store(addr,e6502.Y);
    else if(mode==E6502_INS_MODE_IMMEDIATE)
        e6502.store(imm,e6502.Y);
}

static void instruction_TAX(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr)
{
    e6502.X = e6502.A;
}
static void instruction_TAY(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr)
{
    e6502.Y = e6502.A;
}
static void instruction_TSX(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr)
{
    e6502.X = e6502.SP;
}
static void instruction_TXA(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr)
{
    e6502.A = e6502.X;
}
static void instruction_TXS(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr)
{
    e6502.SP = e6502.X;
}
static void instruction_TYA(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr)
{
    e6502.A = e6502.Y;
}

/** @todo */
static void instruction_PHA(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);
static void instruction_PHP(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);
static void instruction_PLA(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);
static void instruction_PLP(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);


static void instruction_DEC(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);
static void instruction_DEX(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);
static void instruction_DEY(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);
static void instruction_INC(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);
static void instruction_INX(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);
static void instruction_INY(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);


static void instruction_ADC(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);
static void instruction_SBC(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);


static void instruction_AND(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);
static void instruction_EOR(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);
static void instruction_ORA(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);


static void instruction_ASL(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);
static void instruction_LSR(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);
static void instruction_ROL(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);
static void instruction_ROR(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);


static void instruction_CLC(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);
static void instruction_CLD(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);
static void instruction_CLI(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);
static void instruction_CLV(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);
static void instruction_SEC(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);
static void instruction_SED(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);
static void instruction_SEI(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);


static void instruction_CMP(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);
static void instruction_CPX(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);
static void instruction_CPY(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);


static void instruction_BCC(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);
static void instruction_BCS(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);
static void instruction_BEQ(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);
static void instruction_BMI(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);
static void instruction_BNE(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);
static void instruction_BPL(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);
static void instruction_BVC(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);
static void instruction_BVS(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);


static void instruction_JMP(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);
static void instruction_JSR(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);
static void instruction_RTS(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);


static void instruction_BRK(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);
static void instruction_RTI(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);


static void instruction_BIT(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);
static void instruction_NOP(e6502_ins_mode_t mode, uint8_t imm, uint16_t addr);


/** addressing mode implementations */

static e6502_ins_mode_t addr_mode_accumulator(uint8_t* p_imm, uint16_t* p_addr)
{
    return E6502_INS_MODE_NONE;
}

static e6502_ins_mode_t addr_mode_absolute(uint8_t* p_imm, uint16_t* p_addr)
{
    uint8_t l = e6502.load(e6502.PC++);
    uint8_t h = e6502.load(e6502.PC++);
    *p_addr = pack_2u8(h,l);
    return E6502_INS_MODE_ADDR;
}

static e6502_ins_mode_t addr_mode_absolute_x(uint8_t* p_imm, uint16_t* p_addr)
{
    uint8_t l = e6502.load(e6502.PC++);
    uint8_t h = e6502.load(e6502.PC++);
    *p_addr = pack_2u8(h,l) + e6502.X;
    return E6502_INS_MODE_ADDR;
}

static e6502_ins_mode_t addr_mode_absolute_y(uint8_t* p_imm, uint16_t* p_addr)
{
    uint8_t l = e6502.load(e6502.PC++);
    uint8_t h = e6502.load(e6502.PC++);
    *p_addr = pack_2u8(h,l) + e6502.Y;
    return E6502_INS_MODE_ADDR;
}

static e6502_ins_mode_t addr_mode_immediate(uint8_t* p_imm, uint16_t* p_addr)
{
    *p_imm = e6502.load(e6502.PC++);
    return E6502_INS_MODE_IMMEDIATE;
}

static e6502_ins_mode_t addr_mode_implied(uint8_t* p_imm, uint16_t* p_addr)
{
    return E6502_INS_MODE_NONE;
}

static e6502_ins_mode_t addr_mode_indirect(uint8_t* p_imm, uint16_t* p_addr)
{
    uint8_t l = e6502.load(e6502.PC++);
    uint8_t h = e6502.load(e6502.PC++);
    uint16_t temp = pack_2u8(h,l);
    l = e6502.load(temp++);
    h = e6502.load(temp);
    *p_addr = pack_2u8(h,l);
    return E6502_INS_MODE_ADDR;
}

static e6502_ins_mode_t addr_mode_x_indirect(uint8_t* p_imm, uint16_t* p_addr)
{
    uint8_t temp = e6502.load(e6502.PC++);
    temp += e6502.X;
    uint8_t l = e6502.load(temp++);
    uint8_t h = e6502.load(temp);
    *p_addr = pack_2u8(h,l);
    return E6502_INS_MODE_ADDR;
}

static e6502_ins_mode_t addr_mode_indirect_y(uint8_t* p_imm, uint16_t* p_addr)
{
    uint8_t temp = e6502.load(e6502.PC++);
    uint8_t l = e6502.load(temp++);
    uint8_t h = e6502.load(temp);
    *p_addr = pack_2u8(h,l) + e6502.Y;
    return E6502_INS_MODE_ADDR;
}

static e6502_ins_mode_t addr_mode_relative(uint8_t* p_imm, uint16_t* p_addr)
{
    uint8_t temp = e6502.load(e6502.PC);
    /** PC for the op code + temp */
    *p_addr = (uint16_t)(e6502.PC - 1 + (*((int8_t*)&temp)));
    e6502.PC++;
    return E6502_INS_MODE_ADDR;
}

static e6502_ins_mode_t addr_mode_zeropage(uint8_t* p_imm, uint16_t* p_addr)
{
    *p_addr = e6502.load(e6502.PC++);
    return E6502_INS_MODE_ADDR;
}

static e6502_ins_mode_t addr_mode_zeropage_x(uint8_t* p_imm, uint16_t* p_addr)
{
    *p_addr = (uint8_t)(e6502.load(e6502.PC++)+e6502.X);
    return E6502_INS_MODE_ADDR;
}

static e6502_ins_mode_t addr_mode_zeropage_y(uint8_t* p_imm, uint16_t* p_addr)
{
    *p_addr = (uint8_t)(e6502.load(e6502.PC++)+e6502.Y);
    return E6502_INS_MODE_ADDR;
}

