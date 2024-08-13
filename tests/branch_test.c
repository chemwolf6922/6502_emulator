#include "test_framework.h"

TEST_MAIN(
    DEFINE_TEST(BCC,
        PROGRAM(
            BCC_REL(0x10),
        );
        RUN_TEST(1);
        VALIDATE_REG(PC, PROGRAM_ENTRY + 0x12);
    );
    DEFINE_TEST(BCC no branch,
        PROGRAM(
            SEC(),
            BCC_REL(0x10),
        );
        RUN_TEST(2);
        VALIDATE_REG(PC, PROGRAM_ENTRY + 0x03);
    );
    DEFINE_TEST(BCC negative,
        PROGRAM(
            BCC_REL(0xF0),
        );
        RUN_TEST(1);
        VALIDATE_REG(PC, PROGRAM_ENTRY + 0x02 - 0x10);
    );
    DEFINE_TEST(BCS,
        PROGRAM(
            SEC(),
            BCS_REL(0x10),
        );
        RUN_TEST(2);
        VALIDATE_REG(PC, PROGRAM_ENTRY + 0x13);
    );
    DEFINE_TEST(BCS no branch,
        PROGRAM(
            BCS_REL(0x10),
        );
        RUN_TEST(1);
        VALIDATE_REG(PC, PROGRAM_ENTRY + 0x02);
    );
    DEFINE_TEST(BEQ,
        PROGRAM(
            LDA_IMM(0x00),
            BEQ_REL(0x10),
        );
        RUN_TEST(2);
        VALIDATE_REG(PC, PROGRAM_ENTRY + 0x14);
    );
    DEFINE_TEST(BEQ no branch,
        PROGRAM(
            LDA_IMM(0x01),
            BEQ_REL(0x10),
        );
        RUN_TEST(2);
        VALIDATE_REG(PC, PROGRAM_ENTRY + 0x04);
    );
    DEFINE_TEST(BNE,
        PROGRAM(
            LDA_IMM(0x01),
            BNE_REL(0x10),
        );
        RUN_TEST(2);
        VALIDATE_REG(PC, PROGRAM_ENTRY + 0x14);
    );
    DEFINE_TEST(BNE no branch,
        PROGRAM(
            LDA_IMM(0x00),
            BNE_REL(0x10),
        );
        RUN_TEST(2);
        VALIDATE_REG(PC, PROGRAM_ENTRY + 0x04);
    );
    DEFINE_TEST(BMI,
        PROGRAM(
            LDA_IMM(0x80),
            BMI_REL(0x10),
        );
        RUN_TEST(2);
        VALIDATE_REG(PC, PROGRAM_ENTRY + 0x14);
    );
    DEFINE_TEST(BMI no branch,
        PROGRAM(
            LDA_IMM(0x7F),
            BMI_REL(0x10),
        );
        RUN_TEST(2);
        VALIDATE_REG(PC, PROGRAM_ENTRY + 0x04);
    );
    DEFINE_TEST(BPL,
        PROGRAM(
            LDA_IMM(0x7F),
            BPL_REL(0x10),
        );
        RUN_TEST(2);
        VALIDATE_REG(PC, PROGRAM_ENTRY + 0x14);
    );
    DEFINE_TEST(BPL no branch,
        PROGRAM(
            LDA_IMM(0x80),
            BPL_REL(0x10),
        );
        RUN_TEST(2);
        VALIDATE_REG(PC, PROGRAM_ENTRY + 0x04);
    );
    DEFINE_TEST(BVC,
        PROGRAM(
            BVC_REL(0x10),
        );
        RUN_TEST(1);
        VALIDATE_REG(PC, PROGRAM_ENTRY + 0x12);
    );
    DEFINE_TEST(BVC no branch,
        PROGRAM(
            LDA_IMM(0x7F),
            ADC_IMM(0x7F),
            BVC_REL(0x10),
        );
        RUN_TEST(3);
        VALIDATE_FLAG(V, 1);
        VALIDATE_REG(PC, PROGRAM_ENTRY + 0x06);
    );
    DEFINE_TEST(BVS,
        PROGRAM(
            LDA_IMM(0x7F),
            ADC_IMM(0x7F),
            BVS_REL(0x10),
        );
        RUN_TEST(3);
        VALIDATE_FLAG(V, 1);
        VALIDATE_REG(PC, PROGRAM_ENTRY + 0x16);
    );
    DEFINE_TEST(BVS no branch,
        PROGRAM(
            BVS_REL(0x10),
        );
        RUN_TEST(1);
        VALIDATE_REG(PC, PROGRAM_ENTRY + 0x02);
    );
)
