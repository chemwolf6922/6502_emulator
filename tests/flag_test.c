#include "test_framework.h"

TEST_MAIN(
    DEFINE_TEST(CLC,
        PROGRAM(
            CLC(),
        );
        RUN_TEST(1);
        VALIDATE_FLAG(C, 0);
    );
    DEFINE_TEST(CLD,
        PROGRAM(
            CLD(),
        );
        RUN_TEST(1);
        VALIDATE_FLAG(D, 0);
    );
    DEFINE_TEST(CLI,
        PROGRAM(
            CLI(),
        );
        RUN_TEST(1);
        VALIDATE_FLAG(I, 0);
    );
    DEFINE_TEST(CLV,
        PROGRAM(
            CLV(),
        );
        RUN_TEST(1);
        VALIDATE_FLAG(V, 0);
    );
    DEFINE_TEST(SEC,
        PROGRAM(
            SEC(),
        );
        RUN_TEST(1);
        VALIDATE_FLAG(C, 1);
    );
    DEFINE_TEST(SED,
        PROGRAM(
            SED(),
        );
        RUN_TEST(1);
        VALIDATE_FLAG(D, 1);
    );
    DEFINE_TEST(SEI,
        PROGRAM(
            SEI(),
        );
        RUN_TEST(1);
        VALIDATE_FLAG(I, 1);
    );
)
