MEMORY {
    zp : ORIGIN = __rc31 + 1, LENGTH = 0x100 - (__rc31 + 1)
    ram (rw) : ORIGIN = 0x200, LENGTH = 0x7e00 
    peripheral (rw) : ORIGIN = 0x8000, LENGTH = 0x1000
    rom (r) : ORIGIN = 0x9000, LENGTH = 0x7000 - 6
}

REGION_ALIAS("c_readonly", rom)
REGION_ALIAS("c_writeable", ram)

SECTIONS { INCLUDE c.ld }

__rc0 = 0x00;
INCLUDE imag-regs.ld
ASSERT(__rc0 == 0x00, "Inconsistent zero page map.")
ASSERT(__rc31 == 0x1f, "Inconsistent zero page map.")

__stack = 0x8000;

OUTPUT_FORMAT { TRIM(rom) SHORT(_start) }
