#include "test_framework.h"

TEST_MAIN(
    DEFINE_TEST(STX absolute,
        PROGRAM(
            LDX_IMM(0x12),
            STX_ABS(0x10, 0x00),
        );
        RUN_TEST(2);
        VALIDATE_MEM(0x1000, 0x12);
    );
    DEFINE_TEST(STX zero_page,
        PROGRAM(
            LDX_IMM(0x12),
            STX_ZP(0x10),
        );
        RUN_TEST(2);
        VALIDATE_MEM(0x10, 0x12);
    );
    DEFINE_TEST(STX zero_page_y,
        PROGRAM(
            LDX_IMM(0x12),
            LDY_IMM(0x01),
            STX_ZPY(0x0F),
        );
        RUN_TEST(3);
        VALIDATE_MEM(0x10, 0x12);
    );
)
