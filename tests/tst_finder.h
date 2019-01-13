#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include "finder.h"

#include <QDir>

using namespace testing;

TEST(finder, find)
{
    EXPECT_EQ(1, 1);
    ASSERT_THAT(0, Eq(0));
}

TEST(finder, EXAMPLE2)
{
    QDir directory = QDir("D:/University/2018-2019/cpp/dirdemo-master");
    finder f = finder(directory);
    EXPECT_EQ(1, 1);
}
