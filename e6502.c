#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "e6502.h"

#define E6502_STACK_OFFSET (0x100)

#define LOAD(this, addr) ((this)->cb.load((addr), (this)->cb.bus_ctx))

#define STORE(this, addr, value) \
    do \
    { \
        (this)->cb.store((addr), (value), (this)->cb.bus_ctx); \
    } while (0);

#define STACK_PUSH(this, value) \
    do \
    { \
        STORE((this), ((this)->reg.SP++)+E6502_STACK_OFFSET, (value)); \
    } while (0);

#define STACK_POP(this) (LOAD((this), (--((this)->reg.SP))+E6502_STACK_OFFSET))

#define PACK_2U8(h,l) ((((uint16_t)(h))<<8)|((uint16_t)(l)))
#define GET_LOW_BYTE(v) ((v)&0xFF)
#define GET_HIGH_BYTE(v) (((v)>>8)&0xFF)

/** instruction mode, not addressing mode */
enum
{
    E6502_INS_MODE_NONE,
    E6502_INS_MODE_IMMEDIATE,
    E6502_INS_MODE_ADDR
};

/** 
 * instructions 
 * @link{http://6502.org/tutorials/6502opcodes.html} 
 * @link{https://www.masswerk.at/6502/6502_instruction_set.html}
 */
#define INST(name) instruction_##name
#define INST_DEF(name) static void INST(name)(e6502_t* this, int mode, uint8_t imm, uint16_t addr)

INST_DEF(LDA);
INST_DEF(LDX);
INST_DEF(LDY);
INST_DEF(STA);
INST_DEF(STX);
INST_DEF(STY);
INST_DEF(TAX);
INST_DEF(TAY);
INST_DEF(TSX);
INST_DEF(TXA);
INST_DEF(TXS);
INST_DEF(TYA);

INST_DEF(PHA);
INST_DEF(PHP);
INST_DEF(PLA);
INST_DEF(PLP);

INST_DEF(DEC);
INST_DEF(DEX);
INST_DEF(DEY);
INST_DEF(INC);
INST_DEF(INX);
INST_DEF(INY);

INST_DEF(ADC);
INST_DEF(SBC);

INST_DEF(AND);
INST_DEF(EOR);
INST_DEF(ORA);

INST_DEF(ASL);
INST_DEF(LSR);
INST_DEF(ROL);
INST_DEF(ROR);

INST_DEF(CLC);
INST_DEF(CLD);
INST_DEF(CLI);
INST_DEF(CLV);
INST_DEF(SEC);
INST_DEF(SED);
INST_DEF(SEI);

INST_DEF(CMP);
INST_DEF(CPX);
INST_DEF(CPY);

INST_DEF(BCC);
INST_DEF(BCS);
INST_DEF(BEQ);
INST_DEF(BMI);
INST_DEF(BNE);
INST_DEF(BPL);
INST_DEF(BVC);
INST_DEF(BVS);

INST_DEF(JMP);
INST_DEF(JSR);
INST_DEF(RTS);

INST_DEF(BRK);
INST_DEF(RTI);

INST_DEF(BIT);
INST_DEF(NOP);

/** addressing modes */
#define ADDR_MODE(name) addr_mode_##name
#define ADDR_MODE_DEF(name) static int ADDR_MODE(name)(e6502_t* this, uint8_t* p_imm, uint16_t* p_addr)

ADDR_MODE_DEF(accumulator);
ADDR_MODE_DEF(absolute);
ADDR_MODE_DEF(absolute_x);
ADDR_MODE_DEF(absolute_y);
ADDR_MODE_DEF(immediate);
ADDR_MODE_DEF(implied);
ADDR_MODE_DEF(indirect);
ADDR_MODE_DEF(x_indirect);
ADDR_MODE_DEF(indirect_y);
ADDR_MODE_DEF(relative);
ADDR_MODE_DEF(zeropage);
ADDR_MODE_DEF(zeropage_x);
ADDR_MODE_DEF(zeropage_y);

typedef struct
{
    void (*instruction)(e6502_t* this, int mode, uint8_t imm, uint16_t addr);
    int (*addressing_mode)(e6502_t* this, uint8_t* p_imm, uint16_t* p_addr);
} e6502_instruction_t;

#define INST_ENTRY(inst, addr_mode) {INST(inst),ADDR_MODE(addr_mode)}
#define INST_NULL {NULL,NULL}

