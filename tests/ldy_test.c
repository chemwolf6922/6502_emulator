#include "test_framework.h"

TEST_MAIN(
    DEFINE_TEST(LDY immediate,
        PROGRAM( 
            LDY_IMM(0x12),
        );
        RUN_TEST(1);
        VALIDATE_REG(Y, 0x12);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(LDY absolute,
        memory[0x1000] = 0x12;
        PROGRAM(
            LDY_ABS(0x10, 0x00),
        );
        RUN_TEST(1);
        VALIDATE_REG(Y, 0x12);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(LDY zero_page,
        memory[0x10] = 0x12;
        PROGRAM(
            LDY_ZP(0x10),
        );
        RUN_TEST(1);
        VALIDATE_REG(Y, 0x12);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(LDY absolute_x,
        memory[0x1001] = 0x12;
        PROGRAM(
            LDX_IMM(0x01),
            LDY_ABX(0x10, 0x00),
        );
        RUN_TEST(2);
        VALIDATE_REG(Y, 0x12);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(LDY zero_page_x,
        memory[0x10] = 0x12;
        PROGRAM(
            LDX_IMM(0x01),
            LDY_ZPX(0x0F),
        );
        RUN_TEST(2);
        VALIDATE_REG(Y, 0x12);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(LDY N flag,
        PROGRAM(
            LDY_IMM(0x80),
        );
        RUN_TEST(1);
        VALIDATE_FLAG(N, 1);
        VALIDATE_FLAG(Z, 0);
    );
    DEFINE_TEST(LDY Z flag,
        PROGRAM(
            LDY_IMM(0x00),
        );
        RUN_TEST(1);
        VALIDATE_FLAG(Z, 1);
        VALIDATE_FLAG(N, 0);
    );
)
