#include "test_framework.h"

TEST_MAIN(
    DEFINE_TEST(ROL accumulator,
        PROGRAM(
            SEC(),
            LDA_IMM(0x02),
            ROL_A(),
        );
        RUN_TEST(3);
        VALIDATE_REG(A, 0x05);
        VALIDATE_FLAG(C, 0);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(ROL zeropage,
        memory[0x10] = 0x02;
        PROGRAM(
            SEC(),
            ROL_ZP(0x10),
        );
        RUN_TEST(2);
        VALIDATE_MEM(0x10, 0x05);
        VALIDATE_FLAG(C, 0);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(ROL zeropage X,
        memory[0x10] = 0x02;
        PROGRAM(
            SEC(),
            LDX_IMM(0x01),
            ROL_ZPX(0x0F),
        );
        RUN_TEST(3);
        VALIDATE_MEM(0x10, 0x05);
        VALIDATE_FLAG(C, 0);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(ROL absolute,
        memory[0x1000] = 0x02;
        PROGRAM(
            SEC(),
            ROL_ABS(0x10, 0x00),
        );
        RUN_TEST(2);
        VALIDATE_MEM(0x1000, 0x05);
        VALIDATE_FLAG(C, 0);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(ROL absolute X,
        memory[0x1000] = 0x02;
        PROGRAM(
            SEC(),
            LDX_IMM(0x01),
            ROL_ABX(0x0F, 0xFF),
        );
        RUN_TEST(3);
        VALIDATE_MEM(0x1000, 0x05);
        VALIDATE_FLAG(C, 0);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(ROL C flag,
        PROGRAM(
            SEC(),
            LDA_IMM(0x80),
            ROL_A(),
        );
        RUN_TEST(3);
        VALIDATE_REG(A, 0x01);
        VALIDATE_FLAG(C, 1);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(ROL Z flag,
        PROGRAM(
            LDA_IMM(0x00),
            ROL_A(),
        );
        RUN_TEST(2);
        VALIDATE_REG(A, 0x00);
        VALIDATE_FLAG(C, 0);
        VALIDATE_FLAG(Z, 1);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(ROL N flag,
        PROGRAM(
            LDA_IMM(0x40),
            ROL_A(),
        );
        RUN_TEST(2);
        VALIDATE_REG(A, 0x80);
        VALIDATE_FLAG(C, 0);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 1);
    );
)
