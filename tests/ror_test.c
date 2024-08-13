#include "test_framework.h"

TEST_MAIN(
    DEFINE_TEST(ROR accumulator,
        PROGRAM(
            SEC(),
            LDA_IMM(0x02),
            ROR_A(),
        );
        RUN_TEST(3);
        VALIDATE_REG(A, 0x81);
        VALIDATE_FLAG(C, 0);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 1);
    );
    DEFINE_TEST(ROR zeropage,
        memory[0x10] = 0x02;
        PROGRAM(
            SEC(),
            ROR_ZP(0x10),
        );
        RUN_TEST(2);
        VALIDATE_MEM(0x10, 0x81);
        VALIDATE_FLAG(C, 0);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 1);
    );
    DEFINE_TEST(ROR zeropage X,
        memory[0x10] = 0x02;
        PROGRAM(
            SEC(),
            LDX_IMM(0x01),
            ROR_ZPX(0x0F),
        );
        RUN_TEST(3);
        VALIDATE_MEM(0x10, 0x81);
        VALIDATE_FLAG(C, 0);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 1);
    );
    DEFINE_TEST(ROR absolute,
        memory[0x1000] = 0x02;
        PROGRAM(
            SEC(),
            ROR_ABS(0x10, 0x00),
        );
        RUN_TEST(2);
        VALIDATE_MEM(0x1000, 0x81);
        VALIDATE_FLAG(C, 0);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 1);
    );
    DEFINE_TEST(ROR absolute X,
        memory[0x1000] = 0x02;
        PROGRAM(
            SEC(),
            LDX_IMM(0x01),
            ROR_ABX(0x0F, 0xFF),
        );
        RUN_TEST(3);
        VALIDATE_MEM(0x1000, 0x81);
        VALIDATE_FLAG(C, 0);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 1);
    );
    DEFINE_TEST(ROR C flag,
        PROGRAM(
            SEC(),
            LDA_IMM(0x03),
            ROR_A(),
        );
        RUN_TEST(3);
        VALIDATE_REG(A, 0x81);
        VALIDATE_FLAG(C, 1);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 1);
    );
    DEFINE_TEST(ROR Z flag,
        memory[0x10] = 0x01;
        PROGRAM(
            ROR_ZP(0x10),
        );
        RUN_TEST(1);
        VALIDATE_MEM(0x10, 0x00);
        VALIDATE_FLAG(C, 1);
        VALIDATE_FLAG(Z, 1);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(ROR N flag,
        memory[0x10] = 0x80;
        PROGRAM(
            SEC(),
            ROR_ZP(0x10),
        );
        RUN_TEST(2);
        VALIDATE_MEM(0x10, 0xC0);
        VALIDATE_FLAG(C, 0);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 1);
    );
)
