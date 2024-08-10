#include "test_framework.h"

TEST_MAIN(
    DEFINE_TEST(TAY ,
        PROGRAM(
            LDA_IMM(0x12),
            TAY(),
        );
        RUN_TEST(2);
        VALIDATE_REG(Y, 0x12);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(TAY N flag,
        PROGRAM(
            LDA_IMM(0x80),
            LDY_IMM(0x01),
            TAY(),
        );
        RUN_TEST(3);
        VALIDATE_REG(Y, 0x80);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 1);
    );
    DEFINE_TEST(TAY Z flag,
        PROGRAM(
            LDA_IMM(0x00),
            LDY_IMM(0x01),
            TAY(),
        );
        RUN_TEST(3);
        VALIDATE_REG(Y, 0x00);
        VALIDATE_FLAG(Z, 1);
        VALIDATE_FLAG(N, 0);
    );
)
