#include "test_framework.h"

TEST_MAIN(
    DEFINE_TEST(DEY,
        PROGRAM(
            LDY_IMM(0x12),
            DEY(),
        );
        RUN_TEST(2);
        VALIDATE_REG(Y, 0x11);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(DEY N flag,
        PROGRAM(
            LDY_IMM(0x00),
            DEY(),
        );
        RUN_TEST(2);
        VALIDATE_REG(Y, 0xFF);
        VALIDATE_FLAG(N, 1);
        VALIDATE_FLAG(Z, 0);
    );
    DEFINE_TEST(DEY Z flag,
        PROGRAM(
            LDY_IMM(0x01),
            DEY(),
        );
        RUN_TEST(2);
        VALIDATE_REG(Y, 0x00);
        VALIDATE_FLAG(Z, 1);
        VALIDATE_FLAG(N, 0);
    );
)