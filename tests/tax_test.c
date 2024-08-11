#include "test_framework.h"

TEST_MAIN(
    DEFINE_TEST(TAX ,
        PROGRAM(
            LDA_IMM(0x12),
            TAX(),
        );
        RUN_TEST(2);
        VALIDATE_REG(X, 0x12);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(TAX N flag,
        PROGRAM(
            LDA_IMM(0x80),
            LDY_IMM(0x01),
            TAX(),
        );
        RUN_TEST(3);
        VALIDATE_REG(X, 0x80);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 1);
    );
    DEFINE_TEST(TAX Z flag,
        PROGRAM(
            LDA_IMM(0x00),
            LDY_IMM(0x01),
            TAX(),
        );
        RUN_TEST(3);
        VALIDATE_REG(X, 0x00);
        VALIDATE_FLAG(Z, 1);
        VALIDATE_FLAG(N, 0);
    );
)

