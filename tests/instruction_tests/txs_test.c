#include "test_framework.h"

TEST_MAIN(
    DEFINE_TEST(TXS,
        PROGRAM(
            LDX_IMM(0x12),
            TXS(),
        );
        RUN_TEST(2);
        VALIDATE_REG(SP, 0x12);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(TXS N flag,
        PROGRAM(
            LDX_IMM(0x80),
            LDA_IMM(0x01),
            TXS(),
        );
        RUN_TEST(3);
        VALIDATE_REG(SP, 0x80);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(TXS Z flag,
        PROGRAM(
            LDX_IMM(0x00),
            LDA_IMM(0x01),
            TXS(),
        );
        RUN_TEST(3);
        VALIDATE_REG(SP, 0x00);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 0);
    );
)
