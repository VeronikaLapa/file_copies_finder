#include "tst_finder.h"

#include <gtest/gtest.h>

int main(int argc, char *argv[])
{
    dir = QDir("test_source");
    if (!dir.exists()) {
        QDir().mkdir("test_source");
    }
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
