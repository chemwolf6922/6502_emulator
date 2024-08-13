#include "test_framework.h"

TEST_MAIN(
    DEFINE_TEST(JMP absolute,
        PROGRAM(
            JMP_ABS(0x10, 0x00),
        );
        RUN_TEST(1);
        VALIDATE_REG(PC, 0x1000);
    );
    DEFINE_TEST(JMP indirect,
        memory[0x1000] = 0x00;
        memory[0x1001] = 0x20;
        PROGRAM(
            JMP_IND(0x10, 0x00),
        );
        RUN_TEST(1);
        VALIDATE_REG(PC, 0x2000);
    );
)
