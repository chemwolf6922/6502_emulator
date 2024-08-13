#include "test_framework.h"

TEST_MAIN(
    DEFINE_TEST(ASL accumulator,
        PROGRAM(
            LDA_IMM(0x01),
            ASL_A(),
        );
        RUN_TEST(2);
        VALIDATE_REG(A, 0x02);
        VALIDATE_FLAG(C, 0);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(ASL zeropage,
        memory[0x10] = 0x01;
        PROGRAM(
            ASL_ZP(0x10),
        );
        RUN_TEST(1);
        VALIDATE_MEM(0x10, 0x02);
        VALIDATE_FLAG(C, 0);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(ASL zeropage X,
        memory[0x10] = 0x01;
        PROGRAM(
            LDX_IMM(0x01),
            ASL_ZPX(0x0F),
        );
        RUN_TEST(2);
        VALIDATE_MEM(0x10, 0x02);
        VALIDATE_FLAG(C, 0);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(ASL absolute,
        memory[0x1000] = 0x01;
        PROGRAM(
            ASL_ABS(0x10, 0x00),
        );
        RUN_TEST(1);
        VALIDATE_MEM(0x1000, 0x02);
        VALIDATE_FLAG(C, 0);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(ASL absolute X,
        memory[0x1000] = 0x01;
        PROGRAM(
            LDX_IMM(0x01),
            ASL_ABX(0x0F, 0xFF),
        );
        RUN_TEST(2);
        VALIDATE_MEM(0x1000, 0x02);
        VALIDATE_FLAG(C, 0);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(ASL Z flag,
        memory[0x10] = 0x00;
        PROGRAM(
            ASL_ZP(0x10),
        );
        RUN_TEST(1);
        VALIDATE_MEM(0x10, 0x00);
        VALIDATE_FLAG(C, 0);
        VALIDATE_FLAG(Z, 1);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(ASL C flag,
        memory[0x10] = 0x81;
        PROGRAM(
            ASL_ZP(0x10),
        );
        RUN_TEST(1);
        VALIDATE_MEM(0x10, 0x02);
        VALIDATE_FLAG(C, 1);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(ASL N flag,
        memory[0x10] = 0x40;
        PROGRAM(
            ASL_ZP(0x10),
        );
        RUN_TEST(1);
        VALIDATE_MEM(0x10, 0x80);
        VALIDATE_FLAG(C, 0);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 1);
    )
)
