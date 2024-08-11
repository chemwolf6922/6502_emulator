#include "test_framework.h"

TEST_MAIN(
    DEFINE_TEST(SBC immediate,
        PROGRAM(
            SEC(),
            LDA_IMM(0x03),
            SBC_IMM(0x02),
        );
        RUN_TEST(3);
        VALIDATE_REG(A, 0x01);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 0);
        VALIDATE_FLAG(C, 1);
        VALIDATE_FLAG(V, 0);
    );
    DEFINE_TEST(SBC zeropage,
        memory[0x10] = 0x02;
        PROGRAM(
            SEC(),
            LDA_IMM(0x03),
            SBC_ZP(0x10),
        );
        RUN_TEST(3);
        VALIDATE_REG(A, 0x01);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 0);
        VALIDATE_FLAG(C, 1);
        VALIDATE_FLAG(V, 0);
    );
    DEFINE_TEST(SBC zeropage x,
        memory[0x10] = 0x02;
        PROGRAM(
            SEC(),
            LDA_IMM(0x03),
            LDX_IMM(0x01),
            SBC_ZPX(0x0F),
        );
        RUN_TEST(4);
        VALIDATE_REG(A, 0x01);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 0);
        VALIDATE_FLAG(C, 1);
        VALIDATE_FLAG(V, 0);
    );
    DEFINE_TEST(SBC absolute,
        memory[0x1000] = 0x02;
        PROGRAM(
            SEC(),
            LDA_IMM(0x03),
            SBC_ABS(0x10, 0x00),
        );
        RUN_TEST(3);
        VALIDATE_REG(A, 0x01);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 0);
        VALIDATE_FLAG(C, 1);
        VALIDATE_FLAG(V, 0);
    );
    DEFINE_TEST(SBC absolute x,
        memory[0x1000] = 0x02;
        PROGRAM(
            SEC(),
            LDA_IMM(0x03),
            LDX_IMM(0x01),
            SBC_ABX(0x0F, 0xFF),
        );
        RUN_TEST(4);
        VALIDATE_REG(A, 0x01);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 0);
        VALIDATE_FLAG(C, 1);
        VALIDATE_FLAG(V, 0);
    );
    DEFINE_TEST(SBC absolute y,
        memory[0x1000] = 0x02;
        PROGRAM(
            SEC(),
            LDA_IMM(0x03),
            LDY_IMM(0x01),
            SBC_ABY(0x0F, 0xFF),
        );
        RUN_TEST(4);
        VALIDATE_REG(A, 0x01);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 0);
        VALIDATE_FLAG(C, 1);
        VALIDATE_FLAG(V, 0);
    );
    DEFINE_TEST(SBC x indirect,
        memory[0x10] = 0x00;
        memory[0x11] = 0x10;
        memory[0x1000] = 0x02;
        PROGRAM(
            SEC(),
            LDA_IMM(0x03),
            LDX_IMM(0x10),
            SBC_XI(0x00),
        );
        RUN_TEST(4);
        VALIDATE_REG(A, 0x01);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 0);
        VALIDATE_FLAG(C, 1);
        VALIDATE_FLAG(V, 0);
    );
    DEFINE_TEST(SBC indirect y,
        memory[0x10] = 0x00;
        memory[0x11] = 0x10;
        memory[0x1010] = 0x02;
        PROGRAM(
            SEC(),
            LDA_IMM(0x03),
            LDY_IMM(0x10),
            SBC_IY(0x10),
        );
        RUN_TEST(4);
        VALIDATE_REG(A, 0x01);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 0);
        VALIDATE_FLAG(C, 1);
        VALIDATE_FLAG(V, 0);
    );
    DEFINE_TEST(SBC no carry,
        PROGRAM(
            SEC(),
            LDA_IMM(0x01),
            SBC_IMM(0x02),
        );
        RUN_TEST(3);
        VALIDATE_REG(A, 0xFF);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 1);
        VALIDATE_FLAG(C, 0);
        VALIDATE_FLAG(V, 0);
    );
    DEFINE_TEST(SBC sub with no carry,
        PROGRAM(
            LDA_IMM(0x01),
            SBC_IMM(0x01),
        );
        RUN_TEST(3);
        VALIDATE_REG(A, 0xFF);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 1);
        VALIDATE_FLAG(C, 0);
        VALIDATE_FLAG(V, 0);
    );
    DEFINE_TEST(SBC overflow,
        PROGRAM(
            SEC(),
            LDA_IMM(0x80),
            SBC_IMM(0x01),
        );
        RUN_TEST(3);
        VALIDATE_REG(A, 0x7F);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 0);
        VALIDATE_FLAG(C, 1);
        VALIDATE_FLAG(V, 1);
    );
    DEFINE_TEST(SBC overflow 2,
        PROGRAM(
            SEC(),
            LDA_IMM(0x7F),
            SBC_IMM(0xFF),
        );
        RUN_TEST(3);
        VALIDATE_REG(A, 0x80);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 1);
        VALIDATE_FLAG(C, 0);
        VALIDATE_FLAG(V, 1);
    );
    DEFINE_TEST(SBC bcd mode,
        PROGRAM(
            SED(),
            SEC(),
            LDA_IMM(0x42),
            SBC_IMM(0x35),
        );
        RUN_TEST(4);
        VALIDATE_REG(A, 0x07);
        VALIDATE_FLAG(D, 1);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(C, 1);
    );
    DEFINE_TEST(SBC bcd mode borrow,
        PROGRAM(
            SED(),
            SEC(),
            LDA_IMM(0x42),
            SBC_IMM(0x43),
        );
        RUN_TEST(4);
        VALIDATE_REG(A, 0x99);
        VALIDATE_FLAG(D, 1);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(C, 0);
    );
    DEFINE_TEST(SBC bcd mode sub with borrow,
        PROGRAM(
            SED(),
            LDA_IMM(0x42),
            SBC_IMM(0x43),
        );
        RUN_TEST(5);
        VALIDATE_REG(A, 0x98);
        VALIDATE_FLAG(D, 1);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(C, 0);
    );
)
