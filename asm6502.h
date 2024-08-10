#pragma once

#define BRK() 0x00
#define ORA_XI(offset) 0x01, offset
#define ORA_ZP(addr) 0x05, addr
#define ASL_ZP(addr) 0x06, addr
#define PHP() 0x08
#define ORA_IMM(data) 0x09, data
#define ASL_A() 0x0A
#define ORA_ABS(hi, lo) 0x0D, lo, hi
#define ASL_ABS(hi, lo) 0x0E, lo, hi
#define BPL_REL(offset) 0x10, offset
#define ORA_IY(offset) 0x11, offset
#define ORA_ZPX(addr) 0x15, addr
#define ASL_ZPX(addr) 0x16, addr
#define CLC() 0x18
#define ORA_ABY(hi, lo) 0x19, lo, hi
#define ORA_ABX(hi, lo) 0x1D, lo, hi
#define ASL_ABX(hi, lo) 0x1E, lo, hi
#define JSR_ABS(hi, lo) 0x20, lo, hi
#define AND_XI(offset) 0x21, offset
#define BIT_ZP(addr) 0x24, addr
#define AND_ZP(addr) 0x25, addr
#define ROL_ZP(addr) 0x26, addr
#define PLP() 0x28
#define AND_IMM(data) 0x29, data
#define ROL_A() 0x2A
#define BIT_ABS(hi, lo) 0x2C, lo, hi
#define AND_ABS(hi, lo) 0x2D, lo, hi
#define ROL_ABS(hi, lo) 0x2E, lo, hi
#define BMI_REL(offset) 0x30, offset
#define AND_IY(offset) 0x31, offset
#define AND_ZPX(addr) 0x35, addr
#define ROL_ZPX(addr) 0x36, addr
#define SEC() 0x38
#define AND_ABY(hi, lo) 0x39, lo, hi
#define AND_ABX(hi, lo) 0x3D, lo, hi
#define ROL_ABX(hi, lo) 0x3E, lo, hi
#define RTI() 0x40
#define EOR_XI(offset) 0x41, offset
#define EOR_ZP(addr) 0x45, addr
#define LSR_ZP(addr) 0x46, addr
#define PHA() 0x48
#define EOR_IMM(data) 0x49, data
#define LSR_A() 0x4A
#define JMP_ABS(hi, lo) 0x4C, lo, hi
#define EOR_ABS(hi, lo) 0x4D, lo, hi
#define LSR_ABS(hi, lo) 0x4E, lo, hi
#define BVC_REL(offset) 0x50, offset
#define EOR_IY(offset) 0x51, offset
#define EOR_ZPX(addr) 0x55, addr
#define LSR_ZPX(addr) 0x56, addr
#define CLI() 0x58
#define EOR_ABY(hi, lo) 0x59, lo, hi
#define EOR_ABX(hi, lo) 0x5D, lo, hi
#define LSR_ABX(hi, lo) 0x5E, lo, hi
#define RTS() 0x60
#define ADC_XI(offset) 0x61, offset
#define ADC_ZP(addr) 0x65, addr
#define ROR_ZP(addr) 0x66, addr
#define PLA() 0x68
#define ADC_IMM(data) 0x69, data
#define ROR_A() 0x6A
#define JMP_IND(hi, lo) 0x6C, lo, hi
#define ADC_ABS(hi, lo) 0x6D, lo, hi
#define ROR_ABS(hi, lo) 0x6E, lo, hi
#define BVS_REL(offset) 0x70, offset
#define ADC_IY(offset) 0x71, offset
#define ADC_ZPX(addr) 0x75, addr
#define ROR_ZPX(addr) 0x76, addr
#define SEI() 0x78
#define ADC_ABY(hi, lo) 0x79, lo, hi
#define ADC_ABX(hi, lo) 0x7D, lo, hi
#define ROR_ABX(hi, lo) 0x7E, lo, hi
#define STA_XI(offset) 0x81, offset
#define STY_ZP(addr) 0x84, addr
#define STA_ZP(addr) 0x85, addr
#define STX_ZP(addr) 0x86, addr
#define DEY() 0x88
#define TXA() 0x8A
#define STY_ABS(hi, lo) 0x8C, lo, hi
#define STA_ABS(hi, lo) 0x8D, lo, hi
#define STX_ABS(hi, lo) 0x8E, lo, hi
#define BCC_REL(offset) 0x90, offset
#define STA_IY(offset) 0x91, offset
#define STY_ZPX(addr) 0x94, addr
#define STA_ZPX(addr) 0x95, addr
#define STX_ZPY(addr) 0x96, addr
#define TYA() 0x98
#define STA_ABY(hi, lo) 0x99, lo, hi
#define TXS() 0x9A
#define STA_ABX(hi, lo) 0x9D, lo, hi
#define LDY_IMM(data) 0xA0, data
#define LDA_XI(offset) 0xA1, offset
#define LDX_IMM(data) 0xA2, data
#define LDY_ZP(addr) 0xA4, addr
#define LDA_ZP(addr) 0xA5, addr
#define LDX_ZP(addr) 0xA6, addr
#define TAY() 0xA8
#define LDA_IMM(data) 0xA9, data
#define TAX() 0xAA
#define LDY_ABS(hi, lo) 0xAC, lo, hi
#define LDA_ABS(hi, lo) 0xAD, lo, hi
#define LDX_ABS(hi, lo) 0xAE, lo, hi
#define BCS_REL(offset) 0xB0, offset
#define LDA_IY(offset) 0xB1, offset
#define LDY_ZPX(addr) 0xB4, addr
#define LDA_ZPX(addr) 0xB5, addr
#define LDX_ZPY(addr) 0xB6, addr
#define CLV() 0xB8
#define LDA_ABY(hi, lo) 0xB9, lo, hi
#define TSX() 0xBA
#define LDY_ABX(hi, lo) 0xBC, lo, hi
#define LDA_ABX(hi, lo) 0xBD, lo, hi
#define LDX_ABY(hi, lo) 0xBE, lo, hi
#define CPY_IMM(data) 0xC0, data
#define CMP_XI(offset) 0xC1, offset
#define CPY_ZP(addr) 0xC4, addr
#define CMP_ZP(addr) 0xC5, addr
#define DEC_ZP(addr) 0xC6, addr
#define INY() 0xC8
#define CMP_IMM(data) 0xC9, data
#define DEX() 0xCA
#define CPY_ABS(hi, lo) 0xCC, lo, hi
#define CMP_ABS(hi, lo) 0xCD, lo, hi
#define DEC_ABS(hi, lo) 0xCE, lo, hi
#define BNE_REL(offset) 0xD0, offset
#define CMP_IY(offset) 0xD1, offset
#define CMP_ZPX(addr) 0xD5, addr
#define DEC_ZPX(addr) 0xD6, addr
#define CLD() 0xD8
#define CMP_ABY(hi, lo) 0xD9, lo, hi
#define CMP_ABX(hi, lo) 0xDD, lo, hi
#define DEC_ABX(hi, lo) 0xDE, lo, hi
#define CPX_IMM(data) 0xE0, data
#define SBC_XI(offset) 0xE1, offset
#define CPX_ZP(addr) 0xE4, addr
#define SBC_ZP(addr) 0xE5, addr
#define INC_ZP(addr) 0xE6, addr
#define INX() 0xE8
#define SBC_IMM(data) 0xE9, data
#define NOP() 0xEA
#define CPX_ABS(hi, lo) 0xEC, lo, hi
#define SBC_ABS(hi, lo) 0xED, lo, hi
#define INC_ABS(hi, lo) 0xEE, lo, hi
#define BEQ_REL(offset) 0xF0, offset
#define SBC_IY(offset) 0xF1, offset
#define SBC_ZPX(addr) 0xF5, addr
#define INC_ZPX(addr) 0xF6, addr
#define SED() 0xF8
#define SBC_ABY(hi, lo) 0xF9, lo, hi
#define SBC_ABX(hi, lo) 0xFD, lo, hi
#define INC_ABX(hi, lo) 0xFE, lo, hi