static const e6502_instruction_t instructions[] = {
    /** 0x00 ---------------------------------------- */
    INST_ENTRY(BRK, implied),
    INST_ENTRY(ORA, x_indirect),
    INST_NULL,
    INST_NULL,
    INST_NULL,
    INST_ENTRY(ORA, zeropage),
    INST_ENTRY(ASL, zeropage),
    INST_NULL,
    INST_ENTRY(PHP, implied),
    INST_ENTRY(ORA, immediate),
    INST_ENTRY(ASL, accumulator),
    INST_NULL,
    INST_NULL,
    INST_ENTRY(ORA, absolute),
    INST_ENTRY(ASL, absolute),
    INST_NULL,
    /** 0x10 ---------------------------------------- */
    INST_ENTRY(BPL, relative),
    INST_ENTRY(ORA, indirect_y),
    INST_NULL,
    INST_NULL,
    INST_NULL,
    INST_ENTRY(ORA, zeropage_x),
    INST_ENTRY(ASL, zeropage_x),
    INST_NULL,
    INST_ENTRY(CLC, implied),
    INST_ENTRY(ORA, absolute_y),
    INST_NULL,
    INST_NULL,
    INST_NULL,
    INST_ENTRY(ORA, absolute_x),
    INST_ENTRY(ASL, absolute_x),
    INST_NULL,
    /** 0x20 ---------------------------------------- */
    INST_ENTRY(JSR, absolute),
    INST_ENTRY(AND, x_indirect),
    INST_NULL,
    INST_NULL,
    INST_ENTRY(BIT, zeropage),
    INST_ENTRY(AND, zeropage),
    INST_ENTRY(ROL, zeropage),
    INST_NULL,
    INST_ENTRY(PLP, implied),
    INST_ENTRY(AND, immediate),
    INST_ENTRY(ROL, accumulator),
    INST_NULL,
    INST_ENTRY(BIT, absolute),
    INST_ENTRY(AND, absolute),
    INST_ENTRY(ROL, absolute),
    INST_NULL,
    /** 0x30 ---------------------------------------- */
    INST_ENTRY(BMI, relative),
    INST_ENTRY(AND, indirect_y),
    INST_NULL,
    INST_NULL,
    INST_NULL,
    INST_ENTRY(AND, zeropage_x),
    INST_ENTRY(ROL, zeropage_x),
    INST_NULL,
    INST_ENTRY(SEC, implied),
    INST_ENTRY(AND, absolute_y),
    INST_NULL,
    INST_NULL,
    INST_NULL,
    INST_ENTRY(AND, absolute_x),
    INST_ENTRY(ROL, absolute_x),
    INST_NULL,
    /** 0x40 ---------------------------------------- */
    INST_ENTRY(RTI, implied),
    INST_ENTRY(EOR, x_indirect),
    INST_NULL,
    INST_NULL,
    INST_NULL,
    INST_ENTRY(EOR, zeropage),
    INST_ENTRY(LSR, zeropage),
    INST_NULL,
    INST_ENTRY(PHA, implied),
    INST_ENTRY(EOR, immediate),
    INST_ENTRY(LSR, accumulator),
    INST_NULL,
    INST_ENTRY(JMP, absolute),
    INST_ENTRY(EOR, absolute),
    INST_ENTRY(LSR, absolute),
    INST_NULL,
    /** 0x50 ---------------------------------------- */
    INST_ENTRY(BVC, relative),
    INST_ENTRY(EOR, indirect_y),
    INST_NULL,
    INST_NULL,
    INST_NULL,
    INST_ENTRY(EOR, zeropage_x),
    INST_ENTRY(LSR, zeropage_x),
    INST_NULL,
    INST_ENTRY(CLI, implied),
    INST_ENTRY(EOR, absolute_y),
    INST_NULL,
    INST_NULL,
    INST_NULL,
    INST_ENTRY(EOR, absolute_x),
    INST_ENTRY(LSR, absolute_x),
    INST_NULL,
    /** 0x60 ---------------------------------------- */
    INST_ENTRY(RTS, implied),
    INST_ENTRY(ADC, x_indirect),
    INST_NULL,
    INST_NULL,
    INST_NULL,
    INST_ENTRY(ADC, zeropage),
    INST_ENTRY(ROR, zeropage),
    INST_NULL,
    INST_ENTRY(PLA, implied),
    INST_ENTRY(ADC, immediate),
    INST_ENTRY(ROR, accumulator),
    INST_NULL,
    INST_ENTRY(JMP, indirect),
    INST_ENTRY(ADC, absolute),
    INST_ENTRY(ROR, absolute),
    INST_NULL,
    /** 0x70 ---------------------------------------- */
    INST_ENTRY(BVS, relative),
    INST_ENTRY(ADC, indirect_y),
    INST_NULL,
    INST_NULL,
    INST_NULL,
    INST_ENTRY(ADC, zeropage_x),
    INST_ENTRY(ROR, zeropage_x),
    INST_NULL,
    INST_ENTRY(SEI, implied),
    INST_ENTRY(ADC, absolute_y),
    INST_NULL,
    INST_NULL,
    INST_NULL,
    INST_ENTRY(ADC, absolute_x),
    INST_ENTRY(ROR, absolute_x),
    INST_NULL,
    /** 0x80 ---------------------------------------- */
    INST_NULL,
    INST_ENTRY(STA, x_indirect),
    INST_NULL,
    INST_NULL,
    INST_ENTRY(STY, zeropage),
    INST_ENTRY(STA, zeropage),
    INST_ENTRY(STX, zeropage),
    INST_NULL,
    INST_ENTRY(DEY, implied),
    INST_NULL,
    INST_ENTRY(TXA, implied),
    INST_NULL,
    INST_ENTRY(STY, absolute),
    INST_ENTRY(STA, absolute),
    INST_ENTRY(STX, absolute),
    INST_NULL,
    /** 0x90 ---------------------------------------- */
    INST_ENTRY(BCC, relative),
    INST_ENTRY(STA, indirect_y),
    INST_NULL,
    INST_NULL,
    INST_ENTRY(STY, zeropage_x),
    INST_ENTRY(STA, zeropage_x),
    INST_ENTRY(STX, zeropage_y),
    INST_NULL,
    INST_ENTRY(TYA, implied),
    INST_ENTRY(STA, absolute_y),
    INST_ENTRY(TXS, implied),
    INST_NULL,
    INST_NULL,
    INST_ENTRY(STA, absolute_x),
    INST_NULL,
    INST_NULL,
    /** 0xA0 ---------------------------------------- */
    INST_ENTRY(LDY, immediate),
    INST_ENTRY(LDA, x_indirect),
    INST_ENTRY(LDX, immediate),
    INST_NULL,
    INST_ENTRY(LDY, zeropage),
    INST_ENTRY(LDA, zeropage),
    INST_ENTRY(LDX, zeropage),
    INST_NULL,
    INST_ENTRY(TAY, implied),
    INST_ENTRY(LDA, immediate),
    INST_ENTRY(TAX, implied),
    INST_NULL,
    INST_ENTRY(LDY, absolute),
    INST_ENTRY(LDA, absolute),
    INST_ENTRY(LDX, absolute),
    INST_NULL,
    /** 0xB0 ---------------------------------------- */
    INST_ENTRY(BCS, relative),
    INST_ENTRY(LDA, indirect_y),
    INST_NULL,
    INST_NULL,
    INST_ENTRY(LDY, zeropage_x),
    INST_ENTRY(LDA, zeropage_x),
    INST_ENTRY(LDX, zeropage_y),
    INST_NULL,
    INST_ENTRY(CLV, implied),
    INST_ENTRY(LDA, absolute_y),
    INST_ENTRY(TSX, implied),
    INST_NULL,
    INST_ENTRY(LDY, absolute_x),
    INST_ENTRY(LDA, absolute_x),
    INST_ENTRY(LDX, absolute_y),
    INST_NULL,
    /** 0xC0 ---------------------------------------- */
    INST_ENTRY(CPY, immediate),
    INST_ENTRY(CMP, x_indirect),
    INST_NULL,
    INST_NULL,
    INST_ENTRY(CPY, zeropage),
    INST_ENTRY(CMP, zeropage),
    INST_ENTRY(DEC, zeropage),
    INST_NULL,
    INST_ENTRY(INY, implied),
    INST_ENTRY(CMP, immediate),
    INST_ENTRY(DEX, implied),
    INST_NULL,
    INST_ENTRY(CPY, absolute),
    INST_ENTRY(CMP, absolute),
    INST_ENTRY(DEC, absolute),
    INST_NULL,
    /** 0xD0 ---------------------------------------- */
    INST_ENTRY(BNE, relative),
    INST_ENTRY(CMP, indirect_y),
    INST_NULL,
    INST_NULL,
    INST_NULL,
    INST_ENTRY(CMP, zeropage_x),
    INST_ENTRY(DEC, zeropage_x),
    INST_NULL,
    INST_ENTRY(CLD, implied),
    INST_ENTRY(CMP, absolute_y),
    INST_NULL,
    INST_NULL,
    INST_NULL,
    INST_ENTRY(CMP, absolute_x),
    INST_ENTRY(DEC, absolute_x),
    INST_NULL,
    /** 0xE0 ---------------------------------------- */
    INST_ENTRY(CPX, immediate),
    INST_ENTRY(SBC, x_indirect),
    INST_NULL,
    INST_NULL,
    INST_ENTRY(CPX, zeropage),
    INST_ENTRY(SBC, zeropage),
    INST_ENTRY(INC, zeropage),
    INST_NULL,
    INST_ENTRY(INX, implied),
    INST_ENTRY(SBC, immediate),
    INST_ENTRY(NOP, implied),
    INST_NULL,
    INST_ENTRY(CPX, absolute),
    INST_ENTRY(SBC, absolute),
    INST_ENTRY(INC, absolute),
    INST_NULL,
    /** 0xF0 ---------------------------------------- */
    INST_ENTRY(BEQ, relative),
    INST_ENTRY(SBC, indirect_y),
    INST_NULL,
    INST_NULL,
    INST_NULL,
    INST_ENTRY(SBC, zeropage_x),
    INST_ENTRY(INC, zeropage_x),
    INST_NULL,
    INST_ENTRY(SED, implied),
    INST_ENTRY(SBC, absolute_y),
    INST_NULL,
    INST_NULL,
    INST_NULL,
    INST_ENTRY(SBC, absolute_x),
    INST_ENTRY(INC, absolute_x),
    INST_NULL
};

