#include "test_framework.h"

TEST_MAIN(
    DEFINE_TEST(PHA,
        PROGRAM(
            LDX_IMM(0xFF),
            TXS(),
            LDA_IMM(0x12),
            PHA(),
        );
        RUN_TEST(4);
        VALIDATE_REG(SP, 0xFE);
        VALIDATE_MEM(0x01FF, 0x12);
    );
    DEFINE_TEST(PLA,
        PROGRAM(
            LDX_IMM(0xFF),
            TXS(),
            LDA_IMM(0x12),
            PHA(),
            LDA_IMM(0x00),
            PLA(),
        );
        RUN_TEST(6);
        VALIDATE_REG(SP, 0xFF);
        VALIDATE_REG(A, 0x12);
    );
    DEFINE_TEST(PLA N flag,
        PROGRAM(
            LDX_IMM(0xFF),
            TXS(),
            LDA_IMM(0x80),
            PHA(),
            LDA_IMM(0x00),
            PLA(),
        );
        RUN_TEST(6);
        VALIDATE_REG(SP, 0xFF);
        VALIDATE_REG(A, 0x80);
        VALIDATE_FLAG(N, 1);
        VALIDATE_FLAG(Z, 0);
    );
    DEFINE_TEST(PLA Z flag,
        PROGRAM(
            LDX_IMM(0xFF),
            TXS(),
            LDA_IMM(0x00),
            PHA(),
            LDA_IMM(0x01),
            PLA(),
        );
        RUN_TEST(6);
        VALIDATE_REG(SP, 0xFF);
        VALIDATE_REG(A, 0x00);
        VALIDATE_FLAG(N, 0);
        VALIDATE_FLAG(Z, 1);
    );
    DEFINE_TEST(PHP,
        PROGRAM(
            LDX_IMM(0xFF),
            TXS(),
            PHP(),
        );
        RUN_TEST(3);
        VALIDATE_REG(SP, 0xFE);
        VALIDATE_MEM(0x01FF, 0b10100100);
    );
    DEFINE_TEST(PLP,
        PROGRAM(
            LDX_IMM(0xFF),
            TXS(),
            PHP(),
            LDA_IMM(0x00),
            PLP(),
        );
        RUN_TEST(5);
        VALIDATE_REG(SP, 0xFF);
        VALIDATE_REG(status.byte, 0b10100100);
    );
)
