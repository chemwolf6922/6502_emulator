#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "../../e6502.h"
#include "../../asm6502.h"

void test_dump_cpu(e6502_t* cpu);

void test_store(uint16_t addr, uint8_t data, void* ctx);

uint8_t test_load(uint16_t addr, void* ctx);

char** program_to_strs(const char* program_const, int program_len);

void free_program_strs(char** strs);

#define VALIDATE_REG(name, value) \
    do \
    { \
        if (cpu.reg.name == value) \
        { \
            break; \
        } \
        test_pass = false; \
        printf("FAIL: reg %s: expected %X, actual %X\n", #name, value, cpu.reg.name); \
    } while (0)

#define VALIDATE_MEM(addr, value) \
    do \
    { \
        if (memory[addr] == value) \
        { \
            break; \
        } \
        test_pass = false; \
        printf("FAIL: mem %04X: expected %02X, actual %02X\n", addr, value, memory[addr]); \
    } while (0)

#define VALIDATE_FLAG(name, value) \
    do \
    { \
        if (cpu.reg.status.bits.name == value) \
        { \
            break; \
        } \
        test_pass = false; \
        printf("FAIL: flag %s: expected %d, actual %d\n", #name, value, cpu.reg.status.bits.name); \
    } while (0)

#define PROGRAM_ENTRY (0xF000)

#define PROGRAM(...) \
    const char* program_str = #__VA_ARGS__; \
    uint8_t program[] = {__VA_ARGS__};

#define RUN_TEST(steps) \
    memcpy(memory + PROGRAM_ENTRY, program, sizeof(program)); \
    char** strs = program_to_strs(program_str, sizeof(program)); \
    assert(strs); \
    printf("\nStart test: %s\n", test_name); \
    cpu.iface.reset(&cpu); \
    for(int i = 0; i < steps; i ++) \
    { \
        printf("\nStep %d\n", i); \
        int rel_pc = cpu.reg.PC - PROGRAM_ENTRY; \
        if (rel_pc >= 0 && rel_pc < sizeof(program)) \
        { \
            char* str = strs[rel_pc]; \
            printf("@%04X: %s\n", cpu.reg.PC, str ? str : "INVALID"); \
        } \
        else \
        { \
            printf("@%04X: %s\n", cpu.reg.PC, "INVALID"); \
        } \
        cpu.iface.step(&cpu); \
        test_dump_cpu(&cpu); \
    } \
    free_program_strs(strs);

#define DEFINE_TEST(name, ...) \
    do \
    { \
        char* test_name = #name; \
        bool test_pass = true; \
        uint8_t memory[0x10000]; \
        memset(memory, 0, sizeof(memory)); \
        memory[E6502_VEC_RST + 1] = PROGRAM_ENTRY >> 8; \
        memory[E6502_VEC_RST] = PROGRAM_ENTRY & 0xFF; \
        e6502_t cpu; \
        e6502_init(&cpu); \
        cpu.cb.store = test_store; \
        cpu.cb.load = test_load; \
        cpu.cb.bus_ctx = memory; \
        __VA_ARGS__; \
        if (test_pass) \
        { \
            printf("\033[0;32m"); \
        } \
        else \
        { \
            printf("\033[0;31m"); \
        } \
        printf("\nTest %s %s\n", test_name, test_pass ? "PASS" : "FAIL"); \
        printf("\033[0m"); \
    } while(0)
