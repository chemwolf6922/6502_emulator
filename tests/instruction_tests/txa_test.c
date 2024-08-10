#include "test_framework.h"

TEST_MAIN(
    DEFINE_TEST(TXA ,
        PROGRAM(
            LDX_IMM(0x12),
            TXA(),
        );
        RUN_TEST(2);
        VALIDATE_REG(A, 0x12);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(TXA N flag,
        PROGRAM(
            LDX_IMM(0x80),
            LDY_IMM(0x01),
            TXA(),
        );
        RUN_TEST(3);
        VALIDATE_REG(A, 0x80);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 1);
    );
    DEFINE_TEST(TXA Z flag,
        PROGRAM(
            LDX_IMM(0x00),
            LDY_IMM(0x01),
            TXA(),
        );
        RUN_TEST(3);
        VALIDATE_REG(A, 0x00);
        VALIDATE_FLAG(Z, 1);
        VALIDATE_FLAG(N, 0);
    );
)
