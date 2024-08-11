#include "test_framework.h"

TEST_MAIN(
    DEFINE_TEST(INY,
        PROGRAM(
            LDY_IMM(0x12),
            INY(),
        );
        RUN_TEST(2);
        VALIDATE_REG(Y, 0x13);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(INY N flag,
        PROGRAM(
            LDY_IMM(0xFF),
            INY(),
        );
        RUN_TEST(2);
        VALIDATE_REG(Y, 0x00);
        VALIDATE_FLAG(N, 0);
        VALIDATE_FLAG(Z, 1);
    );
    DEFINE_TEST(INY Z flag,
        PROGRAM(
            LDY_IMM(0x7F),
            INY(),
        );
        RUN_TEST(2);
        VALIDATE_REG(Y, 0x80);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 1);
    );
)