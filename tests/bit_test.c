#include "test_framework.h"

TEST_MAIN(
    DEFINE_TEST(BIT zeropage,
        memory[0x10] = 0xC0;
        PROGRAM(
            LDA_IMM(0x80),
            BIT_ZP(0x10),
        );
        RUN_TEST(2);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(V, 1);
        VALIDATE_FLAG(N, 1);
    );
    DEFINE_TEST(BIT absolute,
        memory[0x1000] = 0xC0;
        PROGRAM(
            LDA_IMM(0x80),
            BIT_ABS(0x10, 0x00),
        );
        RUN_TEST(2);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(V, 1);
        VALIDATE_FLAG(N, 1);
    );
    DEFINE_TEST(BIT Z flag,
        memory[0x10] = 0x0F;
        PROGRAM(
            LDA_IMM(0xF0),
            BIT_ZP(0x10),
        );
        RUN_TEST(2);
        VALIDATE_FLAG(Z, 1);
        VALIDATE_FLAG(V, 0);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(BIT V flag,
        memory[0x10] = 0x7F;
        PROGRAM(
            LDA_IMM(0x0F),
            BIT_ZP(0x10),
        );
        RUN_TEST(2);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(V, 1);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(BIT N flag,
        memory[0x10] = 0x80;
        PROGRAM(
            LDA_IMM(0x80),
            BIT_ZP(0x10),
        );
        RUN_TEST(2);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(V, 0);
        VALIDATE_FLAG(N, 1);
    );
)