static int e6502_reset(e6502_t* this);
static int e6502_step(e6502_t* this);
static int e6502_run(e6502_t* this);
static int e6502_trigger_irq(e6502_t* this);
static int e6502_trigger_nmi(e6502_t* this);

void e6502_init(e6502_t* this)
{
    if (this == NULL)
        return;
    memset(this, 0, sizeof(e6502_t));
    this->iface.reset = e6502_reset;
    this->iface.step = e6502_step;
    this->iface.run = e6502_run;
    this->iface.trigger_irq = e6502_trigger_irq;
    this->iface.trigger_nmi = e6502_trigger_nmi;
}

static int e6502_reset(e6502_t* this)
{
    if (!this || !this->cb.store || !this->cb.load)
        return -1;
    if (this->cb.lock)
        this->cb.lock(this->cb.lock_ctx);
    this->states.irq_pending = false;
    this->states.nmi_pending = false;
    memset(&this->reg, 0, sizeof(this->reg));
    this->reg.status.bits.I = 1;
    this->reg.status.bits._ = 1;
    this->reg.PC = PACK_2U8(LOAD(this, E6502_VEC_RST + 1), LOAD(this, E6502_VEC_RST));
    if (this->cb.unlock)
        this->cb.unlock(this->cb.lock_ctx);
    return 0;
}

