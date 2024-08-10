#include "test_framework.h"

TEST_MAIN(
    DEFINE_TEST(STA absolute,
        PROGRAM(
            LDA_IMM(0x12),
            STA_ABS(0x10, 0x00),
        );
        RUN_TEST(2);
        VALIDATE_MEM(0x1000, 0x12);
    );
    DEFINE_TEST(STA zero_page,
        PROGRAM(
            LDA_IMM(0x12),
            STA_ZP(0x10),
        );
        RUN_TEST(2);
        VALIDATE_MEM(0x10, 0x12);
    );
    DEFINE_TEST(STA absolute_x,
        PROGRAM(
            LDA_IMM(0x12),
            LDX_IMM(0x01),
            STA_ABX(0x10, 0x00),
        );
        RUN_TEST(3);
        VALIDATE_MEM(0x1001, 0x12);
    );
    DEFINE_TEST(STA absolute_y,
        PROGRAM(
            LDA_IMM(0x12),
            LDY_IMM(0x01),
            STA_ABY(0x10, 0x00),
        );
        RUN_TEST(3);
        VALIDATE_MEM(0x1001, 0x12);
    );
    DEFINE_TEST(STA zero_page_x,
        PROGRAM(
            LDA_IMM(0x12),
            LDX_IMM(0x01),
            STA_ZPX(0x0F),
        );
        RUN_TEST(3);
        VALIDATE_MEM(0x10, 0x12);
    );
    DEFINE_TEST(STA x_indirect,
        memory[0x11] = 0x00;
        memory[0x12] = 0x20;
        PROGRAM(
            LDA_IMM(0x12),
            LDX_IMM(0x01),
            STA_XI(0x10),
        );
        RUN_TEST(3);
        VALIDATE_MEM(0x2000, 0x12);
    );
    DEFINE_TEST(STA indirect_y,
        memory[0x10] = 0x00;
        memory[0x11] = 0x20;
        PROGRAM(
            LDA_IMM(0x12),
            LDY_IMM(0x01),
            STA_IY(0x10),
        );
        RUN_TEST(3);
        VALIDATE_MEM(0x2001, 0x12);
    );
)
