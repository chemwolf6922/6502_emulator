#include "test_framework.h"

TEST_MAIN(
    DEFINE_TEST(JSR,
        PROGRAM(
            LDX_IMM(0xFF),
            TXS(),
            JSR_ABS(0x10, 0x00),
        );
        RUN_TEST(3);
        VALIDATE_REG(PC, 0x1000);
        VALIDATE_MEM(0x1FF, (PROGRAM_ENTRY + 5) >> 8 );
        VALIDATE_MEM(0x1FE, (PROGRAM_ENTRY + 5) & 0xFF);
    );
)
