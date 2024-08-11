#pragma once

#include "peripheral.h"

#define MEM_SIZE (0x10000)
#define MEM_BASE (0x0000)

#define RAM_SIZE (0x8000)
#define RAM_BASE (MEM_BASE + 0x0000)

/** 6: the three entry point vectors. */
#define ROM_SIZE (0x7000 - 6)
#define ROM_BASE (PERIPHERAL_BASE + PERIPHERAL_SIZE)
