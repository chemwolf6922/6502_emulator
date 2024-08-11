#include "test_framework.h"

TEST_MAIN(
    DEFINE_TEST(INX,
        PROGRAM(
            LDX_IMM(0x12),
            INX(),
        );
        RUN_TEST(2);
        VALIDATE_REG(X, 0x13);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(INX N flag,
        PROGRAM(
            LDX_IMM(0xFF),
            INX(),
        );
        RUN_TEST(2);
        VALIDATE_REG(X, 0x00);
        VALIDATE_FLAG(N, 0);
        VALIDATE_FLAG(Z, 1);
    );
    DEFINE_TEST(INX Z flag,
        PROGRAM(
            LDX_IMM(0x7F),
            INX(),
        );
        RUN_TEST(2);
        VALIDATE_REG(X, 0x80);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 1);
    );
)