static inline void step_internal(e6502_t* this)
{
    /** lock states */
    if (this->cb.lock)
        this->cb.lock(this->cb.lock_ctx);
    /** handle interrupt */
    if(this->states.nmi_pending)
    {
        /** handle nmi */
        this->states.nmi_pending = false;
        /** push PC & status into stack */
        STACK_PUSH(this, GET_HIGH_BYTE(this->reg.PC));
        STACK_PUSH(this, GET_LOW_BYTE(this->reg.PC));
        STACK_PUSH(this, this->reg.status.byte);
        /** set PC */
        this->reg.PC = PACK_2U8(LOAD(this, E6502_VEC_NMI+1), LOAD(this, E6502_VEC_NMI));
        /** set I */
        this->reg.status.bits.I = 1;
    }
    else if(this->states.irq_pending)
    {
        /** handle irq */
        this->states.irq_pending = false;
        /** push PC & status into stack */
        STACK_PUSH(this, GET_HIGH_BYTE(this->reg.PC));
        STACK_PUSH(this, GET_LOW_BYTE(this->reg.PC));
        STACK_PUSH(this, this->reg.status.byte);
        /** set PC */
        this->reg.PC = PACK_2U8(LOAD(this, E6502_VEC_IRQ+1), LOAD(this, E6502_VEC_IRQ));
        /** set I */
        this->reg.status.bits.I = 1;
    }
    /** fetch & decoding */
    uint8_t op = LOAD(this, this->reg.PC++);
    const e6502_instruction_t* inst = &instructions[op];
    int mode = E6502_INS_MODE_NONE;
    uint8_t imm = 0;
    uint16_t addr = 0;
    if(inst->addressing_mode)
        mode = inst->addressing_mode(this, &imm, &addr);
    /** execute */
    if(inst->instruction)
        inst->instruction(this, mode, imm, addr);
    /** unlock states */
    if (this->cb.unlock)
        this->cb.unlock(this->cb.lock_ctx);
}

static int e6502_step(e6502_t* this)
{
    if (!this || !this->cb.store || !this->cb.load)
        return -1;
    step_internal(this);
    return 0;
}

static int e6502_run(e6502_t* this)
{
    if (!this || !this->cb.store || !this->cb.load)
        return -1;
    for(;;)
    {
        step_internal(this);
    }
    return 0;
}

/** interrupt injection */

int e6502_trigger_irq(e6502_t* this)
{
    if (!this || !this->cb.lock || !this->cb.unlock)
        return -1;
    this->cb.lock(this->cb.lock_ctx);
    if(!this->reg.status.bits.I)
    {
        this->states.irq_pending = true;
        this->reg.status.bits.B = 0;
    }
    this->cb.unlock(this->cb.lock_ctx);
    return 0;
}

int e6502_trigger_nmi(e6502_t* this)
{
    if (!this || !this->cb.lock || !this->cb.unlock)
        return -1;
    this->cb.lock(this->cb.lock_ctx);
    this->states.nmi_pending = true;
    this->reg.status.bits.B = 0;
    this->cb.unlock(this->cb.lock_ctx);
    return 0;
}

/** instruction implementations */

#define SET_Z(this, r) \
    do \
    { \
        (this)->reg.status.bits.Z = ((r) == 0); \
    } while (0);

#define SET_N(this, r) \
    do \
    { \
        (this)->reg.status.bits.N = (((r) & 0x80) != 0); \
    } while (0);

