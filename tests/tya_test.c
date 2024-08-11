#include "test_framework.h"

TEST_MAIN(
    DEFINE_TEST(TYA ,
        PROGRAM(
            LDY_IMM(0x12),
            TYA(),
        );
        RUN_TEST(2);
        VALIDATE_REG(A, 0x12);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(TYA N flag,
        PROGRAM(
            LDY_IMM(0x80),
            LDX_IMM(0x01),
            TYA(),
        );
        RUN_TEST(3);
        VALIDATE_REG(A, 0x80);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 1);
    );
    DEFINE_TEST(TYA Z flag,
        PROGRAM(
            LDY_IMM(0x00),
            LDX_IMM(0x01),
            TYA(),
        );
        RUN_TEST(3);
        VALIDATE_REG(A, 0x00);
        VALIDATE_FLAG(Z, 1);
        VALIDATE_FLAG(N, 0);
    );
)
