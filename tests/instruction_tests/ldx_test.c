#include "test_framework.h"

TEST_MAIN(
    DEFINE_TEST(LDX immediate,
        PROGRAM( 
            LDX_IMM(0x12),
        );
        RUN_TEST(1);
        VALIDATE_REG(X, 0x12);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(LDX absolute,
        memory[0x1000] = 0x12;
        PROGRAM(
            LDX_ABS(0x10, 0x00),
        );
        RUN_TEST(1);
        VALIDATE_REG(X, 0x12);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(LDX zero_page,
        memory[0x10] = 0x12;
        PROGRAM(
            LDX_ZP(0x10),
        );
        RUN_TEST(1);
        VALIDATE_REG(X, 0x12);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(LDX absolute_y,
        memory[0x1001] = 0x12;
        PROGRAM(
            LDY_IMM(0x01),
            LDX_ABY(0x10, 0x00),
        );
        RUN_TEST(2);
        VALIDATE_REG(X, 0x12);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(LDX zero_page_y,
        memory[0x10] = 0x12;
        PROGRAM(
            LDY_IMM(0x01),
            LDX_ZPY(0x0F),
        );
        RUN_TEST(2);
        VALIDATE_REG(X, 0x12);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(LDX N flag,
        PROGRAM(
            LDX_IMM(0x80),
        );
        RUN_TEST(1);
        VALIDATE_REG(X, 0x80);
        VALIDATE_FLAG(N, 1);
    );
    DEFINE_TEST(LDX Z flag,
        PROGRAM(
            LDX_IMM(0x00),
        );
        RUN_TEST(1);
        VALIDATE_REG(X, 0x00);
        VALIDATE_FLAG(Z, 1);
    );
)
