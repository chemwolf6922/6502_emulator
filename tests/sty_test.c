#include "test_framework.h"

TEST_MAIN(
    DEFINE_TEST(STY absolute,
        PROGRAM(
            LDY_IMM(0x12),
            STY_ABS(0x10, 0x00),
        );
        RUN_TEST(2);
        VALIDATE_MEM(0x1000, 0x12);
    );
    DEFINE_TEST(STY zero_page,
        PROGRAM(
            LDY_IMM(0x12),
            STY_ZP(0x10),
        );
        RUN_TEST(2);
        VALIDATE_MEM(0x10, 0x12);
    );
    DEFINE_TEST(STY zero_page_x,
        PROGRAM(
            LDY_IMM(0x12),
            LDX_IMM(0x01),
            STY_ZPX(0x0F),
        );
        RUN_TEST(3);
        VALIDATE_MEM(0x10, 0x12);
    );
)
