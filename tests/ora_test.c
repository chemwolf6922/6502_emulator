#include "test_framework.h"

TEST_MAIN(
    DEFINE_TEST(ORA immediate,
        PROGRAM(
            LDA_IMM(0x0C),
            ORA_IMM(0x03),
        );
        RUN_TEST(2);
        VALIDATE_REG(A, 0x0F);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(ORA zeropage,
        memory[0x10] = 0x0C;
        PROGRAM(
            LDA_IMM(0x03),
            ORA_ZP(0x10),
        );
        RUN_TEST(2);
        VALIDATE_REG(A, 0x0F);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(ORA zeropage X,
        memory[0x10] = 0x0C;
        PROGRAM(
            LDA_IMM(0x03),
            LDX_IMM(0x01),
            ORA_ZPX(0x0F),
        );
        RUN_TEST(3);
        VALIDATE_REG(A, 0x0F);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(ORA absolute,
        memory[0x1000] = 0x0C;
        PROGRAM(
            LDA_IMM(0x03),
            ORA_ABS(0x10, 0x00),
        );
        RUN_TEST(2);
        VALIDATE_REG(A, 0x0F);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(ORA absolute X,
        memory[0x1000] = 0x0C;
        PROGRAM(
            LDA_IMM(0x03),
            LDX_IMM(0x01),
            ORA_ABX(0x0F, 0xFF),
        );
        RUN_TEST(3);
        VALIDATE_REG(A, 0x0F);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(ORA absolute Y,
        memory[0x1000] = 0x0C;
        PROGRAM(
            LDA_IMM(0x03),
            LDY_IMM(0x01),
            ORA_ABY(0x0F, 0xFF),
        );
        RUN_TEST(3);
        VALIDATE_REG(A, 0x0F);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(ORA X indirect,
        memory[0x10] = 0x00;
        memory[0x11] = 0x10;
        memory[0x1000] = 0x0C;
        PROGRAM(
            LDA_IMM(0x03),
            LDX_IMM(0x01),
            ORA_XI(0x0F),
        );
        RUN_TEST(3);
        VALIDATE_REG(A, 0x0F);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(ORA indirect Y,
        memory[0x10] = 0x00;
        memory[0x11] = 0x10;
        memory[0x1010] = 0x0C;
        PROGRAM(
            LDA_IMM(0x03),
            LDY_IMM(0x10),
            ORA_IY(0x10),
        );
        RUN_TEST(3);
        VALIDATE_REG(A, 0x0F);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(ORA N flag,
        PROGRAM(
            LDA_IMM(0x08),
            ORA_IMM(0x80),
        );
        RUN_TEST(2);
        VALIDATE_REG(A, 0x88);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 1);
    );
    DEFINE_TEST(ORA Z flag,
        PROGRAM(
            ORA_IMM(0x00),
        );
        RUN_TEST(1);
        VALIDATE_REG(A, 0x00);
        VALIDATE_FLAG(Z, 1);
        VALIDATE_FLAG(N, 0);
    );
)