INST_DEF(LDA)
{
    uint8_t result;
    if(mode == E6502_INS_MODE_IMMEDIATE)
        result = imm;
    else if(mode == E6502_INS_MODE_ADDR)
        result = LOAD(this, addr);
    else
        return; /** invalid */
    this->reg.A = result;
    SET_N(this, result);
    SET_Z(this, result);
}
INST_DEF(LDX)
{
    uint8_t result;
    if(mode == E6502_INS_MODE_IMMEDIATE)
        result = imm;
    else if(mode == E6502_INS_MODE_ADDR)
        result = LOAD(this, addr);
    else
        return; /** invalid */
    this->reg.X = result;
    SET_N(this, result);
    SET_Z(this, result);
}
INST_DEF(LDY)
{
    uint8_t result;
    if(mode == E6502_INS_MODE_IMMEDIATE)
        result = imm;
    else if(mode == E6502_INS_MODE_ADDR)
        result = LOAD(this, addr);
    else
        return; /** invalid */
    this->reg.Y = result;
    SET_N(this, result);
    SET_Z(this, result);
}
INST_DEF(STA)
{
    if(mode == E6502_INS_MODE_ADDR)
    {
        STORE(this, addr, this->reg.A);
    }
    else if(mode==E6502_INS_MODE_IMMEDIATE)
    {
        STORE(this, imm, this->reg.A);
    }
}
INST_DEF(STX)
{
    if(mode == E6502_INS_MODE_ADDR)
    {
        STORE(this, addr, this->reg.X);
    }
    else if(mode==E6502_INS_MODE_IMMEDIATE)
    {
        STORE(this, imm, this->reg.X);
    }
}
INST_DEF(STY)
{
    if(mode == E6502_INS_MODE_ADDR)
    {
        STORE(this, addr, this->reg.Y);
    }
    else if(mode==E6502_INS_MODE_IMMEDIATE)
    {
        STORE(this, imm, this->reg.Y);
    }   
}
INST_DEF(TAX)
{
    this->reg.X = this->reg.A;
}
INST_DEF(TAY)
{
    this->reg.Y = this->reg.A;
}
INST_DEF(TSX)
{
    this->reg.X = this->reg.SP;
}
INST_DEF(TXA)
{
    this->reg.A = this->reg.X;
}
INST_DEF(TXS)
{
    this->reg.SP = this->reg.X;
}
INST_DEF(TYA)
{
    this->reg.A = this->reg.Y;
}


INST_DEF(PHA)
{
    STACK_PUSH(this, this->reg.A);
}
INST_DEF(PHP)
{
    STACK_PUSH(this, this->reg.status.byte);
}
INST_DEF(PLA)
{
    this->reg.A = STACK_POP(this);
}
INST_DEF(PLP)
{
    this->reg.status.byte = STACK_POP(this);
}


INST_DEF(DEC)
{
    if(mode != E6502_INS_MODE_ADDR)
        return;
    uint8_t result;
    result = LOAD(this, addr);
    result--;
    STORE(this, addr, result);
    SET_N(this, result);
    SET_Z(this, result);
}
INST_DEF(DEX)
{
    this->reg.X--;
}
INST_DEF(DEY)
{
    this->reg.Y--;
}
INST_DEF(INC)
{
    if(mode != E6502_INS_MODE_ADDR)
        return;
    uint8_t result;
    result = LOAD(this, addr);
    result++;
    STORE(this, addr, result);
    SET_N(this, result);
    SET_Z(this, result);
}
INST_DEF(INX)
{
    this->reg.X++;
}
INST_DEF(INY)
{
    this->reg.Y++;
}


