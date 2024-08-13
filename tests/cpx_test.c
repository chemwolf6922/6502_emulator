#include "test_framework.h"

TEST_MAIN(
    DEFINE_TEST(CPX immediate,
        PROGRAM(
            LDX_IMM(0x01),
            CPX_IMM(0x01),
        );
        RUN_TEST(2);
        VALIDATE_REG(X, 0x01);
        VALIDATE_FLAG(C, 1);
        VALIDATE_FLAG(Z, 1);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(CPX zeropage,
        memory[0x10] = 0x01;
        PROGRAM(
            LDX_IMM(0x01),
            CPX_ZP(0x10),
        );
        RUN_TEST(2);
        VALIDATE_REG(X, 0x01);
        VALIDATE_FLAG(C, 1);
        VALIDATE_FLAG(Z, 1);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(CPX absolute,
        memory[0x1000] = 0x01;
        PROGRAM(
            LDX_IMM(0x01),
            CPX_ABS(0x10, 0x00),
        );
        RUN_TEST(2);
        VALIDATE_REG(X, 0x01);
        VALIDATE_FLAG(C, 1);
        VALIDATE_FLAG(Z, 1);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(CPX >,
        PROGRAM(
            LDX_IMM(0x02),
            CPX_IMM(0x01),
        );
        RUN_TEST(2);
        VALIDATE_REG(X, 0x02);
        VALIDATE_FLAG(C, 1);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(CPX <,
        PROGRAM(
            LDX_IMM(0x01),
            CPX_IMM(0x02),
        );
        RUN_TEST(2);
        VALIDATE_REG(X, 0x01);
        VALIDATE_FLAG(C, 0);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 1);
    );
    DEFINE_TEST(CPX =,
        PROGRAM(
            LDX_IMM(0x01),
            CPX_IMM(0x01),
        );
        RUN_TEST(2);
        VALIDATE_REG(X, 0x01);
        VALIDATE_FLAG(C, 1);
        VALIDATE_FLAG(Z, 1);
        VALIDATE_FLAG(N, 0);
    )
)
