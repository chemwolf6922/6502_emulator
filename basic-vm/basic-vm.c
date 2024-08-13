#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "../e6502.h"
#include "memory_layout.h"

static uint8_t memory[0x10000];
static e6502_t cpu;
bool running = true;

static uint8_t vm_load(uint16_t addr, void* ctx)
{
    return memory[addr];
}

static void vm_store(uint16_t addr, uint8_t data, void* ctx)
{
    memory[addr] = data;
    if (addr == EXIT_POINT_RC)
    {
        running = false;
    }
}

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }
    /** load program */
    
    memset(memory, 0, sizeof(memory));
    struct stat st;
    if (stat(argv[1], &st) != 0)
    {
        perror("stat");
        return 1;
    }
    if (st.st_size > ROM_SIZE + 2)
    {
        fprintf(stderr, "File too large\n");
        return 1;
    }
    FILE* f = fopen(argv[1], "r");
    if (f == NULL)
    {
        perror("fopen");
        return 1;
    }
    if (fread(memory + ROM_BASE, 1, st.st_size, f) != st.st_size)
    {
        perror("fread");
        return 1;
    }
    fclose(f);
    memory[E6502_VEC_RST] = memory[ROM_BASE + st.st_size - 2];
    memory[E6502_VEC_RST + 1] = memory[ROM_BASE + st.st_size - 1];
    memory[ROM_BASE + st.st_size - 2] = 0x00;
    memory[ROM_BASE + st.st_size - 1] = 0x00;

    /** init cpu and run */
    
    e6502_init(&cpu);
    cpu.cb.load = vm_load;
    cpu.cb.store = vm_store;
    cpu.iface.reset(&cpu);
    
    while (running)
    {
        cpu.iface.step(&cpu);
    }

    int rc = memory[EXIT_POINT_RC];

    return rc;
}