static const uint8_t DEC_AS_BIN[] =
{
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 
    0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 
    0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 
    0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 
    0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 
    0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 
    0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 
    0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 
};
#define BCD_INVALID (0xFF)
static const uint8_t BIN_AS_DEC[] =
{
      0,   1,   2,   3,   4,   5,   6,   7,   8,   9, 255, 255, 255, 255, 255, 255, 
     10,  11,  12,  13,  14,  15,  16,  17,  18,  19, 255, 255, 255, 255, 255, 255, 
     20,  21,  22,  23,  24,  25,  26,  27,  28,  29, 255, 255, 255, 255, 255, 255, 
     30,  31,  32,  33,  34,  35,  36,  37,  38,  39, 255, 255, 255, 255, 255, 255, 
     40,  41,  42,  43,  44,  45,  46,  47,  48,  49, 255, 255, 255, 255, 255, 255, 
     50,  51,  52,  53,  54,  55,  56,  57,  58,  59, 255, 255, 255, 255, 255, 255, 
     60,  61,  62,  63,  64,  65,  66,  67,  68,  69, 255, 255, 255, 255, 255, 255, 
     70,  71,  72,  73,  74,  75,  76,  77,  78,  79, 255, 255, 255, 255, 255, 255, 
     80,  81,  82,  83,  84,  85,  86,  87,  88,  89, 255, 255, 255, 255, 255, 255, 
     90,  91,  92,  93,  94,  95,  96,  97,  98,  99, 255, 255, 255, 255, 255, 255, 
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
};
INST_DEF(ADC)
{
    if(mode==E6502_INS_MODE_NONE)
        return;
    if(!this->reg.status.bits.D)
    {
        /** binary mode */
        uint8_t A = this->reg.A;
        uint8_t B;
        if(mode==E6502_INS_MODE_IMMEDIATE)
            B = imm;
        else
            B = LOAD(this, addr);
        uint16_t buf = A+B+this->reg.status.bits.C;
        uint8_t result = buf & 0xFF;
        this->reg.status.bits.C = (buf>>8) & 0x1;
        this->reg.status.bits.V = ((!(A&0x80))&&(!(B&0x80))&&(result&0x80)) || 
                               ((A&0x80)&&(B&0x80)&&(!(result&0x80)));
        this->reg.A = result;
        SET_N(this, result);
        SET_Z(this, result);
    }
    else
    {
        /** BCD mode */
        uint8_t A = BIN_AS_DEC[this->reg.A];
        /** DO NOT handle invalid BCD value */
        if (A == BCD_INVALID)
            return;
        uint8_t B;
        if(mode==E6502_INS_MODE_IMMEDIATE)
            B = imm;
        else
            B = LOAD(this, addr);
        B = BIN_AS_DEC[B];
        /** DO NOT handle invalid BCD value */
        if (B == BCD_INVALID)
            return;
        uint16_t buf = A+B+this->reg.status.bits.C;
        uint8_t result = buf % 100;
        result = DEC_AS_BIN[result];
        this->reg.status.bits.C = buf > 99;
        this->reg.status.bits.V = buf > 99;
        this->reg.A = result;
        SET_N(this, result);
        SET_Z(this, result);
    }
}
INST_DEF(SBC)
{
    if(mode==E6502_INS_MODE_NONE)
        return;
    if(!this->reg.status.bits.D)
    {
        /** binary mode */
        uint8_t A = this->reg.A;
        uint8_t B;
        if(mode==E6502_INS_MODE_IMMEDIATE)
            B = imm;
        else
            B = LOAD(this, addr);
        uint16_t buf = A+((~B)&0xFF)+1-(!this->reg.status.bits.C);
        uint8_t result = buf & 0xFF;
        this->reg.status.bits.C = (buf>>8) & 0x1;   /** not borrow */
        this->reg.status.bits.V = ((!(A&0x80))&&(B&0x80)&&(result&0x80)) || 
                               ((A&0x80)&&(!(B&0x80))&&(!(result&0x80)));
        this->reg.A = result;
        SET_N(this, result);
        SET_Z(this, result);
    }
    else
    {
        /** BCD mode */
        uint8_t A = BIN_AS_DEC[this->reg.A];
        /** DO NOT handle invalid BCD value */
        if (A == BCD_INVALID)
            return;
        uint8_t B;
        if(mode==E6502_INS_MODE_IMMEDIATE)
            B = imm;
        else
            B = LOAD(this, addr);
        B = BIN_AS_DEC[B];
        /** DO NOT handle invalid BCD value */
        if (B == BCD_INVALID)
            return;
        uint16_t buf = A-B-(!this->reg.status.bits.C);
        uint8_t result = (buf+100) % 100;
        result = DEC_AS_BIN[result];
        this->reg.status.bits.C = A>=B; /** not borrow */
        this->reg.status.bits.V = A<B;
        this->reg.A = result;
        SET_N(this, result);
        SET_Z(this, result);
    }
}


INST_DEF(AND)
{
    if(mode == E6502_INS_MODE_NONE)
        return;
    uint8_t result;
    if(mode == E6502_INS_MODE_IMMEDIATE)
        result = this->reg.A & imm;
    else
        result = this->reg.A & LOAD(this, addr);
    this->reg.A = result;
    SET_N(this, result);
    SET_Z(this, result);
}
INST_DEF(EOR)
{
    if(mode == E6502_INS_MODE_NONE)
        return;
    uint8_t result;
    if(mode == E6502_INS_MODE_IMMEDIATE)
        result = this->reg.A ^ imm;
    else
        result = this->reg.A ^ LOAD(this, addr);
    this->reg.A = result;
    SET_N(this, result);
    SET_Z(this, result);
}
INST_DEF(ORA)
{
    if(mode == E6502_INS_MODE_NONE)
        return;
    uint8_t result;
    if(mode == E6502_INS_MODE_IMMEDIATE)
        result = this->reg.A | imm;
    else
        result = this->reg.A | LOAD(this, addr);
    this->reg.A = result;
    SET_N(this, result);
    SET_Z(this, result);
}


