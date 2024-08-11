#include "test_framework.h"

TEST_MAIN(
    DEFINE_TEST(DEX,
        PROGRAM(
            LDX_IMM(0x12),
            DEX(),
        );
        RUN_TEST(2);
        VALIDATE_REG(X, 0x11);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(DEX N flag,
        PROGRAM(
            LDX_IMM(0x00),
            DEX(),
        );
        RUN_TEST(2);
        VALIDATE_REG(X, 0xFF);
        VALIDATE_FLAG(N, 1);
        VALIDATE_FLAG(Z, 0);
    );
    DEFINE_TEST(DEX Z flag,
        PROGRAM(
            LDX_IMM(0x01),
            DEX(),
        );
        RUN_TEST(2);
        VALIDATE_REG(X, 0x00);
        VALIDATE_FLAG(Z, 1);
        VALIDATE_FLAG(N, 0);
    );
)
