#include "test_framework.h"

TEST_MAIN(
    DEFINE_TEST(DEC absolute,
        PROGRAM(
            LDA_IMM(0x12),
            STA_ABS(0x10, 0x00),
            DEC_ABS(0x10, 0x00),
        );
        RUN_TEST(3);
        VALIDATE_MEM(0x1000, 0x11);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(DEC zero_page,
        PROGRAM(
            LDA_IMM(0x12),
            STA_ZP(0x10),
            DEC_ZP(0x10),
        );
        RUN_TEST(3);
        VALIDATE_MEM(0x10, 0x11);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(DEC absolute_x,
        PROGRAM(
            LDA_IMM(0x12),
            STA_ABS(0x10, 0x01),
            LDX_IMM(0x01),
            DEC_ABX(0x10, 0x00),
        );
        RUN_TEST(4);
        VALIDATE_MEM(0x1001, 0x11);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(DEC zero_page_x,
        PROGRAM(
            LDA_IMM(0x12),
            STA_ZP(0x10),
            LDX_IMM(0x01),
            DEC_ZPX(0x0F),
        );
        RUN_TEST(4);
        VALIDATE_MEM(0x10, 0x11);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(DEC N flag,
        PROGRAM(
            LDA_IMM(0x81),
            STA_ZP(0x10),
            LDA_IMM(0x00),
            DEC_ZP(0x10),
        );
        RUN_TEST(4);
        VALIDATE_MEM(0x10, 0x80);
        VALIDATE_FLAG(N, 1);
        VALIDATE_FLAG(Z, 0);
    );
    DEFINE_TEST(DEC Z flag,
        PROGRAM(
            LDA_IMM(0x01),
            STA_ZP(0x10),
            LDA_IMM(0x12),
            DEC_ZP(0x10),
        );
        RUN_TEST(4);
        VALIDATE_MEM(0x10, 0x00);
        VALIDATE_FLAG(N, 0);
        VALIDATE_FLAG(Z, 1);
    );
)
