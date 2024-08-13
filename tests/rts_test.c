#include "test_framework.h"

TEST_MAIN(
    DEFINE_TEST(RTS,
        PROGRAM(
            LDX_IMM(0xFF),
            TXS(),
            JSR_ABS((PROGRAM_ENTRY + 8) >> 8, (PROGRAM_ENTRY + 8) & 0xFF),
            NOP(),
            NOP(),
            RTS(),
        );
        RUN_TEST(4);
        VALIDATE_REG(PC, PROGRAM_ENTRY + 6);
    );
)
