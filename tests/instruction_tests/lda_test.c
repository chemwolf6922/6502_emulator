#include "test_framework.h"

int main(int argc, char const *argv[])
{
    DEFINE_TEST(LDA immediate,
        PROGRAM( 
            LDA_IMM(0x12),
        );
        RUN_TEST(1);
        VALIDATE_REG(A, 0x12);
    );
    DEFINE_TEST(LDA absolute,
        memory[0x1000] = 0x12;
        PROGRAM(
            LDA_ABS(0x10, 0x00),
        );
        RUN_TEST(1);
        VALIDATE_REG(A, 0x12);
    );
    DEFINE_TEST(LDA zero_page,
        memory[0x10] = 0x12;
        PROGRAM(
            LDA_ZP(0x10),
        );
        RUN_TEST(1);
        VALIDATE_REG(A, 0x12);
    );
    DEFINE_TEST(LDA absolute_x,
        memory[0x1001] = 0x12;
        PROGRAM(
            LDX_IMM(0x01),
            LDA_ABX(0x10, 0x00),
        );
        RUN_TEST(2);
        VALIDATE_REG(A, 0x12);
    );
    DEFINE_TEST(LDA absolute_y,
        memory[0x1001] = 0x12;
        PROGRAM(
            LDY_IMM(0x01),
            LDA_ABY(0x10, 0x00),
        );
        RUN_TEST(2);
        VALIDATE_REG(A, 0x12);
    );
    DEFINE_TEST(LDA zero_page_x,
        memory[0x10] = 0x12;
        PROGRAM(
            LDX_IMM(0x01),
            LDA_ZPX(0x0F),
        );
        RUN_TEST(2);
        VALIDATE_REG(A, 0x12);
    );
    DEFINE_TEST(LDA x_indirect,
        memory[0x11] = 0x00;
        memory[0x12] = 0x10;
        memory[0x1000] = 0x12;
        PROGRAM(
            LDX_IMM(0x01),
            LDA_XI(0x10),
        );
        RUN_TEST(2);
        VALIDATE_REG(A, 0x12);
    );
    DEFINE_TEST(LDA indirect_y,
        memory[0x10] = 0x00;
        memory[0x11] = 0x10;
        memory[0x1001] = 0x12;
        PROGRAM(
            LDY_IMM(0x01),
            LDA_IY(0x10),
        );
        RUN_TEST(2);
        VALIDATE_REG(A, 0x12);
    );
    return 0;
}

