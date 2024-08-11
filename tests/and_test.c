#include "test_framework.h"

TEST_MAIN(
    DEFINE_TEST(AND immediate,
        PROGRAM(
            LDA_IMM(0x0F),
            AND_IMM(0x03),
        );
        RUN_TEST(2);
        VALIDATE_REG(A, 0x03);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(AND zeropage,
        memory[0x10] = 0x0F;
        PROGRAM(
            LDA_IMM(0x03),
            AND_ZP(0x10),
        );
        RUN_TEST(2);
        VALIDATE_REG(A, 0x03);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(AND zeropage X,
        memory[0x10] = 0x0F;
        PROGRAM(
            LDA_IMM(0x03),
            LDX_IMM(0x01),
            AND_ZPX(0x0F),
        );
        RUN_TEST(3);
        VALIDATE_REG(A, 0x03);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(AND absolute,
        memory[0x1000] = 0x0F;
        PROGRAM(
            LDA_IMM(0x03),
            AND_ABS(0x10, 0x00),
        );
        RUN_TEST(2);
        VALIDATE_REG(A, 0x03);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(AND absolute X,
        memory[0x1000] = 0x0F;
        PROGRAM(
            LDA_IMM(0x03),
            LDX_IMM(0x01),
            AND_ABX(0x0F, 0xFF),
        );
        RUN_TEST(3);
        VALIDATE_REG(A, 0x03);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(AND absolute Y,
        memory[0x1000] = 0x0F;
        PROGRAM(
            LDA_IMM(0x03),
            LDY_IMM(0x01),
            AND_ABY(0x0F, 0xFF),
        );
        RUN_TEST(3);
        VALIDATE_REG(A, 0x03);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(AND X indirect,
        memory[0x10] = 0x00;
        memory[0x11] = 0x10;
        memory[0x1000] = 0x0F;
        PROGRAM(
            LDA_IMM(0x03),
            LDX_IMM(0x01),
            AND_XI(0x0F),
        );
        RUN_TEST(3);
        VALIDATE_REG(A, 0x03);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(AND indirect Y,
        memory[0x10] = 0x00;
        memory[0x11] = 0x10;
        memory[0x1010] = 0x0F;
        PROGRAM(
            LDA_IMM(0x03),
            LDY_IMM(0x10),
            AND_IY(0x10),
        );
        RUN_TEST(3);
        VALIDATE_REG(A, 0x03);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(AND Z flag,
        PROGRAM(
            LDA_IMM(0xF0),
            AND_IMM(0x0F),
        );
        RUN_TEST(2);
        VALIDATE_REG(A, 0x00);
        VALIDATE_FLAG(Z, 1);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(AND N flag,
        PROGRAM(
            LDA_IMM(0x80),
            AND_IMM(0x80),
        );
        RUN_TEST(2);
        VALIDATE_REG(A, 0x80);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 1);
    );
)
