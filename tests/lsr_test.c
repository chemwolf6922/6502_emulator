#include "test_framework.h"

TEST_MAIN(
    DEFINE_TEST(LSR accumulator,
        PROGRAM(
            LDA_IMM(0x02),
            LSR_A(),
        );
        RUN_TEST(2);
        VALIDATE_REG(A, 0x01);
        VALIDATE_FLAG(C, 0);
        VALIDATE_FLAG(Z, 0);
    );
    DEFINE_TEST(LSR zeropage,
        memory[0x10] = 0x02;
        PROGRAM(
            LSR_ZP(0x10),
        );
        RUN_TEST(1);
        VALIDATE_MEM(0x10, 0x01);
        VALIDATE_FLAG(C, 0);
        VALIDATE_FLAG(Z, 0);
    );
    DEFINE_TEST(LSR zeropage X,
        memory[0x10] = 0x02;
        PROGRAM(
            LDX_IMM(0x01),
            LSR_ZPX(0x0F),
        );
        RUN_TEST(2);
        VALIDATE_MEM(0x10, 0x01);
        VALIDATE_FLAG(C, 0);
        VALIDATE_FLAG(Z, 0);
    );
    DEFINE_TEST(LSR absolute,
        memory[0x1000] = 0x02;
        PROGRAM(
            LSR_ABS(0x10, 0x00),
        );
        RUN_TEST(1);
        VALIDATE_MEM(0x1000, 0x01);
        VALIDATE_FLAG(C, 0);
        VALIDATE_FLAG(Z, 0);
    );
    DEFINE_TEST(LSR absolute X,
        memory[0x1000] = 0x02;
        PROGRAM(
            LDX_IMM(0x01),
            LSR_ABX(0x0F, 0xFF),
        );
        RUN_TEST(2);
        VALIDATE_MEM(0x1000, 0x01);
        VALIDATE_FLAG(C, 0);
        VALIDATE_FLAG(Z, 0);
    );
    DEFINE_TEST(LSR C flag,
        PROGRAM(
            LDA_IMM(0x03),
            LSR_A(),
        );
        RUN_TEST(2);
        VALIDATE_REG(A, 0x01);
        VALIDATE_FLAG(C, 1);
        VALIDATE_FLAG(Z, 0);
    );
    DEFINE_TEST(LSR Z flag,
        memory[0x10] = 0x00;
        PROGRAM(
            LSR_ZP(0x10),
        );
        RUN_TEST(1);
        VALIDATE_MEM(0x10, 0x00);
        VALIDATE_FLAG(C, 0);
        VALIDATE_FLAG(Z, 1);
    );
)
