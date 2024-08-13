#include "test_framework.h"

TEST_MAIN(
    DEFINE_TEST(CPY immediate,
        PROGRAM(
            LDY_IMM(0x01),
            CPY_IMM(0x01),
        );
        RUN_TEST(2);
        VALIDATE_REG(Y, 0x01);
        VALIDATE_FLAG(C, 1);
        VALIDATE_FLAG(Z, 1);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(CPY zeropage,
        memory[0x10] = 0x01;
        PROGRAM(
            LDY_IMM(0x01),
            CPY_ZP(0x10),
        );
        RUN_TEST(2);
        VALIDATE_REG(Y, 0x01);
        VALIDATE_FLAG(C, 1);
        VALIDATE_FLAG(Z, 1);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(CPY absolute,
        memory[0x1000] = 0x01;
        PROGRAM(
            LDY_IMM(0x01),
            CPY_ABS(0x10, 0x00),
        );
        RUN_TEST(2);
        VALIDATE_REG(Y, 0x01);
        VALIDATE_FLAG(C, 1);
        VALIDATE_FLAG(Z, 1);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(CPY >,
        PROGRAM(
            LDY_IMM(0x02),
            CPY_IMM(0x01),
        );
        RUN_TEST(2);
        VALIDATE_REG(Y, 0x02);
        VALIDATE_FLAG(C, 1);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(CPY <,
        PROGRAM(
            LDY_IMM(0x01),
            CPY_IMM(0x02),
        );
        RUN_TEST(2);
        VALIDATE_REG(Y, 0x01);
        VALIDATE_FLAG(C, 0);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 1);
    );
    DEFINE_TEST(CPY =,
        PROGRAM(
            LDY_IMM(0x01),
            CPY_IMM(0x01),
        );
        RUN_TEST(2);
        VALIDATE_REG(Y, 0x01);
        VALIDATE_FLAG(C, 1);
        VALIDATE_FLAG(Z, 1);
        VALIDATE_FLAG(N, 0);
    )
)
