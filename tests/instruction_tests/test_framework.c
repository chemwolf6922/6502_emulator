#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "../../e6502.h"

void test_dump_cpu(e6502_t* cpu)
{
    printf("A:%02X X:%02X Y:%02X SP:%02X PC:%04X\n",
        cpu->reg.A, cpu->reg.X, cpu->reg.Y, cpu->reg.SP, cpu->reg.PC);
    printf("NV-BDIZC\n");
    printf("%d%d%d%d%d%d%d%d\n",
        cpu->reg.status.bits.N,
        cpu->reg.status.bits.V,
        cpu->reg.status.bits._,
        cpu->reg.status.bits.B,
        cpu->reg.status.bits.D,
        cpu->reg.status.bits.I,
        cpu->reg.status.bits.Z,
        cpu->reg.status.bits.C);
}

void test_store(uint16_t addr, uint8_t data, void* ctx)
{
    printf("store: %04X <- %02X\n", addr, data);
    uint8_t* memory = (uint8_t*)ctx;
    memory[addr] = data;
}

uint8_t test_load(uint16_t addr, void* ctx)
{
    
    uint8_t* memory = (uint8_t*)ctx;
    uint8_t data = memory[addr];
    printf("load:  %04X -> %02X\n", addr, data);
    return data;
}

char** program_to_strs(const char* program_const, int program_len)
{
    char** strs = malloc(sizeof(char*) * program_len + strlen(program_const) + 1);
    if (!strs)
        return NULL;
    memset(strs, 0, sizeof(char*) * program_len);
    memcpy(strs + program_len, program_const, strlen(program_const) + 1);
    char* program = (char*)(strs + program_len);

    int i_inst_start = 0;
    int total_len = 0;
    int inst_len = 0;
    bool inside_bracket = false;
    for (int i = 0; program[i]; i ++)
    {
        char c = program[i];
        switch (c)
        {
        case '(':
            inside_bracket = true;
            inst_len = 1;
            break;
        case ')':
            inside_bracket = false;
            if (total_len >= program_len)
            {
                free(strs);
                return NULL;
            }
            strs[total_len] = program + i_inst_start;
            total_len += inst_len;
            inst_len = 0;
            break;
        case 'x':
            /** x for 0x */
            if (inside_bracket)
            {
                inst_len ++;
            }
            break;
        case ',':
            if (!inside_bracket)
            {
                i_inst_start = i + 1;
                program[i] = '\0';
            }
            break;
        default:
            break;
        }
    }
    return strs;
}

void free_program_strs(char** strs)
{
    free((void*)strs);
}