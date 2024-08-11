#include "test_framework.h"

TEST_MAIN(
    DEFINE_TEST(TSX ,
        PROGRAM(
            PHA(),
            TSX(),
        );
        RUN_TEST(2);
        VALIDATE_REG(X, 0xFF);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 1);
    );
    DEFINE_TEST(TSX N flag,
        PROGRAM(
            PLA(),
            TSX(),
        );
        RUN_TEST(2);
        VALIDATE_REG(X, 0x01);
        VALIDATE_FLAG(Z, 0);
        VALIDATE_FLAG(N, 0);
    );
    DEFINE_TEST(TSX Z flag,
        PROGRAM(
            TSX(),
        );
        RUN_TEST(1);
        VALIDATE_REG(X, 0x00);
        VALIDATE_FLAG(Z, 1);
        VALIDATE_FLAG(N, 0);
    );
)
