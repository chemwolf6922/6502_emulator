#include "test_framework.h"

TEST_MAIN(
    DEFINE_TEST(EOR immediate,
        PROGRAM(
            LDA_IMM(0x0F),
            EOR_IMM(0x03),
        );
        RUN_TEST(2);
        VALIDATE_REG(A, 0x0C);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(EOR zeropage,
        memory[0x10] = 0x0F;
        PROGRAM(
            LDA_IMM(0x03),
            EOR_ZP(0x10),
        );
        RUN_TEST(2);
        VALIDATE_REG(A, 0x0C);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(EOR zeropage X,
        memory[0x10] = 0x0F;
        PROGRAM(
            LDA_IMM(0x03),
            LDX_IMM(0x01),
            EOR_ZPX(0x0F),
        );
        RUN_TEST(3);
        VALIDATE_REG(A, 0x0C);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(EOR absolute,
        memory[0x1000] = 0x0F;
        PROGRAM(
            LDA_IMM(0x03),
            EOR_ABS(0x10, 0x00),
        );
        RUN_TEST(2);
        VALIDATE_REG(A, 0x0C);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(EOR absolute X,
        memory[0x1000] = 0x0F;
        PROGRAM(
            LDA_IMM(0x03),
            LDX_IMM(0x01),
            EOR_ABX(0x0F, 0xFF),
        );
        RUN_TEST(3);
        VALIDATE_REG(A, 0x0C);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(EOR absolute Y,
        memory[0x1000] = 0x0F;
        PROGRAM(
            LDA_IMM(0x03),
            LDY_IMM(0x01),
            EOR_ABY(0x0F, 0xFF),
        );
        RUN_TEST(3);
        VALIDATE_REG(A, 0x0C);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(EOR X indirect,
        memory[0x10] = 0x00;
        memory[0x11] = 0x10;
        memory[0x1000] = 0x0F;
        PROGRAM(
            LDA_IMM(0x03),
            LDX_IMM(0x01),
            EOR_XI(0x0F),
        );
        RUN_TEST(3);
        VALIDATE_REG(A, 0x0C);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(EOR indirect Y,
        memory[0x10] = 0x00;
        memory[0x11] = 0x10;
        memory[0x1010] = 0x0F;
        PROGRAM(
            LDA_IMM(0x03),
            LDY_IMM(0x10),
            EOR_IY(0x10),
        );
        RUN_TEST(3);
        VALIDATE_REG(A, 0x0C);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(EOR Z flag,
        PROGRAM(
            EOR_IMM(0x00),
        );
        RUN_TEST(1);
        VALIDATE_REG(A, 0x00);
        VALIDATE_FLAG(Z, 1);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(EOR N flag,
        PROGRAM(
            LDA_IMM(0x80),
            EOR_IMM(0x00),
        );
        RUN_TEST(2);
        VALIDATE_REG(A, 0x80);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 1);
    );
)
