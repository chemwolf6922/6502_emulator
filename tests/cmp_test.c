#include "test_framework.h"

TEST_MAIN(
    DEFINE_TEST(CMP immediate,
        PROGRAM(
            LDA_IMM(0x01),
            CMP_IMM(0x01),
        );
        RUN_TEST(2);
        VALIDATE_REG(A, 0x01);
        VALIDATE_FLAG(C, 1);
        VALIDATE_FLAG(Z, 1);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(CMP zeropage,
        memory[0x10] = 0x01;
        PROGRAM(
            LDA_IMM(0x01),
            CMP_ZP(0x10),
        );
        RUN_TEST(2);
        VALIDATE_REG(A, 0x01);
        VALIDATE_FLAG(C, 1);
        VALIDATE_FLAG(Z, 1);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(CMP zeropage X,
        memory[0x10] = 0x01;
        PROGRAM(
            LDA_IMM(0x01),
            LDX_IMM(0x01),
            CMP_ZPX(0x0F),
        );
        RUN_TEST(3);
        VALIDATE_REG(A, 0x01);
        VALIDATE_FLAG(C, 1);
        VALIDATE_FLAG(Z, 1);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(CMP absolute,
        memory[0x1000] = 0x01;
        PROGRAM(
            LDA_IMM(0x01),
            CMP_ABS(0x10, 0x00),
        );
        RUN_TEST(2);
        VALIDATE_REG(A, 0x01);
        VALIDATE_FLAG(C, 1);
        VALIDATE_FLAG(Z, 1);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(CMP absolute X,
        memory[0x1000] = 0x01;
        PROGRAM(
            LDA_IMM(0x01),
            LDX_IMM(0x01),
            CMP_ABX(0x0F, 0xFF),
        );
        RUN_TEST(3);
        VALIDATE_REG(A, 0x01);
        VALIDATE_FLAG(C, 1);
        VALIDATE_FLAG(Z, 1);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(CMP absolute Y,
        memory[0x1000] = 0x01;
        PROGRAM(
            LDA_IMM(0x01),
            LDY_IMM(0x01),
            CMP_ABY(0x0F, 0xFF),
        );
        RUN_TEST(3);
        VALIDATE_REG(A, 0x01);
        VALIDATE_FLAG(C, 1);
        VALIDATE_FLAG(Z, 1);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(CMP X indirect,
        memory[0x10] = 0x00;
        memory[0x11] = 0x10;
        memory[0x1000] = 0x01;
        PROGRAM(
            LDA_IMM(0x01),
            LDX_IMM(0x01),
            CMP_XI(0x0F),
        );
        RUN_TEST(3);
        VALIDATE_REG(A, 0x01);
        VALIDATE_FLAG(C, 1);
        VALIDATE_FLAG(Z, 1);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(CMP indirect Y,
        memory[0x10] = 0x00;
        memory[0x11] = 0x10;
        memory[0x1010] = 0x01;
        PROGRAM(
            LDA_IMM(0x01),
            LDY_IMM(0x10),
            CMP_IY(0x10),
        );
        RUN_TEST(3);
        VALIDATE_REG(A, 0x01);
        VALIDATE_FLAG(C, 1);
        VALIDATE_FLAG(Z, 1);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(CMP >,
        PROGRAM(
            LDA_IMM(0x02),
            CMP_IMM(0x01),
        );
        RUN_TEST(2);
        VALIDATE_REG(A, 0x02);
        VALIDATE_FLAG(C, 1);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(CMP <,
        PROGRAM(
            LDA_IMM(0x01),
            CMP_IMM(0x02),
        );
        RUN_TEST(2);
        VALIDATE_REG(A, 0x01);
        VALIDATE_FLAG(C, 0);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 1);
    );
    DEFINE_TEST(CMP =,
        PROGRAM(
            LDA_IMM(0x01),
            CMP_IMM(0x01),
        );
        RUN_TEST(2);
        VALIDATE_REG(A, 0x01);
        VALIDATE_FLAG(C, 1);
        VALIDATE_FLAG(Z, 1);
        VALIDATE_FLAG(N, 0);
    );
)