INST_DEF(ASL)
{
    if(mode == E6502_INS_MODE_NONE)
    {
        this->reg.status.bits.C = (this->reg.A >> 7) & 0x1;
        this->reg.A = this->reg.A << 1;
        SET_N(this, this->reg.A);
        SET_Z(this, this->reg.A);
    }
    else if(mode == E6502_INS_MODE_ADDR)
    {
        uint8_t result = LOAD(this, addr);
        this->reg.status.bits.C = (result>>7) & 0x1;
        result = result << 1;
        SET_N(this, result);
        SET_Z(this, result);
        STORE(this, addr, result);
    }
}
INST_DEF(LSR)
{
    if(mode == E6502_INS_MODE_NONE)
    {
        this->reg.status.bits.C = this->reg.A & 0x1;
        this->reg.A = this->reg.A >> 1;
        SET_N(this, this->reg.A);
        SET_Z(this, this->reg.A);
    }
    else if(mode == E6502_INS_MODE_ADDR)
    {
        uint8_t result = LOAD(this, addr);
        this->reg.status.bits.C = result & 0x1;
        result = result >> 1;
        SET_N(this, result);
        SET_Z(this, result);
        STORE(this, addr, result);
    }
}
INST_DEF(ROL)
{
    if(mode == E6502_INS_MODE_NONE)
    {
        uint16_t temp = this->reg.A;
        temp = temp << 1;
        temp |= this->reg.status.bits.C;
        this->reg.status.bits.C = (temp >> 8) & 0x1;
        this->reg.A = temp & 0xFF;
        SET_N(this, this->reg.A);
        SET_Z(this, this->reg.A);
    }
    else if(mode == E6502_INS_MODE_ADDR)
    {
        uint16_t temp = LOAD(this, addr);
        temp = temp << 1;
        temp |= this->reg.status.bits.C;
        this->reg.status.bits.C = (temp >> 8) & 0x1;
        uint8_t result = temp & 0xFF;
        SET_N(this, result);
        SET_Z(this, result);
        STORE(this, addr, result);
    }
}
INST_DEF(ROR)
{
    if(mode == E6502_INS_MODE_NONE)
    {
        uint16_t temp = (((uint16_t)this->reg.status.bits.C) << 8) | this->reg.A;
        this->reg.status.bits.C = temp & 0x1;
        temp = temp >> 1;
        this->reg.A = temp & 0xFF;
        SET_N(this, this->reg.A);
        SET_Z(this, this->reg.A);
    }
    else if(mode == E6502_INS_MODE_ADDR)
    {
        uint16_t temp = (((uint16_t)this->reg.status.bits.C) << 8) | LOAD(this, addr);
        this->reg.status.bits.C = temp & 0x1;
        temp = temp >> 1;
        uint8_t result = temp & 0xFF;
        SET_N(this, result);
        SET_Z(this, result);
        STORE(this, addr, result);
    }
}


INST_DEF(CLC)
{
    this->reg.status.bits.C = 0;
}
INST_DEF(CLD)
{
    this->reg.status.bits.D = 0;
}
INST_DEF(CLI)
{
    this->reg.status.bits.I = 0;
}
INST_DEF(CLV)
{
    this->reg.status.bits.V = 0;
}
INST_DEF(SEC)
{
    this->reg.status.bits.C = 1;
}
INST_DEF(SED)
{
    this->reg.status.bits.D = 1;
}
INST_DEF(SEI)
{
    this->reg.status.bits.I = 1;
}


INST_DEF(CMP)
{
    if(mode == E6502_INS_MODE_NONE)
        return;
    uint8_t B = (mode==E6502_INS_MODE_IMMEDIATE) ? imm : LOAD(this, addr);
    uint16_t temp = this->reg.A + ((~B)&0xFF) + 1;
    uint8_t result = temp & 0xFF;
    this->reg.status.bits.C = (temp >> 8) & 0x1;
    SET_N(this, result);
    SET_Z(this, result);
}
INST_DEF(CPX)
{
    if(mode == E6502_INS_MODE_NONE)
        return;
    uint8_t B = (mode==E6502_INS_MODE_IMMEDIATE) ? imm : LOAD(this, addr);
    uint16_t temp = this->reg.X + ((~B)&0xFF) + 1;
    uint8_t result = temp & 0xFF;
    this->reg.status.bits.C = (temp >> 8) & 0x1;
    SET_N(this, result);
    SET_Z(this, result);
}
INST_DEF(CPY)
{
    if(mode == E6502_INS_MODE_NONE)
        return;
    uint8_t B = (mode==E6502_INS_MODE_IMMEDIATE) ? imm : LOAD(this, addr);
    uint16_t temp = this->reg.Y + ((~B)&0xFF) + 1;
    uint8_t result = temp & 0xFF;
    this->reg.status.bits.C = (temp >> 8) & 0x1;
    SET_N(this, result);
    SET_Z(this, result);
}


INST_DEF(BCC)
{
    if(mode != E6502_INS_MODE_ADDR)
        return;
    if(!this->reg.status.bits.C)
        this->reg.PC = addr;
}
INST_DEF(BCS)
{
    if(mode != E6502_INS_MODE_ADDR)
        return;
    if(this->reg.status.bits.C)
        this->reg.PC = addr;
}
INST_DEF(BEQ)
{
    if(mode != E6502_INS_MODE_ADDR)
        return;
    if(this->reg.status.bits.Z)
        this->reg.PC = addr;
}
INST_DEF(BMI)
{
    if(mode != E6502_INS_MODE_ADDR)
        return;
    if(this->reg.status.bits.N)
        this->reg.PC = addr;
}
INST_DEF(BNE)
{
    if(mode != E6502_INS_MODE_ADDR)
        return;
    if(!this->reg.status.bits.Z)
        this->reg.PC = addr;
}
INST_DEF(BPL)
{
    if(mode != E6502_INS_MODE_ADDR)
        return;
    if(!this->reg.status.bits.N)
        this->reg.PC = addr;
}
INST_DEF(BVC)
{
    if(mode != E6502_INS_MODE_ADDR)
        return;
    if(!this->reg.status.bits.V)
        this->reg.PC = addr;
}
INST_DEF(BVS)
{
    if(mode != E6502_INS_MODE_ADDR)
        return;
    if(this->reg.status.bits.V)
        this->reg.PC = addr;
}


