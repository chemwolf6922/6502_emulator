#include "test_framework.h"

TEST_MAIN(
    DEFINE_TEST(ADC immediate,
        PROGRAM(
            LDA_IMM(0x01),
            ADC_IMM(0x02),
        );
        RUN_TEST(2);
        VALIDATE_REG(A, 0x03);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 0);
        VALIDATE_FLAG(C, 0);
        VALIDATE_FLAG(V, 0);
    );
    DEFINE_TEST(ADC zeropage,
        memory[0x10] = 0x01;
        PROGRAM(
            LDA_IMM(0x02),
            ADC_ZP(0x10),
        );
        RUN_TEST(2);
        VALIDATE_REG(A, 0x03);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 0);
        VALIDATE_FLAG(C, 0);
        VALIDATE_FLAG(V, 0);
    );
    DEFINE_TEST(ADC zeropage X,
        memory[0x10] = 0x01;
        PROGRAM(
            LDA_IMM(0x02),
            LDX_IMM(0x01),
            ADC_ZPX(0x0F),
        );
        RUN_TEST(3);
        VALIDATE_REG(A, 0x03);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 0);
        VALIDATE_FLAG(C, 0);
        VALIDATE_FLAG(V, 0);
    );
    DEFINE_TEST(ADC absolute,
        memory[0x1000] = 0x01;
        PROGRAM(
            LDA_IMM(0x02),
            ADC_ABS(0x10, 0x00),
        );
        RUN_TEST(2);
        VALIDATE_REG(A, 0x03);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 0);
        VALIDATE_FLAG(C, 0);
        VALIDATE_FLAG(V, 0);
    );
    DEFINE_TEST(ADC absolute X,
        memory[0x1000] = 0x01;
        PROGRAM(
            LDA_IMM(0x02),
            LDX_IMM(0x01),
            ADC_ABX(0x0F, 0xFF),
        );
        RUN_TEST(3);
        VALIDATE_REG(A, 0x03);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 0);
        VALIDATE_FLAG(C, 0);
        VALIDATE_FLAG(V, 0);
    );
    DEFINE_TEST(ADC absolute Y,
        memory[0x1000] = 0x01;
        PROGRAM(
            LDA_IMM(0x02),
            LDY_IMM(0x01),
            ADC_ABY(0x0F, 0xFF),
        );
        RUN_TEST(3);
        VALIDATE_REG(A, 0x03);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 0);
        VALIDATE_FLAG(C, 0);
        VALIDATE_FLAG(V, 0);
    );
    DEFINE_TEST(ADC x indirect,
        memory[0x10] = 0x00;
        memory[0x11] = 0x10;
        memory[0x1000] = 0x02;
        PROGRAM(
            LDA_IMM(0x01),
            LDX_IMM(0x01),
            ADC_XI(0x0F),
        );
        RUN_TEST(3);
        VALIDATE_REG(A, 0x03);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 0);
        VALIDATE_FLAG(C, 0);
        VALIDATE_FLAG(V, 0);
    );
    DEFINE_TEST(ADC indirect y,
        memory[0x10] = 0x00;
        memory[0x11] = 0x10;
        memory[0x1001] = 0x02;
        PROGRAM(
            LDA_IMM(0x01),
            LDY_IMM(0x01),
            ADC_IY(0x10),
        );
        RUN_TEST(3);
        VALIDATE_REG(A, 0x03);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 0);
        VALIDATE_FLAG(C, 0);
        VALIDATE_FLAG(V, 0);
    );
    DEFINE_TEST(ADC carry,
        PROGRAM(
            LDA_IMM(0xFF),
            ADC_IMM(0x01),
        );
        RUN_TEST(2);
        VALIDATE_REG(A, 0x00);
        VALIDATE_FLAG(Z, 1);
        VALIDATE_FLAG(N, 0);
        VALIDATE_FLAG(C, 1);
        VALIDATE_FLAG(V, 0);
    );
    DEFINE_TEST(ADC add with carry,
        PROGRAM(
            LDA_IMM(0xFF),
            ADC_IMM(0x01),
            ADC_IMM(0x01),
        );
        RUN_TEST(3);
        VALIDATE_REG(A, 0x02);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 0);
        VALIDATE_FLAG(C, 0);
        VALIDATE_FLAG(V, 0);
    );
    DEFINE_TEST(ADC add with carry and carry,
        PROGRAM(
            LDA_IMM(0xFF),
            ADC_IMM(0x01),
            ADC_IMM(0xFF),
        );
        RUN_TEST(3);
        VALIDATE_REG(A, 0x00);
        VALIDATE_FLAG(Z, 1);
        VALIDATE_FLAG(N, 0);
        VALIDATE_FLAG(C, 1);
        VALIDATE_FLAG(V, 0);
    );
    DEFINE_TEST(ADC N flag,
        PROGRAM(
            LDA_IMM(0x7F),
            ADC_IMM(0x01),
        );
        RUN_TEST(2);
        VALIDATE_REG(A, 0x80);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 1);
        VALIDATE_FLAG(C, 0);
        VALIDATE_FLAG(V, 1);
    );
    DEFINE_TEST(ADC V flag,
        PROGRAM(
            LDA_IMM(0x81),
            ADC_IMM(0x81),
        );
        RUN_TEST(2);
        VALIDATE_REG(A, 0x02);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 0);
        VALIDATE_FLAG(C, 1);
        VALIDATE_FLAG(V, 1);
    );
    DEFINE_TEST(ADC bcd mode,
        PROGRAM(
            SED(),
            LDA_IMM(0x15),
            ADC_IMM(0x16),
        );
        RUN_TEST(3);
        VALIDATE_REG(A, 0x31);
        VALIDATE_FLAG(D, 1);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(C, 0);
    );
    DEFINE_TEST(ADC bcd mode carry,
        PROGRAM(
            SED(),
            LDA_IMM(0x49),
            ADC_IMM(0x54),
        );
        RUN_TEST(3);
        VALIDATE_REG(A, 0x03);
        VALIDATE_FLAG(D, 1);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(C, 1);
    );
    DEFINE_TEST(ADC bcd mode add with carry,
        PROGRAM(
            SED(),
            LDA_IMM(0x49),
            ADC_IMM(0x54),
            ADC_IMM(0x01),
        );
        RUN_TEST(4);
        VALIDATE_REG(A, 0x05);
        VALIDATE_FLAG(D, 1);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(C, 0);
    );
    DEFINE_TEST(ADC bcd mode add with carry and carry,
        PROGRAM(
            SED(),
            LDA_IMM(0x49),
            ADC_IMM(0x54),
            ADC_IMM(0x99),
        );
        RUN_TEST(4);
        VALIDATE_REG(A, 0x03);
        VALIDATE_FLAG(D, 1);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(C, 1);
    );
    DEFINE_TEST(ADC bcd mode Z flag,
        PROGRAM(
            SED(),
            LDA_IMM(0x40),
            ADC_IMM(0x60),
        );
        RUN_TEST(3);
        VALIDATE_REG(A, 0x00);
        VALIDATE_FLAG(D, 1);
        VALIDATE_FLAG(Z, 1);
        VALIDATE_FLAG(C, 1);
    );
)