INST_DEF(JMP)
{
    if(mode != E6502_INS_MODE_ADDR)
        return;
    this->reg.PC = addr;
}
INST_DEF(JSR)
{
    if(mode != E6502_INS_MODE_ADDR)
        return;
    STORE(this, (this->reg.SP++)+E6502_STACK_OFFSET, GET_HIGH_BYTE(this->reg.PC-1));
    STORE(this, (this->reg.SP++)+E6502_STACK_OFFSET, GET_LOW_BYTE(this->reg.PC-1));
    this->reg.PC = addr;
}
INST_DEF(RTS)
{
    uint8_t l = LOAD(this, (--this->reg.SP)+E6502_STACK_OFFSET);
    uint8_t h = LOAD(this, (--this->reg.SP)+E6502_STACK_OFFSET);
    this->reg.PC = PACK_2U8(h,l)+1;
}


INST_DEF(BRK)
{
    this->states.nmi_pending = true;
    this->reg.status.bits.B = 1;
    /** push op PC+2 into stack */
    this->reg.PC++;
}
INST_DEF(RTI)
{
    this->reg.status.byte = LOAD(this, (--this->reg.SP)+E6502_STACK_OFFSET);
    uint8_t l = LOAD(this, (--this->reg.SP)+E6502_STACK_OFFSET);
    uint8_t h = LOAD(this, (--this->reg.SP)+E6502_STACK_OFFSET);
    this->reg.PC = PACK_2U8(h,l);
}


INST_DEF(BIT)
{
    if(mode != E6502_INS_MODE_ADDR)
        return;
    uint8_t B = LOAD(this, addr);
    uint8_t temp = this->reg.A & B;
    SET_Z(this, temp);
    SET_N(this, B);
    this->reg.status.bits.V = (B >> 6) & 0x1;
}
INST_DEF(NOP)
{    
}


/** addressing mode implementations */

ADDR_MODE_DEF(accumulator)
{
    return E6502_INS_MODE_NONE;
}

ADDR_MODE_DEF(absolute)
{
    uint8_t l = LOAD(this, this->reg.PC++);
    uint8_t h = LOAD(this, this->reg.PC++);
    *p_addr = PACK_2U8(h,l);
    return E6502_INS_MODE_ADDR;
}

ADDR_MODE_DEF(absolute_x)
{
    uint8_t l = LOAD(this, this->reg.PC++);
    uint8_t h = LOAD(this, this->reg.PC++);
    *p_addr = PACK_2U8(h,l) + this->reg.X;
    return E6502_INS_MODE_ADDR;
}

ADDR_MODE_DEF(absolute_y)
{
    uint8_t l = LOAD(this, this->reg.PC++);
    uint8_t h = LOAD(this, this->reg.PC++);
    *p_addr = PACK_2U8(h,l) + this->reg.Y;
    return E6502_INS_MODE_ADDR;
}

ADDR_MODE_DEF(immediate)
{
    *p_imm = LOAD(this, this->reg.PC++);
    return E6502_INS_MODE_IMMEDIATE;
}

ADDR_MODE_DEF(implied)
{
    return E6502_INS_MODE_NONE;
}

ADDR_MODE_DEF(indirect)
{
    uint8_t l = LOAD(this, this->reg.PC++);
    uint8_t h = LOAD(this, this->reg.PC++);
    uint16_t temp = PACK_2U8(h,l);
    l = LOAD(this, temp++);
    h = LOAD(this, temp);
    *p_addr = PACK_2U8(h,l);
    return E6502_INS_MODE_ADDR;
}

ADDR_MODE_DEF(x_indirect)
{
    uint8_t temp = LOAD(this, this->reg.PC++);
    temp += this->reg.X;
    uint8_t l = LOAD(this, temp++);
    uint8_t h = LOAD(this, temp);
    *p_addr = PACK_2U8(h,l);
    return E6502_INS_MODE_ADDR;
}

ADDR_MODE_DEF(indirect_y)
{
    uint8_t temp = LOAD(this, this->reg.PC++);
    uint8_t l = LOAD(this, temp++);
    uint8_t h = LOAD(this, temp);
    *p_addr = PACK_2U8(h,l) + this->reg.Y;
    return E6502_INS_MODE_ADDR;
}

ADDR_MODE_DEF(relative)
{
    uint8_t temp = LOAD(this, this->reg.PC);
    /** PC for the op code + temp */
    *p_addr = (uint16_t)(this->reg.PC - 1 + (*((int8_t*)&temp)));
    this->reg.PC++;
    return E6502_INS_MODE_ADDR;
}

ADDR_MODE_DEF(zeropage)
{
    *p_addr = LOAD(this, this->reg.PC++);
    return E6502_INS_MODE_ADDR;
}

ADDR_MODE_DEF(zeropage_x)
{
    *p_addr = (uint8_t)(LOAD(this, this->reg.PC++)+this->reg.X);
    return E6502_INS_MODE_ADDR;
}

ADDR_MODE_DEF(zeropage_y)
{
    *p_addr = (uint8_t)(LOAD(this, this->reg.PC++)+this->reg.Y);
    return E6502_INS_MODE_ADDR;
}

